#include"pch.h"

#include"globedef.h"
#include"SystemMessage.h"
#include"ThreadStatus.h"
#include"Thread.h"

#include"ChinaMarket.h"

#include"VirtualWebInquiry.h"

atomic_llong CVirtualWebInquiry::m_lTotalByteReaded = 0;

CVirtualWebInquiry::CVirtualWebInquiry() : CObject() {
	DWORD dwValue = 0;

	m_pSession = new CInternetSession{ _T("StockAnalysis") };
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // 设置连接超时时间为120秒
	m_pSession->QueryOption(INTERNET_OPTION_RECEIVE_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // 设置接收超时时间为120秒
	m_pSession->QueryOption(INTERNET_OPTION_SEND_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // 设置发送超时时间为2秒
	m_pSession->QueryOption(INTERNET_OPTION_CONNECT_RETRIES, dwValue); // 查询重连次数
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); // 1次重试
	m_pSession->QueryOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, dwValue); // 查询接收超时时间
	m_pFile = nullptr;
	m_strHeaders = _T("");

	m_lByteRead = 0;
	m_fWebError = false;
	m_dwWebErrorCode = 0;
	m_strInquire = _T("");
	m_strWebDataInquireMiddle = m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
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
	if (m_pSession != nullptr) delete m_pSession;
}

void CVirtualWebInquiry::Reset(void) noexcept {
	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
	m_fWebError = false;
}

/// <summary>
/// 当采用此函数读取网易日线历史数据时，偶尔会出现超时（网络错误代码12002）错误，可以采用多读取几次解决之
/// 目前最大的问题是读取finnhub.io时，由于网站被墙而导致巨大延时。
///
/// </summary>
/// <param name="strInquiring"></param>
/// <returns></returns>
bool CVirtualWebInquiry::OpenFile(CString strInquiring) {
	bool fStatus = true;
	int iCounter = 0;
	bool fDone = false;
	CString strMessage, strErrorNo;
	char buffer[30];
	long lHeadersLength = m_strHeaders.GetLength();

	LARGE_INTEGER liBegin{ 0,0 }, liEnd{ 0,0 };
	bool fBegin = false, fEnd = false;

	fBegin = QueryPerformanceCounter(&liBegin);

	ASSERT(m_pSession != nullptr);
	ASSERT(m_pFile == nullptr);
	do {
		try {
			// 由于新浪实时数据服务器需要提供头验证数据，故而OpenURL不再使用默认值，调用者需要设置m_strHeaders（默认为空）。
			// 其他的数据尚未需要提供头验证数据。
			if (gl_fExitingSystem) {
				fStatus = false;
				fDone = true;
			}
			else {
				m_pFile = static_cast<CHttpFile*>(m_pSession->OpenURL((LPCTSTR)strInquiring, 1, INTERNET_FLAG_TRANSFER_ASCII, (LPCTSTR)m_strHeaders, lHeadersLength));
				fDone = true;
			}
		}
		catch (CInternetException* exception) {
			ASSERT(m_pFile == nullptr);
			if (m_pFile != nullptr) {
				m_pFile->Close();
				delete m_pFile;
				m_pFile = nullptr;
			}
			if (iCounter++ >= 0) { // 如果重试次数超过0次，则报告错误
				fEnd = QueryPerformanceCounter(&liEnd);
				long long ll = liEnd.QuadPart - liBegin.QuadPart;
				m_dwWebErrorCode = exception->m_dwError;
				sprintf_s(buffer, _T("%d"), exception->m_dwError);
				strErrorNo = buffer;
				strMessage = _T("Net Error #") + strErrorNo;
				sprintf_s(buffer, _T(" %lld毫秒"), ll / gl_lFrequency);
				strMessage += _T(" 用时:");
				strMessage += buffer;
				SetWebError(true);
				gl_systemMessage.PushErrorMessage(strMessage);
				fStatus = false;
				fDone = true;
			}
			exception->Delete();
		}
	} while (!fDone);
	if (fStatus) {
		CString str;
		m_pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, str);
		if (str.GetLength() > 0) {
			m_lContentLength = atol(str.GetBuffer());
		}
	}

	return fStatus;
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
	m_pFile = nullptr;
	bool fReadingSuccess = true;
	long lCurrentByteReaded = 0;
	CString strMessage, strErrorNo;
	char buffer[30];

	ASSERT(IsReadingWebData());
	gl_ThreadStatus.IncreaseWebInquiringThread();
	SetWebError(false);
	SetByteReaded(0);

	if (m_pSession != nullptr) delete m_pSession;
	m_pSession = new CInternetSession{ _T("StockAnalysis") };
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // 设置连接超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // 设置接收超时时间为120秒
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // 设置发送超时时间为2秒
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 2); // 2次重试
	if (OpenFile(GetInquiringString())) {
		try {
			do {
				if (gl_fExitingSystem) { // 当系统退出时，要立即中断此进程，以防止内存泄露。
					fReadingSuccess = false;
					break;
				}
				lCurrentByteReaded = ReadWebFileOneTime(); // 每次读取1K数据。

				if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) { // 数据可存储空间不到128K时
					m_sBuffer.resize(m_sBuffer.size() + 1024 * 1024); // 扩大1M数据范围
				}
			} while (lCurrentByteReaded > 0);
			m_lTotalByteReaded += m_lByteRead;
			if (m_pFile != nullptr) {
				m_pFile->Close();
				delete m_pFile;
				m_pFile = nullptr;
			}
		}
		catch (CInternetException* exception) {
			fReadingSuccess = false;
			m_dwWebErrorCode = exception->m_dwError;
			sprintf_s(buffer, _T("%d"), exception->m_dwError);
			strErrorNo = buffer;
			strMessage = _T("Net Error #") + strErrorNo;
			SetWebError(true);
			gl_systemMessage.PushErrorMessage(strMessage);
		}
	}
	else fReadingSuccess = false;

	if (m_pSession != nullptr) {
		delete m_pSession;
		m_pSession = nullptr;
	}

	gl_ThreadStatus.DecreaseWebInquiringThread();

	return fReadingSuccess;
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

