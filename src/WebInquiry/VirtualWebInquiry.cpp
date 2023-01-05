///////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 申请数据时使用了MFC的CInternetSession的OpenURL函数。其速度超过libcurl中的curl_easy_perform()。
///	同时申请若干个数据时，由于受到网络带宽的影响，下载速度会互相影响，导致时间延长。如下载Tiingo的StockSymbol时，
///	瞬间下载速度会达到70M bit/秒， 导致sina实时数据的下载时间由几十毫秒 延长至300-700毫秒。还可以接受。
/// 使用libcurl的curl_multi_perform多线程模式，估计能有所改善，但感觉也不会比MFC好多少。
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "VirtualWebInquiry.h"
#include"ThreadStatus.h"
#include"Thread.h"

#include"InfoReport.h"
#include"HighPerformanceCounter.h"

#include"ChinaMarket.h"
#include"curl/curl.h"

#include<thread>
using std::thread;

atomic_long CVirtualWebInquiry::sm_lTotalByteRead = 0;

CVirtualWebInquiry::CVirtualWebInquiry() {
	m_pDataSource = nullptr;

	m_pSession = new CInternetSession(_T("StockAnalysis")); // 此处需要加上调用程序的名称，否则无法运行单元测试程序（原因不明）。
	SetDefaultSessionOption();
	m_pFile = nullptr;
	m_strHeaders = _T("");

	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
	m_strInquiry = _T("");
	m_strInquiryFunction = _T("");
	m_strInquiryToken = _T("");
	m_fReadingWebData = false; // 接收实时数据线程是否执行标识
	m_sBuffer.resize(DefaultWebDataBufferSize_); // 大多数情况下，2M缓存就足够了，无需再次分配内存。

	m_lInquiringNumber = 500; // 每次查询数量默认值为500
	m_tCurrentInquiryTime = 0;

	m_lContentLength = -1;

#ifdef DEBUG
	m_fReportStatus = false;
#else
	m_fReportStatus = false;
#endif
}

CVirtualWebInquiry::~CVirtualWebInquiry(void) {
	m_pDataSource = nullptr;
	if (m_pSession != nullptr) {
		delete m_pSession;
		m_pSession = nullptr;
	}
}

void CVirtualWebInquiry::SetDefaultSessionOption(void) {
	DWORD dwValue = 0;

	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // 设置连接超时时间为120秒
	m_pSession->QueryOption(INTERNET_OPTION_RECEIVE_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // 设置接收超时时间为120秒
	m_pSession->QueryOption(INTERNET_OPTION_SEND_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // 设置发送超时时间为2秒
	m_pSession->QueryOption(INTERNET_OPTION_CONNECT_RETRIES, dwValue); // 查询重连次数
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 5); // 5次重试
	m_pSession->QueryOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, dwValue); // 查询接收超时时间
}

void CVirtualWebInquiry::Reset(void) noexcept {
	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
}

/////////////////////////////////////////////////////////////////////////
//
// 这是此类唯一的接口函数
//
//////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::GetWebData(void) {
	ASSERT(m_pDataSource != nullptr);

	if (!IsReadingWebData()) {
		// 工作线程没有启动？
		if (PrepareNextInquiringString()) {
			SetReadingWebData(true); // 在启动工作线程前就设置，以防由于线程延迟导致重入。
			StartReadingThread();
			return true;
		}
		else return false;
	}
	else return false; // 工作线程已在执行接收数据的任务
}

void CVirtualWebInquiry::PrepareReadingWebData(void) {
	ConfigureSession();
}

void CVirtualWebInquiry::StartReadingThread(void) {
	thread thread1(ThreadReadVirtualWebData, this);
	thread1.detach();
}

UINT ThreadReadVirtualWebData(not_null<CVirtualWebInquiry*> pVirtualWebInquiry) {
	pVirtualWebInquiry->Read();
	return 1;
}

