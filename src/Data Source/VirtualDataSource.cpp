#include "pch.h"

#include "VirtualDataSource.h"

#include <simdjson.h>

#include"Thread.h"
#include"ThreadStatus.h"

#include"HighPerformanceCounter.h"
#include "InfoReport.h"

using std::thread;

atomic_long CVirtualDataSource::sm_lTotalByteRead = 0;
atomic_long CVirtualDataSource::sm_lTotalByteReadPerSecond = 0;

CVirtualDataSource::CVirtualDataSource() {
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

	m_lInquiringNumber = 500; // 每次查询数量默认值为500
	m_tCurrentInquiryTime = 0;
	m_pCurrentProduct = nullptr;

	m_lContentLength = 0;
	m_fInquiring = false;
	m_bIsWorkingThreadRunning = false;

	m_llLastTimeTickCount = 0;
	m_dwWebErrorCode = 0;
}

///////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// DataSource的顶层函数。
/// 当申请信息为空时，生成当前查询字符串。
/// 当存在申请信息且没有正在运行的查询线程时，生成查询线程。
///
/// Note 调用函数不能使用thread_pool_executor或者background_executor，只能使用thread_executor，否则thread_pool_executor所生成线程的返回值无法读取，原因待查。
///
/// </summary>
///
/// 必须使用独立的thread_executor任务序列，不能使用thread_pool_executor或者background_executor，
//  否则解析工作使用的thread_pool_executor会与之产生冲突，导致产生同步问题。
///
/// lCurrentLocalMarketTime：当前市场时间
///
////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::Run(const long lCurrentLocalMarketTime) {
	if (!IsInquiring()) {
		ASSERT(!HaveInquiry());
		GenerateInquiryMessage(lCurrentLocalMarketTime);
	}

	if (HaveInquiry() && !IsWorkingThreadRunning()) {
		ASSERT(gl_systemConfiguration.IsWorkingMode()); // 不允许测试
		ASSERT(IsInquiring());
		GetCurrentProduct();
		GenerateCurrentInquiryMessage();
		SetWorkingThreadRunning(true); // 在调用工作线程前即设置该值
		gl_runtime.thread_executor()->post([this] { //Note 必须使用独立的thread_executor任务序列，不能使用thread_pool_executor或者background_executor
				gl_ThreadStatus.IncreaseWebInquiringThread();
				this->GetWebDataAndProcessIt();
				gl_ThreadStatus.DecreaseWebInquiringThread();
			});
	}
}

void CVirtualDataSource::GetWebDataAndProcessIt() {
	CHighPerformanceCounter counter;

	ASSERT(IsWorkingThreadRunning());// 在调用工作线程前即设置
	counter.start();
	GetWebData();
	if (!IsWebError()) {
		if (IsEnable()) ProcessWebDataReceived(); // 只有当本服务器正在使用时，才处理接收到的网络数据
		else DiscardReceivedData(); // 否则抛弃掉
	}
	counter.stop();
	SetWorkingThreadRunning(false);
	SetCurrentInquiryTime(counter.GetElapsedMillisecond());
}

//////////////////////////////////////////////
//
//
// 只允许同时处理一条信息。即信息从申请至处理完之前，不允许处理下一条信息。这样能够保证同一性。且由于Finnhub网站有速度限制，
// 每分钟只允许60次申请，故而没有必要强调处理速度。
//
//
//////////////////////////////////////////////
void CVirtualDataSource::ProcessWebDataReceived() {
	if (HaveReceivedData()) {// 处理当前网络数据
		ASSERT(m_pCurrentProduct != nullptr);
		const CWebDataPtr pWebData = GetReceivedData();
		CheckInaccessible(pWebData);
		m_pCurrentProduct->ParseAndStoreWebData(pWebData);
		m_pCurrentProduct->UpdateDataSourceStatus(this->GetShared()); // 这里传递的是实际DataSource智能指针
	}
	ASSERT(IsInquiring()); // 执行到此时，尚不允许申请下次的数据。
	if (!HaveInquiry()) { // 没有现存的申请时
		SetInquiring(false); // 此标识的重置需要位于位于最后一步
	}
}

void CVirtualDataSource::SetDefaultSessionOption() const {
	DWORD dwValue = 0;

	m_pSession->QueryOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, dwValue); // 查询接收超时时间
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // 设置连接超时时间为120秒
	m_pSession->QueryOption(INTERNET_OPTION_RECEIVE_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // 设置接收超时时间为120秒
	m_pSession->QueryOption(INTERNET_OPTION_SEND_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // 设置发送超时时间为2秒
	m_pSession->QueryOption(INTERNET_OPTION_CONNECT_RETRIES, dwValue); // 查询重连次数
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 5); // 5次重试
}

void CVirtualDataSource::GenerateCurrentInquiryMessage() {
	ASSERT(m_pCurrentProduct != nullptr);
	m_strInquiryFunction = m_pCurrentProduct->CreateMessage();
	CreateTotalInquiringString();
}

void CVirtualDataSource::PrepareReadingWebData() {
	ConfigureSession();
	SetByteRead(0);
}

void CVirtualDataSource::GetWebDataImp() {
	PrepareReadingWebData();
	ReadWebData();
	if (!IsWebError()) {
		VerifyDataLength();
		const auto pWebData = CreateWebData();
		// 网络数据服务器正在使用时就可能被中止，故而存储当前数据时需要判断
		if (IsEnable()) StoreReceivedData(pWebData); // 当变更服务器（如中国市场的实时数据）时，要保证抛弃掉被变更服务器当前接收到的数据
	}
	else { // error handling
		DiscardAllInquiry(); // 当一次查询产生多次申请时，这些申请都是各自相关的，只要出现一次错误，其他的申请就无意义了。
		DiscardReceivedData();
		ASSERT(IsInquiring());
		SetInquiring(false); // 当工作线程出现故障时，直接重置数据申请标志。
	}
}

