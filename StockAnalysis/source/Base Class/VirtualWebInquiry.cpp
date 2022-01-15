#include"pch.h"

#include"Thread.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"VirtualWebInquiry.h"

atomic_llong CVirtualWebInquiry::m_lTotalByteReaded = 0;

CVirtualWebInquiry::CVirtualWebInquiry() : CObject() {
	m_pSession = new CInternetSession{ _T("�������Ϊ�գ������ʱ����ֶ��Դ��󡣵���Ӱ����Խ��") };
	m_pFile = nullptr;
	m_lByteRead = 0;
	m_fWebError = false;
	m_dwWebErrorCode = 0;
	m_strInquire = _T("");
	m_strWebDataInquireMiddle = m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
	m_fReadingWebData = false; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
	m_vBuffer.resize(128 * 1024);

	m_lShortestInquiringInterval = 1000; // ÿ1���ѯһ�Ρ�
	m_lInquiringNumber = 500; // ÿ�β�ѯ����Ĭ��ֵΪ500
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
/// �����ô˺�����ȡ����������ʷ����ʱ��ż������ֳ�ʱ������������12002�����󣬿��Բ��ö��ȡ���ν��֮
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
			// ʹ��dynamic_castʱ��Address Sanitizer�ڴ˴�����
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
			Sleep(10); // �ȴ�10���롣���ȴ���ʵҲ���ԣ�
			if (iCountNumber++ > 2) { // �ظ���ȡ���ν�ʧ�ܺ��򱨴�
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
// �������ȡ���ݡ�ÿ�ζ�1KB��ֱ��������Ϊֹ��
// �����ô˺�����ȡ����������ʷ����ʱ��OpenFileż������ֳ�ʱ������������12002�����󣬿��Բ��ö��ȡ���ν��֮��
// ���ڷ���������·��ȡ�߳�Ҳż�����ֳ�ʱ���󣬶�����μ��ɽ��֮��--20211104
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
			if (gl_fExitingSystem) { // ��ϵͳ�˳�ʱ��Ҫ�����жϴ˽��̡�
				fReadingSuccess = false;
				break;
			}
			lCurrentByteReaded = ReadWebFileOneTime(); // ÿ�ζ�ȡ1K���ݡ�
		} while (lCurrentByteReaded > 0);
		ASSERT(m_vBuffer.size() > m_lByteRead);
		m_lTotalByteReaded += m_lByteRead;
		m_vBuffer.at(m_lByteRead) = 0x000; // �����0x000��β
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

/// ÿ�ζ�ȡ1K���ݣ�Ȼ�󽫶�ȡ�������ݴ��뻺����
/// </summary>
/// <param name=""></param>
/// <returns></returns>
UINT CVirtualWebInquiry::ReadWebFileOneTime(void) {
	char buffer[1024];
	const UINT uByteRead = m_pFile->Read(buffer, 1024);
	for (int i = 0; i < uByteRead; i++) {
		m_vBuffer.at(m_lByteRead++) = buffer[i];
	}
	if (m_vBuffer.size() < (m_lByteRead + 128 * 1024)) { // ����128Kʱ
		m_vBuffer.resize(m_vBuffer.size() + 128 * 1024); // �������ݷ�Χ
	}
	return uByteRead;
}

CWebDataPtr CVirtualWebInquiry::TransferReceivedDataToWebData() {
	CWebDataPtr pWebDataReceived = make_shared<CWebData>();
	auto byteReaded = GetByteReaded();
	m_vBuffer.resize(byteReaded + 1);
	pWebDataReceived->m_vDataBuffer = std::move(m_vBuffer); // ʹ��std::move�Լ���ִ���ٶ�
	m_vBuffer.resize(128 * 1024); // ���·����ڴ�

	pWebDataReceived->SetBufferLength(byteReaded);
	pWebDataReceived->ResetCurrentPos();
	return pWebDataReceived;
}

/////////////////////////////////////////////////////////////////////////
//
// ���Ǵ���Ψһ�Ľӿں���
//
//////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::GetWebData(void) {
	if (!IsReadingWebData()) {
		if (PrepareNextInquiringStr()) {
			SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
			StartReadingThread();
			return true;
		}
		else return false;
	}
	else return false;
}

bool CVirtualWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d��ʵʱ����\n", lNumberOfData);
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
// ͨ���������ݶ�ȡ�̡߳�
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