void CVirtualWebInquiry::Read(void) {
	CHighPerformanceCounter counter;

	ASSERT(IsReadingWebData());
	ASSERT(m_pDataSource != nullptr);
	PrepareReadingWebData();
	counter.start();
	if (ReadingWebData()) {
		const auto pWebData = make_shared<CWebData>();
		VerifyDataLength();
		TransferDataToWebData(pWebData); // 将接收到的数据转移至pWebData中。由于使用std::move来加快速度，源数据不能再被使用。
		ResetBuffer();
		ParseData(pWebData);
		UpdateStatusAfterSucceed(pWebData);

		pWebData->SetTime(gl_pChinaMarket->GetUTCTime());
		m_pDataSource->StoreReceivedData(pWebData);
		ASSERT(m_pDataSource->IsInquiring());
	}
	else {
		// error handling
		while (m_pDataSource->GetReceivedDataSize() > 0) m_pDataSource->GetReceivedData();
		m_pDataSource->SetInquiring(false); // 当工作线程出现故障时，直接重置数据申请标志。
	}
	m_pDataSource->SetWebInquiryFinished(true); // 无论成功与否，都要设置此标识
	counter.stop();
	SetCurrentInquiryTime(counter.GetElapsedMilliSecond());

	SetReadingWebData(false);
}

///////////////////////////////////////////////////////////////////////////
//
// 从网络读取数据。每次读1KB，直到读不到为止。
// 当采用此函数读取网易日线历史数据时，OpenFile偶尔会出现超时（网络错误代码12002）错误，可以采用多读取几次解决之。
// 现在发现其他网路读取线程也偶尔出现超时错误，多读几次即可解决之。--20211104
//
// 新浪实时数据服务器打开时间为100毫秒左右，网易实时数据服务器打开时间为350毫秒左右。
//
// 设置连接120秒超时和接收120秒超时、发送2秒超时、重复2次，这样应该能够满足所有网络要求。目前接收数据最长的是申请Tiingo stock，大致为60秒。--20220514
//
///////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::ReadingWebData(void) {
	bool fReadingSuccess = true;

	ASSERT(IsReadingWebData());
	gl_ThreadStatus.IncreaseWebInquiringThread();
	SetByteRead(0);

	ASSERT(m_pFile == nullptr);
	if (OpenFile(GetInquiringString())) {
		try {
			UINT lCurrentByteRead;
			do {
				if (gl_systemStatus.IsExitingSystem()) {
					// 当系统退出时，要立即中断此进程，以防止内存泄露。
					fReadingSuccess = false;
					break;
				}
				lCurrentByteRead = ReadWebFileOneTime(); // 每次读取1K数据。
				IncreaseBufferSizeIfNeeded();
			} while (lCurrentByteRead > 0);
			sm_lTotalByteRead += m_lByteRead;
			// 清除网络错误代码的动作，只在此处进行。以保证只有当顺利读取到网络数据后，方才清除之前的错误标识。
			m_dwWebErrorCode = 0; // 清除错误代码（如果有的话）。只在此处重置该错误代码。
		}
		catch (CInternetException* exception) {//这里一般是使用引用。但我准备在处理完后就删除这个例外了，故而直接使用指针。否则由于系统不处理此例外，会导致程序自动退出。  // NOLINT(misc-throw-by-value-catch-by-reference)
			fReadingSuccess = false;
			m_dwWebErrorCode = exception->m_dwError;
			ReportWebError(m_dwWebErrorCode, m_strInquiry);
			exception->Delete();
		}
		DeleteWebFile();
	}
	else fReadingSuccess = false;

	gl_ThreadStatus.DecreaseWebInquiringThread();
	ASSERT(m_pFile == nullptr);

	return fReadingSuccess;
}

