#include"pch.h"
#include"afxinet.h"

#include"Thread.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"VirtualWebInquiry.h"

atomic_llong CVirtualWebInquiry::m_lTotalByteReaded = 0;

CVirtualWebInquiry::CVirtualWebInquiry() : CObject() {
	m_pSession = new CInternetSession{ _T("如果此项为空，则测试时会出现断言错误。但不影响测试结果") };
	m_pFile = nullptr;
	m_lByteRead = 0;
	m_fWebError = false;
	m_dwWebErrorCode = 0;
	m_strInquire = _T("");
	m_strWebDataInquireMiddle = m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
	m_fReadingWebData = false; // 接收实时数据线程是否执行标识
	m_vBuffer.resize(128 * 1024);

	m_lInquiringNumber = 500; // 每次查询数量默认值为500

	m_tCurrentInquiryTime = 0;

#ifdef DEBUG
	m_fReportStatus = false;
#else
	m_fReportStatus = false;
#endif
}

CVirtualWebInquiry::~CVirtualWebInquiry(void) {
	delete m_pSession;
}

void CVirtualWebInquiry::Reset(void) noexcept {
	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
	m_fWebError = false;
}

/// <summary>
/// 当采用此函数读取网易日线历史数据时，偶尔会出现超时（网络错误代码12002）错误，可以采用多读取几次解决之
///
/// </summary>
/// <param name="strInquiring"></param>
/// <returns></returns>
bool CVirtualWebInquiry::OpenFile(CString strInquiring) {
	bool fStatus = true;
	bool fDone = false;
	int iCountNumber = 0;
	CString strMessage, strLeft, strErrorNo;
	char buffer[30];

	ASSERT(m_pSession != nullptr);
	ASSERT(m_pFile == nullptr);
	do {
		try {
			// 使用dynamic_cast时，Address Sanitizer在此处报错
			//m_pFile = dynamic_cast<CHttpFile*>(m_pSession->OpenURL((LPCTSTR)strInquiring));
			m_pFile = static_cast<CHttpFile*>(m_pSession->OpenURL((LPCTSTR)strInquiring));
			fDone = true;
		}
		catch (CInternetException* exception) {
			if (m_pFile != nullptr) {
				m_pFile->Close();
				delete m_pFile;
				m_pFile = nullptr;
			}
			sprintf_s(buffer, _T("%d"), exception->m_dwError);
			strErrorNo = buffer;
			strLeft = GetInquiringString().Left(80);
			strMessage = _T("Net Warning No ") + strErrorNo + _T(" : ") + strLeft;
			gl_systemMessage.PushErrorMessage(strMessage);
			Sleep(10); // 等待10毫秒。不等待其实也可以，
			if (iCountNumber++ > 2) { // 重复读取三次皆失败后，则报错。
				SetWebError(true);
				m_dwWebErrorCode = exception->m_dwError;
				TRACE(_T("Net Error No %d,  %s\n"), exception->m_dwError, (LPCTSTR)strLeft);
				sprintf_s(buffer, _T("%d"), exception->m_dwError);
				strErrorNo = buffer;
				strMessage = _T("Net Error No ") + strErrorNo + _T(" : ") + strLeft + _T("\n");
				gl_systemMessage.PushErrorMessage(strMessage);
				fStatus = false;
				fDone = true;
			}
			exception->Delete();
		}
	} while (!fDone);

	return fStatus;
}

