#include "pch.h"

#include "VirtualDataSource.h"
#include"Thread.h"
#include"ThreadStatus.h"

#include<thread>

#include "ChinaMarket.h"
#include "InfoReport.h"
using std::thread;

atomic_long CVirtualDataSource::sm_lTotalByteRead = 0;

CVirtualDataSource::CVirtualDataSource(void) {
	m_pCurrentProduct = nullptr;
	m_fEnable = true; // 默认为允许执行

	m_pSession = make_shared<CInternetSession>(_T("FireBird")); // 此处需要加上调用程序的名称，否则无法运行单元测试程序（原因不明）。
	SetDefaultSessionOption();
	m_pFile = nullptr;
	m_strHeaders = _T("");

	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
	m_strInquiry = _T("");
	m_strInquiryFunction = _T("");
	m_strSuffix = _T("");
	m_strInquiryToken = _T("");
	m_fInquiringWebData = false; // 接收实时数据线程是否执行标识
	m_sBuffer.resize(DefaultWebDataBufferSize_); // 大多数情况下，2M缓存就足够了，无需再次分配内存。

	m_lInquiringNumber = 500; // 每次查询数量默认值为500
	m_tCurrentInquiryTime = 0;
	m_pCurrentProduct = nullptr;

	m_lContentLength = -1;

	CVirtualDataSource::Reset();
}

bool CVirtualDataSource::Reset(void) {
	m_fInquiring = false;

	m_lByteRead = 0;
	m_dwWebErrorCode = 0;

	return true;
}