bool CVirtualWebInquiry::TransferData(CWebDataPtr pWebData) {
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
			str += m_strInquire.Left(120);
			gl_systemMessage.PushErrorMessage(str);
		}
	}
	m_sBuffer.resize(byteReaded);
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // 使用std::move以加速执行速度
	pWebData->SetBufferLength(byteReaded);
	pWebData->ResetCurrentPos();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 解析接收到的数据，默认数据格式为JSon, 使用boost perproty tree解析
//
////////////////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::ParseData(CWebDataPtr pWebData) {
	pWebData->m_fParsed = pWebData->CreatePTree(0, 0);
	pWebData->SetJSonContentType(true);
	return pWebData->IsParsed();
}

/////////////////////////////////////////////////////////////////////////
//
// 这是此类唯一的接口函数
//
//////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::GetWebData(void) {
	if (!IsReadingWebData()) { // 工作线程没有启动？
		if (PrepareNextInquiringStr()) {
			SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
			StartReadingThread();
			return true;
		}
		else return false;
	}
	else return false; // 工作线程已在执行接收数据的任务
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
	LARGE_INTEGER liBegin{ 0,0 }, liEnd{ 0,0 };
	bool fBegin = false, fEnd = false;
	long long  differ = 0;

	fBegin = QueryPerformanceCounter(&liBegin);

	ASSERT(IsReadingWebData());
	PrepareReadingWebData();
	if (ReadingWebData()) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		TransferData(pWebData);
		SetTime(pWebData);
		ParseData(pWebData);
		StoreWebData(pWebData);
		ResetBuffer();
		UpdateStatusWhenSecceed(pWebData);
	}
	else { // error handling
		ClearUpIfReadingWebDataFailed();
	}
	UpdateAfterReadingWebData();

	fEnd = QueryPerformanceCounter(&liEnd);
	if (fBegin && fEnd) {
		differ = liEnd.QuadPart - liBegin.QuadPart;
		SetCurrentInquiryTime(differ / gl_lFrequency);
	}

	SetReadingWebData(false);
}

bool CVirtualWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个实时数据\n", lNumberOfData);
	return true;
}

void CVirtualWebInquiry::CreateTotalInquiringString(CString strMiddle) {
	m_strInquire = m_strWebDataInquirePrefix + strMiddle + m_strWebDataInquireSuffix;
}

void CVirtualWebInquiry::SetTime(CWebDataPtr pData) {
	pData->SetTime(gl_pChinaMarket->GetUTCTime());
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 通用网络数据读取线程。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
UINT ThreadReadVirtualWebData(not_null<CVirtualWebInquiry*> pVirtualWebInquiry) {
	ASSERT(pVirtualWebInquiry->IsReadingWebData());
	LARGE_INTEGER liBegin{ 0,0 }, liEnd{ 0,0 };

	QueryPerformanceCounter(&liBegin);

	pVirtualWebInquiry->PrepareReadingWebData();
	if (pVirtualWebInquiry->ReadingWebData()) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		pVirtualWebInquiry->TransferData(pWebData);
		pVirtualWebInquiry->ParseData(pWebData);
		pVirtualWebInquiry->ResetBuffer();

		pVirtualWebInquiry->SetTime(pWebData);
		pVirtualWebInquiry->UpdateStatusWhenSecceed(pWebData);
		pVirtualWebInquiry->StoreWebData(pWebData);
	}
	else { // error handling
		pVirtualWebInquiry->ClearUpIfReadingWebDataFailed();
	}
	pVirtualWebInquiry->UpdateAfterReadingWebData();

	pVirtualWebInquiry->SetReadingWebData(false);

	QueryPerformanceCounter(&liEnd);
	pVirtualWebInquiry->SetCurrentInquiryTime((liEnd.QuadPart - liBegin.QuadPart) / 1000);

	return 1;
}
*/