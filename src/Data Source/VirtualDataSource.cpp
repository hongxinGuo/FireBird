#include "pch.h"

#include "VirtualDataSource.h"
#include"Thread.h"
#include"ThreadStatus.h"

#include<thread>

#include "ChinaMarket.h"
#include "InfoReport.h"
#include "TimeConvert.h"
using std::thread;

atomic_long CVirtualDataSource::sm_lTotalByteRead = 0;

CVirtualDataSource::CVirtualDataSource() {
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
	m_sBuffer.resize(DefaultWebDataBufferSize_); // 大多数情况下，2M缓存就足够了，无需再次分配内存。

	m_lInquiringNumber = 500; // 每次查询数量默认值为500
	m_tCurrentInquiryTime = 0;
	m_pCurrentProduct = nullptr;

	m_lContentLength = -1;
	m_fInquiring = false;
	m_bIsGetWebDataAndProcessItThreadRunning = false;

	m_llLastTimeTickCount = 0;
	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
}

/// <summary>
/// DataSource的顶层函数。
/// 当申请信息为空时，生成当前查询字符串。
/// 当存在申请信息且没有正在运行的查询线程时，生成查询线程。
/// </summary>
/// <param name="lCurrentTime"：当前市场时间></param>
void CVirtualDataSource::Run(const long lCurrentTime) {
	ASSERT(m_fEnable);

	if (!IsInquiring()) {
		ASSERT(!HaveInquiry());
		GenerateInquiryMessage(lCurrentTime);
	}

	if (HaveInquiry() && !IsGetWebDataAndProcessItThreadRunning()) {
		ASSERT(IsInquiring());
		GetCurrentProduct();
		GenerateCurrentInquiryMessage();
		SetGetWebDataAndProcessItThreadRunning(true); // 在调用工作线程前即设置该值
		CreateThreadGetWebDataAndProcessIt();
	}
}

UINT ThreadGetWebDataAndProcessIt(CVirtualDataSource* pDataSource) {
	gl_ThreadStatus.IncreaseWebInquiringThread();
	pDataSource->GetWebDataAndProcessIt();
	gl_ThreadStatus.DecreaseWebInquiringThread();
	return 205;
}

void CVirtualDataSource::CreateThreadGetWebDataAndProcessIt() {
	thread thread1(ThreadGetWebDataAndProcessIt, this);
	thread1.detach();
}

bool CVirtualDataSource::GetWebDataAndProcessIt() {
	CHighPerformanceCounter counter;
	bool bSucceed = false;

	ASSERT(IsGetWebDataAndProcessItThreadRunning());// 在调用工作线程前即设置
	counter.start();
	if (GetWebData()) {
		if (ProcessWebDataReceived()) {
			UpdateStatus();
			bSucceed = true;
		}
	}
	counter.stop();
	SetCurrentInquiryTime(counter.GetElapsedMillisecond());
	SetGetWebDataAndProcessItThreadRunning(false);
	return bSucceed;
}

//////////////////////////////////////////////
//
//
// 只允许同时处理一条信息。即信息从申请至处理完之前，不允许处理下一条信息。这样能够保证同一性。且由于Finnhub网站有速度限制，
// 每分钟只允许60次申请，故而没有必要强调处理速度。
//
//
//////////////////////////////////////////////
bool CVirtualDataSource::ProcessWebDataReceived() {
	bool bProcessed = false;
	//ASSERT(HaveReceivedData());
	if (HaveReceivedData()) {
		// 处理当前网络数据
		ASSERT(m_pCurrentProduct != nullptr);
		const CWebDataPtr pWebData = GetReceivedData();
		CheckInaccessible(pWebData);
		m_pCurrentProduct->ParseAndStoreWebData(pWebData);
		bProcessed = true;
	}
	ASSERT(IsInquiring()); // 执行到此时，尚不允许申请下次的数据。
	if (!HaveInquiry()) { // 没有现存的申请时
		SetInquiring(false); // 此标识的重置需要位于位于最后一步
	}
	return bProcessed;
}

void CVirtualDataSource::CheckInaccessible(const CWebDataPtr& pWebData) const {
	ASSERT(m_pCurrentProduct != nullptr);
	if (m_pCurrentProduct->CheckInaccessible(pWebData)) {
		// 如果系统报告无权查询此类数据, 目前先在软件系统消息中报告
		gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
	}
}