///////////////////////////////////////////////////////////////////////////
//
// 从网络读取数据。每次读1KB，直到读不到为止。
// 当采用此函数读取网易日线历史数据时，OpenFile偶尔会出现超时（网络错误代码12002）错误，可以采用多读取几次解决之。
// 现在发现其他网路读取线程也偶尔出现超时错误，多读几次即可解决之。--20211104
//
///////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::ReadWebData(void) {
	m_pFile = nullptr;
	bool fReadingSuccess = true;
	time_t tt = 0;
	long lCurrentByteReaded = 0;

	ASSERT(IsReadingWebData());
	gl_ThreadStatus.IncreaseWebInquiringThread();
	SetWebError(false);
	SetByteReaded(0);
	tt = GetTickCount64();
	if (OpenFile(GetInquiringString())) {
		do {
			if (gl_fExitingSystem) { // 当系统退出时，要立即中断此进程。
				fReadingSuccess = false;
				break;
			}
			lCurrentByteReaded = ReadWebFileOneTime(); // 每次读取1K数据。
		} while (lCurrentByteReaded > 0);
		ASSERT(m_vBuffer.size() > m_lByteRead);
		m_lTotalByteReaded += m_lByteRead;
		m_vBuffer.at(m_lByteRead) = 0x000; // 最后以0x000结尾
		if (m_pFile != nullptr) {
			m_pFile->Close();
			delete m_pFile;
			m_pFile = nullptr;
		}
	}
	else fReadingSuccess = false;

	m_tCurrentInquiryTime = GetTickCount64() - tt;
	gl_ThreadStatus.DecreaseWebInquiringThread();
	ASSERT(gl_ThreadStatus.GetNumberOfWebInquiringThread() >= 0);

	return fReadingSuccess;
}

///////////////////////////////////////////////////////////////////////////
//
// 从网络读取数据。每次读1KB，读不到等待若干毫秒，共等待三次。
// 
// 这种方式目前已废弃，暂时保留此功能以备不时之需。
//
//
///////////////////////////////////////////////////////////////////////////

bool CVirtualWebInquiry::ReadWebDataTimeLimit(long lFirstDelayTime, long lSecondDelayTime, long lThirdDelayTime) {
	m_pFile = nullptr;
	bool fDone = false;
	bool fStatus = true;
	time_t tt = 0;

	ASSERT(IsReadingWebData());

	gl_ThreadStatus.IncreaseWebInquiringThread();

	long lCurrentByteReaded = 0;
	SetWebError(false);
	SetByteReaded(0);
	tt = GetTickCount64();
	if (OpenFile(GetInquiringString())) {
		Sleep(lFirstDelayTime); // 服务器延迟lStartDelayTime毫秒即可。
		while (!fDone) {
			do {
				lCurrentByteReaded = ReadWebFileOneTime();
			} while (lCurrentByteReaded > 0);
			Sleep(lSecondDelayTime); // 等待lSecondDelayTime毫秒后再读一次，确认没有新数据后才返回。
			if ((lCurrentByteReaded = ReadWebFileOneTime()) == 0) {
				Sleep(lThirdDelayTime); // 等待lThirdDelayTime毫秒后读第三次，确认没有新数据后才返回，否则继续读。
				if ((lCurrentByteReaded = ReadWebFileOneTime()) == 0) {
					fDone = true;
				}
			}
		}
		ASSERT(m_vBuffer.size() > m_lByteRead);
		m_vBuffer.at(m_lByteRead) = 0x000; // 最后以0x000结尾
		if (m_pFile != nullptr) {
			m_pFile->Close();
			delete m_pFile;
			m_pFile = nullptr;
		}
		m_lTotalByteReaded += m_lByteRead; //
	}
	else fStatus = false;

	m_tCurrentInquiryTime = GetTickCount64() - tt;
	gl_ThreadStatus.DecreaseWebInquiringThread();
	ASSERT(gl_ThreadStatus.GetNumberOfWebInquiringThread() >= 0);
	return fStatus;
}

/// <summary>
/// 每次读取1K数据，然后将读取到的数据存入缓冲区
/// </summary>
/// <param name=""></param>
/// <returns></returns>
UINT CVirtualWebInquiry::ReadWebFileOneTime(void) {
	char buffer[1024];
	const UINT uByteRead = m_pFile->Read(buffer, 1024);
	for (int i = 0; i < uByteRead; i++) {
		m_vBuffer.at(m_lByteRead++) = buffer[i];
	}
	if (m_vBuffer.size() < (m_lByteRead + 128 * 1024)) { // 相差不到128K时
		m_vBuffer.resize(m_vBuffer.size() + 128 * 1024); // 扩大数据范围
	}
	return uByteRead;
}