/// <summary>
/// 当采用此函数读取网易日线历史数据时，偶尔会出现超时（网络错误代码12002）错误
/// 目前最大的问题是读取finnhub.io时，由于网站被墙而导致连接错误--20220401
///
/// 调用函数需要处理exception。
///
/// </summary>
/// <param name="strInquiring"></param>
/// <returns></returns>
bool CVirtualWebInquiry::OpenFile(const CString& strInquiring) {
	bool fSucceedOpen = true;
	const long lHeadersLength = m_strHeaders.GetLength();

	const ULONG64 llCurrentTickCount = GetTickCount64();
	ASSERT(m_pSession != nullptr);
	ASSERT(m_pFile == nullptr);
	try {
		// 由于新浪实时数据服务器需要提供头部验证数据，故而OpenURL不再使用默认值，调用者需要各自设置m_strHeaders（默认为空）。
		// 其他的数据尚未需要提供头部验证数据。
		if (gl_systemStatus.IsExitingSystem()) { return false; }
		//CStdioFile* file = m_pSession->OpenURL((LPCTSTR)strInquiring, 1, INTERNET_FLAG_TRANSFER_ASCII, (LPCTSTR)m_strHeaders, lHeadersLength);
		//auto p = typeid(*file).name();
		//ASSERT(std::strcmp(typeid(*file).name(), _T("class CHttpFile")) == 0);
		//m_pFile = dynamic_cast<CHttpFile*>(file);
		m_pFile = static_cast<CHttpFile*>(m_pSession->OpenURL((LPCTSTR)strInquiring, 1, INTERNET_FLAG_TRANSFER_ASCII, (LPCTSTR)m_strHeaders, lHeadersLength));
		ASSERT(std::strcmp(typeid(*m_pFile).name(), _T("class CHttpFile")) == 0);
	}
	catch (CInternetException* exception) { //这里一般是使用引用。但我准备在处理完后就删除这个例外了，故而直接使用指针。否则由于系统不处理此例外，会导致程序自动退出。  // NOLINT(misc-throw-by-value-catch-by-reference)
		ASSERT(m_pFile == nullptr);
		DeleteWebFile();
		m_dwWebErrorCode = exception->m_dwError;
		ReportWebError(m_dwWebErrorCode, GetTickCount64() - llCurrentTickCount, m_strInquiry);
		fSucceedOpen = false;
		exception->Delete();
	}
	if (fSucceedOpen) {
		QueryDataLength();
	}

	return fSucceedOpen;
}

void CVirtualWebInquiry::DeleteWebFile() {
	if (m_pFile != nullptr) {
		m_pFile->Close();
		delete m_pFile;
		m_pFile = nullptr;
	}
}

long CVirtualWebInquiry::QueryDataLength() {
	CString str;
	m_pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, str);
	if (str.GetLength() > 0) {
		char* p;
		m_lContentLength = strtol(str.GetBuffer(), &p, 10);
	}
	return m_lContentLength;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// 每次读取1K数据，然后将读取到的数据存入缓冲区
// 此函数的耗时，皆在Read函数，故而无法加快执行速度了。
//
////////////////////////////////////////////////////////////////////////////////////////////
UINT CVirtualWebInquiry::ReadWebFileOneTime(void) {
	char buffer[1024 * 16];
	const UINT uByteRead = m_pFile->Read(buffer, 1024 * 16);
	for (UINT i = 0; i < uByteRead; i++) {
		m_sBuffer.at(m_lByteRead++) = buffer[i];
	}

	return uByteRead;
}

bool CVirtualWebInquiry::IncreaseBufferSizeIfNeeded(long lIncreaseSize) {
	if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) {
		// 数据可存储空间不到128K时
		m_sBuffer.resize(m_sBuffer.size() + lIncreaseSize); // 扩大lSize（默认为1M）数据范围
	}
	return true;
}

