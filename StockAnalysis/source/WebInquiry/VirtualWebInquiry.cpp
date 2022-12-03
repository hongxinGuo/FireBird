#include"pch.h"

#include "VirtualWebInquiry.h"
#include"ThreadStatus.h"
#include"Thread.h"

#include"InfoReport.h"

#include"ChinaMarket.h"

#include"VirtualWebInquiry.h"
#include"HighPerformanceCounter.h"

atomic_llong CVirtualWebInquiry::m_lTotalByteReaded = 0;

CVirtualWebInquiry::CVirtualWebInquiry() : CObject() {
	m_pDataSource = nullptr;

	m_pSession = new CInternetSession(_T("StockAnalysis")); // 此处需要加上调用程序的名称，否则无法运行单元测试程序（原因不明）。
	SetDefaultSessionOption();
	m_pFile = nullptr;
	m_strHeaders = _T("");

	m_lByteRead = 0;
	m_fWebError = false;
	m_dwWebErrorCode = 0;
	m_strInquiry = _T("");
	m_strInquiryFunction = _T("");
	m_strInquiryToken = _T("");
	m_fReadingWebData = false; // 接收实时数据线程是否执行标识
	m_sBuffer.resize(__DefaultWebDataBufferSize__); // 大多数情况下，1M缓存就足够了，无需再次分配内存。

	m_lInquiringNumber = 500; // 每次查询数量默认值为500
	m_tCurrentInquiryTime = 0;

	m_fFSonContentType = false;

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
	m_fWebError = false;
}

/////////////////////////////////////////////////////////////////////////
//
// 这是此类唯一的接口函数
//
//////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::GetWebData(void) {
	ASSERT(m_pDataSource != nullptr);

	if (!IsReadingWebData()) { // 工作线程没有启动？
		if (PrepareNextInquiringString()) {
			SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
			StartReadingThread();
			return true;
		}
		else return false;
	}
	else return false; // 工作线程已在执行接收数据的任务
}

void CVirtualWebInquiry::PrepareReadingWebData(void) {
	ConfigerateSession();
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
	ULONG64 llCurrentTickCount = GetTickCount64();

	ASSERT(IsReadingWebData());
	ASSERT(m_pDataSource != nullptr);
	PrepareReadingWebData();
	if (ReadingWebData()) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		VerifyDataLength();
		TransferDataToWebData(pWebData); // 将接收到的数据转移至pWebData中。由于使用std::move来加快速度，源数据不能再被使用。
		ResetBuffer();
		ParseData(pWebData);
		UpdateStatusWhenSecceed(pWebData);

		pWebData->SetTime(gl_pChinaMarket->GetUTCTime());
		m_pDataSource->StoreReceivedData(pWebData);
		m_pDataSource->SetDataReceived(true);
	}
	else { // error handling
		while (m_pDataSource->GetReceivedDataSize() > 0) m_pDataSource->GetReceivedData();
		m_pDataSource->SetInquiring(false); // 当工作线程出现故障时，直接重置数据申请标志。
	}
	SetCurrentInquiryTime(GetTickCount64() - llCurrentTickCount); // 这种是使用GetTickCount()函数版本，应该占用的时间少。

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
	long lCurrentByteReaded = 0;

	ASSERT(IsReadingWebData());
	gl_ThreadStatus.IncreaseWebInquiringThread();
	SetByteReaded(0);

	ASSERT(m_pFile == nullptr);
	if (OpenFile(GetInquiringString())) {
		try {
			do {
				if (gl_systemStatus.IsExitingSystem()) { // 当系统退出时，要立即中断此进程，以防止内存泄露。
					fReadingSuccess = false;
					break;
				}
				lCurrentByteReaded = ReadWebFileOneTime(); // 每次读取1K数据。
				IncreaseBufferSizeIfNeeded();
			} while (lCurrentByteReaded > 0);
			m_lTotalByteReaded += m_lByteRead;
			// 清除网络错误代码的动作，只在此处进行。以保证只有当顺利读取到网络数据后，方才清除之前的错误标识。
			ClearWebError();// 清除错误代码（如果有的话）
		}
		catch (CInternetException* exception) {
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
bool CVirtualWebInquiry::OpenFile(CString strInquiring) {
	bool fSucceedOpen = true;
	long lHeadersLength = m_strHeaders.GetLength();

	ULONG64 llCurrentTickCount = GetTickCount64();
	ASSERT(m_pSession != nullptr);
	ASSERT(m_pFile == nullptr);
	try {
		// 由于新浪实时数据服务器需要提供头部验证数据，故而OpenURL不再使用默认值，调用者需要各自设置m_strHeaders（默认为空）。
		// 其他的数据尚未需要提供头部验证数据。
		if (gl_systemStatus.IsExitingSystem()) {
			fSucceedOpen = false;
		}
		else {
			m_pFile = static_cast<CHttpFile*>(m_pSession->OpenURL((LPCTSTR)strInquiring, 1, INTERNET_FLAG_TRANSFER_ASCII, (LPCTSTR)m_strHeaders, lHeadersLength));
		}
	}
	catch (CInternetException* exception) {
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
		m_lContentLength = atol(str.GetBuffer());
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
	char buffer[1024];
	const UINT uByteRead = m_pFile->Read(buffer, 1024);
	for (int i = 0; i < uByteRead; i++) {
		m_sBuffer.at(m_lByteRead++) = buffer[i];
	}

	return uByteRead;
}

bool CVirtualWebInquiry::IncreaseBufferSizeIfNeeded(long lSize) {
	if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) { // 数据可存储空间不到128K时
		m_sBuffer.resize(m_sBuffer.size() + lSize); // 扩大lSize（默认为1M）数据范围
	}
	return true;
}

bool CVirtualWebInquiry::VerifyDataLength() {
	auto byteReaded = GetByteReaded();
	char buffer[100];
	CString str = _T("网络数据长度不符。预期长度：");

	if (m_lContentLength > 0) {
		if (m_lContentLength != byteReaded) {
			sprintf_s(buffer, _T("%d"), m_lContentLength);
			str += buffer;
			str += _T("，实际长度：");
			sprintf_s(buffer, _T("%I64d"), byteReaded);
			str += buffer;
			str += m_strInquiry.Left(120);
			gl_systemMessage.PushErrorMessage(str);
		}
		return false;
	}
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

void CVirtualWebInquiry::CreateTotalInquiringString(CString strMiddle) {
	m_strInquiry = m_strInquiryFunction + strMiddle + m_strInquiryToken;
}

void CVirtualWebInquiry::UpdateStatusWhenSecceed(CWebDataPtr pData) {
	m_pDataSource->SetDataReceived(true);
}

void CVirtualWebInquiry::__TESTSetBuffer(char* buffer, INT64 lTotalNumber) {
	m_sBuffer.resize(lTotalNumber);
	for (INT64 i = 0; i < lTotalNumber; i++) {
		m_sBuffer.at(i) = buffer[i];
	}
	m_lByteRead = lTotalNumber;
}

void CVirtualWebInquiry::__TESTSetBuffer(CString str) {
	INT64 lTotalNumber = str.GetLength();
	char* buffer = str.GetBuffer();

	m_sBuffer.resize(lTotalNumber);
	for (INT64 i = 0; i < lTotalNumber; i++) {
		m_sBuffer.at(i) = buffer[i];
	}
	m_lByteRead = lTotalNumber;
}