CWebDataPtr CVirtualWebInquiry::TransferReceivedDataToWebData() {
	CWebDataPtr pWebDataReceived = make_shared<CWebData>();
	auto byteReaded = GetByteReaded();
	pWebDataReceived->Resize(byteReaded + 1);
	for (int i = 0; i < byteReaded + 1; i++) {// 缓冲区需要多加一个字符长度（最后那个0x00)
		pWebDataReceived->SetData(i, m_vBuffer.at(i));
	}
	pWebDataReceived->SetBufferLength(byteReaded);
	pWebDataReceived->ResetCurrentPos();
	return pWebDataReceived;
}

/////////////////////////////////////////////////////////////////////////
//
// 这是此类唯一的接口函数
//
//////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::GetWebData(void) {
	if (!IsReadingWebData()) {
		if (PrepareNextInquiringStr()) {
			SetReadingWebData(true);  // 在此先设置一次，以防重入（线程延迟导致）
			StartReadingThread();
			return true;
		}
		else return false;
	}
	else return false;
}

bool CVirtualWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("读入%d个实时数据\n", lNumberOfData);
	return true;
}

void CVirtualWebInquiry::CreateTotalInquiringString(CString strMiddle) {
	m_strInquire = m_strWebDataInquirePrefix + strMiddle + m_strWebDataInquireSuffix;
}

void CVirtualWebInquiry::StartReadingThread(void) {
	thread thread1(ThreadReadVirtualWebData, this);
	thread1.detach();
}

void CVirtualWebInquiry::SetTime(CWebDataPtr pData) {
	pData->SetTime(gl_pChinaMarket->GetUTCTime());
}

void CVirtualWebInquiry::__TESTSetBuffer(char* buffer, INT64 lTotalNumber) {
	if (m_vBuffer.size() < (lTotalNumber + 128 * 1024)) {
		m_vBuffer.resize(lTotalNumber + 128 * 1024);
	}
	for (INT64 i = 0; i < lTotalNumber; i++) {
		m_vBuffer.at(i) = buffer[i];
	}
	m_vBuffer.at(lTotalNumber) = 0x000;
	m_lByteRead = lTotalNumber;
}

void CVirtualWebInquiry::__TESTSetBuffer(CString str) {
	INT64 lTotalNumber = str.GetLength();
	char* buffer = str.GetBuffer();
	if (m_vBuffer.size() < (lTotalNumber + 128 * 1024)) {
		m_vBuffer.resize(lTotalNumber + 128 * 1024);
	}
	for (INT64 i = 0; i < lTotalNumber; i++) {
		m_vBuffer.at(i) = buffer[i];
	}
	m_vBuffer.at(lTotalNumber) = 0x000;
	m_lByteRead = lTotalNumber;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 通用网络数据读取线程。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadReadVirtualWebData(not_null<CVirtualWebInquiry*> pVirtualWebInquiry) {
	ASSERT(pVirtualWebInquiry->IsReadingWebData());

	pVirtualWebInquiry->PrepareReadingWebData();
	if (pVirtualWebInquiry->ReadWebData()) {
		CWebDataPtr pWebData = pVirtualWebInquiry->TransferReceivedDataToWebData();
		if (pWebData != nullptr) {
			pVirtualWebInquiry->SetTime(pWebData);
			pVirtualWebInquiry->UpdateStatusWhenSecceed(pWebData);
			pVirtualWebInquiry->StoreWebData(pWebData);
		}
	}
	else { // error handling
		pVirtualWebInquiry->ClearUpIfReadingWebDataFailed();
	}
	pVirtualWebInquiry->UpdateAfterReadingWebData();

	pVirtualWebInquiry->SetReadingWebData(false);

	return 1;
}