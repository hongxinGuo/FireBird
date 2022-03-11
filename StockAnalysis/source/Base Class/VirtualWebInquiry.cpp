#include"pch.h"

#include"Thread.h"

#include"globedef.h"
#include"ChinaMarket.h"

#include"VirtualWebInquiry.h"

atomic_llong CVirtualWebInquiry::m_lTotalByteReaded = 0;

CVirtualWebInquiry::CVirtualWebInquiry() : CObject() {
	m_pSession = new CInternetSession{ _T("�������Ϊ�գ������ʱ����ֶ��Դ��󡣵���Ӱ����Խ��") };
	m_pFile = nullptr;
	m_strHeaders = _T("");

	m_lByteRead = 0;
	m_fWebError = false;
	m_dwWebErrorCode = 0;
	m_strInquire = _T("");
	m_strWebDataInquireMiddle = m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
	m_fReadingWebData = false; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
	m_sBuffer.resize(1024 * 1024); // ���������£�1M������㹻�ˣ������ٴη����ڴ档

	m_lShortestInquiringInterval = 1000; // ÿ1���ѯһ�Ρ�
	m_lInquiringNumber = 500; // ÿ�β�ѯ����Ĭ��ֵΪ500
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
	long lHeadersLength = m_strHeaders.GetLength();

	ASSERT(m_pSession != nullptr);
	ASSERT(m_pFile == nullptr);
	do {
		try {
			// ʹ��dynamic_castʱ��Address Sanitizer�ڴ˴�����
			//m_pFile = dynamic_cast<CHttpFile*>(m_pSession->OpenURL((LPCTSTR)strInquiring));
			// ��������ʵʱ���ݷ�������Ҫ�ṩͷ��֤���ݣ��ʶ�OpenURL����ʹ��Ĭ��ֵ����������Ҫ����m_strHeaders��Ĭ��Ϊ�գ���
			m_pFile = static_cast<CHttpFile*>(m_pSession->OpenURL((LPCTSTR)strInquiring, 1, INTERNET_FLAG_TRANSFER_ASCII, (LPCTSTR)m_strHeaders, lHeadersLength));
			fDone = true;
		}
		catch (CInternetException* exception) {
			if (m_pFile != nullptr) {
				m_pFile->Close();
				delete m_pFile;
				m_pFile = nullptr;
			}
			Sleep(1); // �ȴ�10���롣���ȴ���ʵҲ���ԣ�
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
// �������ȡ���ݡ�ÿ�ζ�1KB��ֱ��������Ϊֹ��
// �����ô˺�����ȡ����������ʷ����ʱ��OpenFileż������ֳ�ʱ������������12002�����󣬿��Բ��ö��ȡ���ν��֮��
// ���ڷ���������·��ȡ�߳�Ҳż�����ֳ�ʱ���󣬶�����μ��ɽ��֮��--20211104
//
// ����ʵʱ���ݷ�������ʱ��Ϊ100�������ң�����ʵʱ���ݷ�������ʱ��Ϊ350�������ҡ�
//
///////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::ReadWebData(void) {
	m_pFile = nullptr;
	bool fReadingSuccess = true;
	long lCurrentByteReaded = 0;
	LARGE_INTEGER liBegin{ 0,0 }, liEnd{ 0,0 };

	ASSERT(IsReadingWebData());
	gl_ThreadStatus.IncreaseWebInquiringThread();
	SetWebError(false);
	SetByteReaded(0);
	QueryPerformanceCounter(&liBegin);
	if (OpenFile(GetInquiringString())) {
		QueryPerformanceCounter(&liEnd);
		do {
			if (gl_fExitingSystem) { // ��ϵͳ�˳�ʱ��Ҫ�����жϴ˽��̣��Է�ֹ�ڴ�й¶��
				fReadingSuccess = false;
				break;
			}
			lCurrentByteReaded = ReadWebFileOneTime(); // ÿ�ζ�ȡ1K���ݡ�

			if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) { // ���ݿɴ洢�ռ䲻��128Kʱ
				m_sBuffer.resize(m_sBuffer.size() + 1024 * 1024); // ����1M���ݷ�Χ
			}
		} while (lCurrentByteReaded > 0);
		ASSERT(m_sBuffer.size() > m_lByteRead);
		m_lTotalByteReaded += m_lByteRead;
		if (m_pFile != nullptr) {
			m_pFile->Close();
			delete m_pFile;
			m_pFile = nullptr;
		}
	}
	else fReadingSuccess = false;

	gl_ThreadStatus.DecreaseWebInquiringThread();
	ASSERT(gl_ThreadStatus.GetNumberOfWebInquiringThread() >= 0);

	SetCurrentInquiryTime((liEnd.QuadPart - liBegin.QuadPart) / 1000);

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
		m_sBuffer.at(m_lByteRead++) = buffer[i];
	}
	return uByteRead;
}

bool CVirtualWebInquiry::TransferData(CWebDataPtr pWebData) {
	auto byteReaded = GetByteReaded();

	if (m_lContentLength > 0) {
		if (m_lContentLength != byteReaded) gl_systemMessage.PushErrorMessage(_T("�������ݳ��Ȳ�����") + m_strInquire.Left(120));
	}
	m_sBuffer.resize(byteReaded);
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // ʹ��std::move�Լ���ִ���ٶ�
	pWebData->SetBufferLength(byteReaded);
	pWebData->ResetCurrentPos();

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// �������յ������ݣ�Ĭ�����ݸ�ʽΪJSon
//
////////////////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::ParseData(CWebDataPtr pWebData) {
	pWebData->m_fSucceedCreatePTree = pWebData->CreatePTree(0, 0);
	pWebData->SetJSonContentType(true);
	return pWebData->IsSucceedCreatePTree();
}

/////////////////////////////////////////////////////////////////////////
//
// ���Ǵ���Ψһ�Ľӿں���
//
//////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::GetWebData(void) {
	if (!IsReadingWebData()) { // �����߳�û��������
		if (PrepareNextInquiringStr()) {
			SetReadingWebData(true);  // �ڴ�������һ�Σ��Է����루�߳��ӳٵ��£�
			StartReadingThread();
			return true;
		}
		else return false;
	}
	else return false; // �����߳�����ִ�н������ݵ�����
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
// ͨ���������ݶ�ȡ�̡߳�
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT ThreadReadVirtualWebData(not_null<CVirtualWebInquiry*> pVirtualWebInquiry) {
	ASSERT(pVirtualWebInquiry->IsReadingWebData());
	LARGE_INTEGER liBegin{ 0,0 }, liEnd{ 0,0 };

	QueryPerformanceCounter(&liBegin);

	pVirtualWebInquiry->PrepareReadingWebData();
	if (pVirtualWebInquiry->ReadWebData()) {
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

	//pVirtualWebInquiry->SetCurrentInquiryTime((liEnd.QuadPart - liBegin.QuadPart) / 1000);

	return 1;
}