///////////////////////////////////////////////////////////////////////////
//
// 从网络读取数据。每次读1KB，直到读不到为止。
//
// 新浪实时数据服务器打开时间为100毫秒左右，网易实时数据服务器打开时间为350毫秒左右。
//
// 设置连接120秒超时和接收120秒超时、发送2秒超时、重复2次，这样应该能够满足所有网络要求。目前接收数据最长的是申请Tiingo stock，大致为60秒。--20220514
//
///////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::ReadWebData() {
	ASSERT(m_pFile == nullptr);
	try {
		OpenFile(GetInquiringString());
		GetFileHeaderInformation();
		if (m_lContentLength > 0) {
			m_sBuffer.resize(m_lContentLength + 1); // 调整缓存区大小，比实际数据大1字节,以防止越界访问。
		}
		else {
			m_sBuffer.resize(1024 * 1024);// 服务器不回报数据长度时，设置初始缓冲区为1M。
		}
		UINT lCurrentByteRead;
		do {
			if (gl_systemConfiguration.IsExitingSystem()) {
				CInternetException* e = new CInternetException(1); // 不为零即可。使用五位数以下的数据，避开系统预先声明的范围
				throw(e); // 当系统退出时，要立即中断此进程，以防止内存泄露。
			}
			lCurrentByteRead = ReadWebFileOneTime(); // 每次读取16K数据。
			XferReadingToBuffer(lCurrentByteRead);
			m_lByteRead += lCurrentByteRead;
			if (m_lContentLength == 0) IncreaseBufferSizeIfNeeded(1024 * 1024);
		} while (lCurrentByteRead > 0);
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
//
// 由于新浪实时数据服务器需要提供头部验证数据，故而OpenURL不再使用默认值，调用者需要各自设置m_strHeaders（默认为空）。
// 其他的数据尚未需要提供头部验证数据。
// 调用函数需要处理exception。
//
void CVirtualDataSource::OpenFile(const CString& strInquiring) {
	m_pFile = static_cast<CHttpFile*>(m_pSession->OpenURL(strInquiring, 1,
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

void CVirtualDataSource::QueryDataLength() {
	CString str;
	m_pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, str);
	if (str.GetLength() > 0) { // 正常时此字符串不为零
		char* p;
		m_lContentLength = strtol(str.GetBuffer(), &p, 10);
		ASSERT(m_lContentLength > 0);
	}
	else { // 服务器无响应
		m_lContentLength = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// 每次读取16K数据，然后将读取到的数据存入缓冲区
// 此函数的耗时，皆在Read函数，故而无法加快执行速度了。
//
////////////////////////////////////////////////////////////////////////////////////////////
UINT CVirtualDataSource::ReadWebFileOneTime() {
	const UINT uByteRead = m_pFile->Read(m_dataBuffer, WEB_SOURCE_DATA_BUFFER_SIZE_);
	sm_lTotalByteRead += uByteRead;
	return uByteRead;
}

//
// Debug编译模式下，使用memcpy函数完成，耗时154纳秒
// release编译模式下：使用逐字节拷贝，16KB耗时11微秒，使用memcpy函数完成，耗时120纳秒。
//
void CVirtualDataSource::XferReadingToBuffer(UINT uByteRead) {
	ASSERT(m_sBuffer.size() >= m_lByteRead + uByteRead);
	memcpy(&m_sBuffer.at(m_lByteRead), m_dataBuffer, uByteRead);
}

bool CVirtualDataSource::IncreaseBufferSizeIfNeeded(long lIncreaseSize) {
	if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) {
		// 数据可存储空间不到128K时
		m_sBuffer.resize(m_sBuffer.size() + lIncreaseSize); // 扩大lSize（默认为1M）数据范围
	}
	return true;
}

CWebDataPtr CVirtualDataSource::CreateWebData() {
	const auto pWebData = make_shared<CWebData>();
	pWebData->ResetCurrentPos();
	pWebData->SetTime(GetUTCTime());
	TransferDataToWebData(pWebData); // 将接收到的数据转移至pWebData中。由于使用std::move来加快速度，源数据不能再被使用。
	UpdateStatus(pWebData);

	return pWebData;
}

void CVirtualDataSource::VerifyDataLength() const {
	if (m_lContentLength > 0) {
		if (m_lContentLength != m_lByteRead) {
			CString str = _T("网络数据长度不符。预期长度：");
			char buffer[100];
			sprintf_s(buffer, _T("%d"), m_lContentLength);
			str += buffer;
			str += _T("，实际长度：");
			sprintf_s(buffer, _T("%d"), m_lByteRead);
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

void CVirtualDataSource::TransferDataToWebData(const CWebDataPtr& pWebData) {
	ASSERT(m_sBuffer.size() > m_lByteRead); // Note 即使知道数据总长度，也要多加上一个字节以防止越界。
	m_sBuffer.resize(m_lByteRead); // Note 缓冲区大小为实际数据量
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // 使用std::move以加速执行速度
}

void CVirtualDataSource::CreateTotalInquiringString() {
	m_strInquiry = m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken;
}

void CVirtualDataSource::CalcTotalBytePerSecond() {
	const long l = sm_lTotalByteRead;
	sm_lTotalByteRead = 0;
	sm_lTotalByteReadPerSecond = l;
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