void CVirtualDataSource::Run(const long lCurrentTime) {
	ASSERT(m_fEnable);
	if (!HaveInquiry()) { // 目前允许一次申请生成多个查询，故而有可能需要多次查询后方允许再次申请。
		GenerateInquiryMessage(lCurrentTime);
	}
	else {
		SetInquiring(true); // 队列中还有待查询的申请，设置此标识，继续查询数据。
	}
	if (ProcessWebDataReceived()) {	// 先处理接收到的网络数据
		UpdateStatus();
	}
	GetWebData(); // 然后再申请下一个网络数据
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// DataSource读取函数采用申请和接收轮换执行方式，故而至少调用两次才完成一个轮回。
//
/////////////////////////////////////////////////////////////////////////////////////////////
bool CVirtualDataSource::GetWebData(void) {
	if (HaveInquiry()) {
		if (!IsInquiringWebData()) {
			GetCurrentProduct();
			CreateInquiryMessageFromCurrentProduct();
			ProcessInquiryMessage();
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////
//
// 处理工作线程接收到的网络信息。
//
// 只允许同时处理一条信息。即信息从申请至处理完之前，不允许处理下一条信息。这样能够保证同一性。且由于Finnhub网站有速度限制，
// 每分钟只允许60次申请，故而没有必要强调处理速度。
//
// 目前已将中国市场的SinaRT、NeteaseRT、TengxunRT和NeteaseDayLine的数据使用此数据源实现了。由于SinaRT和NeteaseRT要求的实时性比较强，
// 故而该两个数据源采用先返回后处理数据的模式。--20221120
//
//////////////////////////////////////////////
bool CVirtualDataSource::ProcessWebDataReceived(void) {
	if (HaveReceivedData()) {
		// 处理当前网络数据
		ASSERT(m_pCurrentProduct != nullptr);
		CWebDataPtr pWebData = GetReceivedData();
		if (pWebData->IsParsed()) {
			m_pCurrentProduct->CheckNoRightToAccess(pWebData);
			if (m_pCurrentProduct->IsNoRightToAccess()) {
				// 如果系统报告无权查询此类数据
				// 目前先在软件系统消息中报告
				gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
				m_pCurrentProduct->AddInaccessibleExchangeIfNeeded(); // 检查是否无权查询
			}
		}
		ASSERT(IsInquiring()); // 执行到此时，尚不允许申请下次的数据。
		if (!HaveInquiry()) { // 没有现存的申请时
			SetInquiring(false); // 此标识的重置需要位于位于最后一步
		}
		// 有些网络数据比较大，处理需要的时间超长（如美国市场的股票代码有5M，处理时间为。。。）， 故而需要将ProductWebData的函数ParseAndStoreWebData()线程化。
		// 本线程必须位于本函数的最后，因其调用SetInquiry(false)后，启动了下次申请，故而能防止发生重入问题。
		thread thread1(ThreadWebSourceParseAndStoreWebData, this, m_pCurrentProduct, pWebData);
		thread1.detach();
		return true;
	}
	return false;
}

UINT ThreadWebSourceParseAndStoreWebData(CVirtualDataSource* pDataSource, CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) {
	gl_WebSourceParseAndStoreData.acquire();
	gl_ThreadStatus.IncreaseBackGroundWorkingThread();
	pDataSource->ParseAndStoreData(pProductWebData, pWebData);
	gl_ThreadStatus.DecreaseBackGroundWorkingThread();
	gl_WebSourceParseAndStoreData.release();

	return 203;
}

/// <summary>
///
/// </summary>
/// <param name="pProductWebData"></param>
/// <param name="pWebData"></param>
void CVirtualDataSource::ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData) {
	pProductWebData->ParseAndStoreWebData(pWebData);
}

void CVirtualDataSource::SetDefaultSessionOption(void) {
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

void CVirtualDataSource::CreateInquiryMessageFromCurrentProduct(void) {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiryFunction = m_pCurrentProduct->CreateMessage();
	CreateTotalInquiringString();
}

/////////////////////////////////////////////////////////////////////////
//
// 这是此类唯一的接口函数
//
//////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::ProcessInquiryMessage(void) {
	ASSERT(!IsInquiringWebData());
	SetInquiringWebData(true); // 在启动工作线程前就设置，以防由于线程延迟导致重入。
	StartReadingThread();
}

void CVirtualDataSource::PrepareReadingWebData(void) {
	ConfigureSession();
}

void CVirtualDataSource::StartReadingThread(void) {
	thread thread1(ThreadReadVirtualWebData, this);
	thread1.detach();
}

UINT ThreadReadVirtualWebData(not_null<CVirtualDataSource*> pVirtualDataSource) {
	pVirtualDataSource->Read();
	return 1;
}

void CVirtualDataSource::Read(void) {
	CHighPerformanceCounter counter;

	ASSERT(IsInquiringWebData());
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
		StoreReceivedData(pWebData);
		ASSERT(IsInquiring());
	}
	else {
		// error handling
		DiscardProduct(); // 当一次查询产生多次申请时，这些申请都是各自相关的，只有出现一次错误，其他的申请就无意义了。
		DiscardReceivedData();
		SetInquiring(false); // 当工作线程出现故障时，直接重置数据申请标志。
	}
	counter.stop();
	SetCurrentInquiryTime(counter.GetElapsedMilliSecond());

	SetInquiringWebData(false);
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
bool CVirtualDataSource::ReadingWebData(void) {
	ASSERT(IsInquiringWebData());
	gl_ThreadStatus.IncreaseWebInquiringThread();
	SetByteRead(0);

	ASSERT(m_pFile == nullptr);
	try {
		OpenFile(GetInquiringString());
		GetFileHeaderInformation();
		UINT lCurrentByteRead;
		do {
			if (gl_systemStatus.IsExitingSystem()) {// 当系统退出时，要立即中断此进程，以防止内存泄露。
				m_dwWebErrorCode = 0;
				break;
			}
			lCurrentByteRead = ReadWebFileOneTime(); // 每次读取1K数据。
			XferReadingToBuffer(m_lByteRead, lCurrentByteRead);
			m_lByteRead += lCurrentByteRead;
			IncreaseBufferSizeIfNeeded();
		}
		while (lCurrentByteRead > 0);
		sm_lTotalByteRead += m_lByteRead;
		// 清除网络错误代码的动作，只在此处进行。以保证只有当顺利读取到网络数据后，方才清除之前的错误标识。
		m_dwWebErrorCode = 0; // 清除错误代码（如果有的话）。只在此处重置该错误代码。
	}
	catch (CInternetException* exception) {//这里一般是使用引用。但我准备在处理完后就删除这个例外，故而直接使用指针。否则由于系统不处理此例外，会导致程序自动退出。
		m_dwWebErrorCode = exception->m_dwError;
		ReportWebError(m_dwWebErrorCode, m_strInquiry);
		exception->Delete();
	}
	DeleteWebFile();
	gl_ThreadStatus.DecreaseWebInquiringThread();

	return !IsWebError();
}

/// <summary>
/// 当采用此函数读取网易日线历史数据时，偶尔会出现超时（网络错误代码12002）错误
/// 目前最大的问题是读取finnhub.io时，由于网站被墙而导致连接错误--20220401
///
/// 由于新浪实时数据服务器需要提供头部验证数据，故而OpenURL不再使用默认值，调用者需要各自设置m_strHeaders（默认为空）。
/// 其他的数据尚未需要提供头部验证数据。
/// 调用函数需要处理exception。
///
/////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::OpenFile(const CString& strInquiring) {
	m_pFile = dynamic_cast<CHttpFile*>(m_pSession->OpenURL(strInquiring, 1,
	                                                       INTERNET_FLAG_TRANSFER_ASCII,
	                                                       m_strHeaders, m_strHeaders.GetLength()));
}

void CVirtualDataSource::GetFileHeaderInformation() {
	ASSERT(m_pFile != nullptr);
	m_pFile->QueryInfoStatusCode(m_dwHTTPStatusCode);
	QueryDataLength();
}

void CVirtualDataSource::DeleteWebFile() {
	if (m_pFile != nullptr) {
		m_pFile->Close();
		delete m_pFile;
		m_pFile = nullptr;
	}
}

long CVirtualDataSource::QueryDataLength() {
	CString str;
	m_pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, str);
	if (str.GetLength() > 0) {
		char* p;
		m_lContentLength = strtol(str.GetBuffer(), &p, 10);
	}
	else {
		m_lContentLength = 0;
	}
	return m_lContentLength;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// 每次读取1K数据，然后将读取到的数据存入缓冲区
// 此函数的耗时，皆在Read函数，故而无法加快执行速度了。
//
////////////////////////////////////////////////////////////////////////////////////////////
UINT CVirtualDataSource::ReadWebFileOneTime(void) {
	const UINT uByteRead = m_pFile->Read(m_dataBuffer, DATA_BUFFER_SIZE_);
	return uByteRead;
}

//
// 使用逐字节拷贝，16KB耗时11微秒（release），使用memcpy函数完成，耗时120纳秒。
//
void CVirtualDataSource::XferReadingToBuffer(long lPosition, UINT uByteRead) {
	/*
	for (UINT i = 0; i < uByteRead; i++) {
		m_sBuffer.at(lPosition++) = m_dataBuffer[i];
	}
	*/
	char* p = &m_sBuffer.at(lPosition);
	memcpy(p, m_dataBuffer, uByteRead);
}

bool CVirtualDataSource::IncreaseBufferSizeIfNeeded(long lIncreaseSize) {
	if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) {
		// 数据可存储空间不到128K时
		m_sBuffer.resize(m_sBuffer.size() + lIncreaseSize); // 扩大lSize（默认为1M）数据范围
	}
	return true;
}

void CVirtualDataSource::VerifyDataLength() const {
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
			str += m_strInquiry.Left(200);
			gl_systemMessage.PushErrorMessage(str);
			sprintf_s(buffer, _T("%d"), m_dwHTTPStatusCode);
			str = _T("Status code : ");
			str += buffer;
			str += m_sBuffer.c_str();
			str = str.Left(400);
			gl_systemMessage.PushErrorMessage(str);
		}
	}
}