void CVirtualDataSource::SetDefaultSessionOption() const {
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

void CVirtualDataSource::GenerateCurrentInquiryMessage() {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiryFunction = m_pCurrentProduct->CreateMessage();
	CreateTotalInquiringString();
}

bool CVirtualDataSource::GetWebData() {
	return Read();
}

void CVirtualDataSource::PrepareReadingWebData() {
	ConfigureSession();
}

bool CVirtualDataSource::Read() {
	bool bSucceed = true;
	PrepareReadingWebData();
	ReadWebData();
	if (!IsWebError()) {
		VerifyDataLength();
		const auto pWebData = CreateWebDataAfterSucceedReading();
		StoreReceivedData(pWebData);
		ResetBuffer();
	}
	else {
		// error handling
		bSucceed = false;
		DiscardAllInquiry(); // 当一次查询产生多次申请时，这些申请都是各自相关的，只要出现一次错误，其他的申请就无意义了。
		DiscardReceivedData();
		ASSERT(IsInquiring());
		SetInquiring(false); // 当工作线程出现故障时，直接重置数据申请标志。
	}

	return bSucceed;
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
void CVirtualDataSource::ReadWebData() {
	SetByteRead(0);

	ASSERT(m_pFile == nullptr);
	try {
		OpenFile(GetInquiringString());
		GetFileHeaderInformation();
		UINT lCurrentByteRead;
		do {
			if (gl_systemStatus.IsExitingSystem()) {// 当系统退出时，要立即中断此进程，以防止内存泄露。
				break;
			}
			lCurrentByteRead = ReadWebFileOneTime(); // 每次读取1K数据。
			XferReadingToBuffer(m_lByteRead, lCurrentByteRead);
			m_lByteRead += lCurrentByteRead;
			IncreaseBufferSizeIfNeeded();
		} while (lCurrentByteRead > 0);
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
}

// <summary>
// 当采用此函数读取网易日线历史数据时，偶尔会出现超时（网络错误代码12002）错误
// 目前最大的问题是读取finnhub.io时，由于网站被墙而导致连接错误--20220401
//
// 由于新浪实时数据服务器需要提供头部验证数据，故而OpenURL不再使用默认值，调用者需要各自设置m_strHeaders（默认为空）。
// 其他的数据尚未需要提供头部验证数据。
// 调用函数需要处理exception。
//
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
// 每次读取16K数据，然后将读取到的数据存入缓冲区
// 此函数的耗时，皆在Read函数，故而无法加快执行速度了。
//
////////////////////////////////////////////////////////////////////////////////////////////
UINT CVirtualDataSource::ReadWebFileOneTime() {
	const UINT uByteRead = m_pFile->Read(m_dataBuffer, DATA_BUFFER_SIZE_);
	return uByteRead;
}

//
// release编译模式下：使用逐字节拷贝，16KB耗时11微秒，使用memcpy函数完成，耗时120纳秒。
//
void CVirtualDataSource::XferReadingToBuffer(long lPosition, UINT uByteRead) {
	ASSERT((lPosition + uByteRead) < m_sBuffer.size());
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

CWebDataPtr CVirtualDataSource::CreateWebDataAfterSucceedReading() {
	const auto pWebData = make_shared<CWebData>();
	pWebData->ResetCurrentPos();
	pWebData->SetTime(GetUTCTime());
	TransferDataToWebData(pWebData); // 将接收到的数据转移至pWebData中。由于使用std::move来加快速度，源数据不能再被使用。
	ParseData(pWebData);
	UpdateStatusAfterReading(pWebData);

	return pWebData;
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

	return true;
}

void CVirtualDataSource::CreateTotalInquiringString() {
	m_strInquiry = m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken;
}

void CVirtualDataSource::TESTSetBuffer(const char* buffer, const INT64 lTotalNumber) {
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

void CVirtualDataSource::TESTSetWebBuffer(const char* buffer, const INT64 lTotalNumber) {
	for (INT64 i = 0; i < lTotalNumber; i++) { m_dataBuffer[i] = buffer[i]; }
}