bool CVirtualWebInquiry::VerifyDataLength() const {
	const auto byteRead = GetByteRead();

	if (m_lContentLength > 0) {
		if (m_lContentLength != byteRead) {
			CString str = _T("网络数据长度不符。预期长度：");
			char buffer[100];
			sprintf_s(buffer, _T("%d"), m_lContentLength);
			str += buffer;
			str += _T("，实际长度：");
			sprintf_s(buffer, _T("%d"), byteRead);
			str += buffer;
			str += m_strInquiry.Left(120);
			gl_systemMessage.PushErrorMessage(str);
			str = m_sBuffer.c_str();
			str = str.Left(200);
			gl_systemMessage.PushErrorMessage(str);
		}
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 使用libcurl的curl_easy_perform()读取网络数据的模式。
///	与Windows自带的CHttpFile读取模式相比，没有任何速度上的提高，且降低了不少。
///	不使用此模式，保留在此仅为对照。
///
/////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualWebInquiry::Read2(void) {
	ASSERT(0); // 此模式速度降低了不少，不使用此模式。保留的原因是用于对比。
	ASSERT(IsReadingWebData());
	ASSERT(m_pDataSource != nullptr);
	m_pWebData = make_shared<CWebData>();
	PrepareReadingWebData();
	if (ReadingWebData2()) {
		ResetBuffer();
		ParseData(m_pWebData);
		UpdateStatusAfterSucceed(m_pWebData);

		m_pWebData->SetTime(gl_pChinaMarket->GetUTCTime());
		m_pDataSource->StoreReceivedData(m_pWebData);
		ASSERT(m_pDataSource->IsInquiring());
	}
	else {
		// error handling
		while (m_pDataSource->GetReceivedDataSize() > 0) m_pDataSource->GetReceivedData();
		m_pDataSource->SetInquiring(false); // 当工作线程出现故障时，直接重置数据申请标志。
	}
	m_pDataSource->SetWebInquiryFinished(true); // 无论成功与否，都要设置此标识

	SetReadingWebData(false);
}

static size_t receive_data_func(void* ptr, size_t size, size_t nmemb, void* userOp) {
	const size_t realSize = size * nmemb;

	(static_cast<CWebData*>(userOp))->StoreData(static_cast<char*>(ptr), realSize);

	return realSize;
}

bool CVirtualWebInquiry::ReadingWebData2(void) {
	gl_ThreadStatus.IncreaseWebInquiringThread();
	m_pWebData = make_shared<CWebData>();

	CURL* curl_handle = curl_easy_init();
	CURLcode code;
	if (curl_handle) {
		curl_easy_setopt(curl_handle, CURLOPT_URL, m_strInquiry);
		if (m_strHeaders.GetLength() > 0) {
			struct curl_slist* list = nullptr;
			list = curl_slist_append(list, m_strHeaders);
			curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, list); // 提交报头
			curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1); // 关闭进度条
			//curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 60); // TimeOut
			//curl_easy_setopt(curl_handle, CURLOPT_MAX_RECV_SPEED_LARGE, static_cast<curl_off_t>(2 * 512 * 1024)); // 最大下载速度为2M Bytes/S
			code = curl_easy_setopt(curl_handle, CURLOPT_BUFFERSIZE, 1024 * 512);
		}
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0); // 不校验SSL
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, m_pWebData.get());
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, receive_data_func);
	}
	CURLcode res = curl_easy_perform(curl_handle);
	double d = 0.0;
	curl_easy_getinfo(curl_handle, CURLINFO_TOTAL_TIME, &d);
	m_tCurrentInquiryTime = d * 1000;
	curl_easy_cleanup(curl_handle);
	m_pWebData->Resize(m_pWebData->GetCurrentPos());
	sm_lTotalByteRead += m_pWebData->GetCurrentPos();
	m_pWebData->SetCurrentPos(0);
	// 清除网络错误代码的动作，只在此处进行。以保证只有当顺利读取到网络数据后，方才清除之前的错误标识。
	m_dwWebErrorCode = 0; // 清除错误代码（如果有的话）。只在此处重置该错误代码。

	gl_ThreadStatus.DecreaseWebInquiringThread();

	return true;
}

bool CVirtualWebInquiry::TransferDataToWebData(CWebDataPtr pWebData) {
	m_sBuffer.resize(m_lByteRead);
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // 使用std::move以加速执行速度
	pWebData->SetBufferLength(m_lByteRead);
	pWebData->ResetCurrentPos();

	return true;
}

bool CVirtualWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个实时数据\n", lNumberOfData);
	return true;
}

void CVirtualWebInquiry::CreateTotalInquiringString(CString strMiddle) { m_strInquiry = m_strInquiryFunction + strMiddle + m_strInquiryToken; }

void CVirtualWebInquiry::TESTSetBuffer(char* buffer, INT64 lTotalNumber) {
	m_sBuffer.resize(lTotalNumber);
	for (INT64 i = 0; i < lTotalNumber; i++) { m_sBuffer.at(i) = buffer[i]; }
	m_lByteRead = lTotalNumber;
}

void CVirtualWebInquiry::TESTSetBuffer(CString str) {
	const INT64 lTotalNumber = str.GetLength();
	const char* buffer = str.GetBuffer();

	m_sBuffer.resize(lTotalNumber);
	for (INT64 i = 0; i < lTotalNumber; i++) { m_sBuffer.at(i) = buffer[i]; }
	m_lByteRead = lTotalNumber;
}