bool CVirtualDataSource::TransferDataToWebData(CWebDataPtr pWebData) {
	m_sBuffer.resize(m_lByteRead);
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // 使用std::move以加速执行速度
	pWebData->SetBufferLength(m_lByteRead);
	pWebData->ResetCurrentPos();

	return true;
}

void CVirtualDataSource::CreateTotalInquiringString() {
	m_strInquiry = m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken;
}

void CVirtualDataSource::TESTSetBuffer(char* buffer, INT64 lTotalNumber) {
	m_sBuffer.resize(lTotalNumber);
	for (INT64 i = 0; i < lTotalNumber; i++) { m_sBuffer.at(i) = buffer[i]; }
	m_lByteRead = lTotalNumber;
}

void CVirtualDataSource::TESTSetBuffer(CString str) {
	const INT64 lTotalNumber = str.GetLength();
	const char* buffer = str.GetBuffer();

	m_sBuffer.resize(lTotalNumber);
	for (INT64 i = 0; i < lTotalNumber; i++) { m_sBuffer.at(i) = buffer[i]; }
	m_lByteRead = lTotalNumber;
}

void CVirtualDataSource::TESTSetWebBuffer(char* buffer, INT64 lTotalNumber) {
	for (INT64 i = 0; i < lTotalNumber; i++) { m_dataBuffer[i] = buffer[i]; }
}
