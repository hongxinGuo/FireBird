#include"pch.h"

#include"ThreadStatus.h"
#include"Thread.h"

#include"ChinaMarket.h"

#include"VirtualWebInquiry.h"
#include"HighPerformanceCounter.h"

atomic_llong CVirtualWebInquiry::m_lTotalByteReaded = 0;

CVirtualWebInquiry::CVirtualWebInquiry() : CObject() {
	DWORD dwValue = 0;

	m_pSession = new CInternetSession(_T("StockAnalysis")); // �˴���Ҫ���ϵ��ó�������ƣ������޷����е�Ԫ���Գ���ԭ��������
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // �������ӳ�ʱʱ��Ϊ120��
	m_pSession->QueryOption(INTERNET_OPTION_RECEIVE_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // ���ý��ճ�ʱʱ��Ϊ120��
	m_pSession->QueryOption(INTERNET_OPTION_SEND_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // ���÷��ͳ�ʱʱ��Ϊ2��
	m_pSession->QueryOption(INTERNET_OPTION_CONNECT_RETRIES, dwValue); // ��ѯ��������
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 5); // 5������
	m_pSession->QueryOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, dwValue); // ��ѯ���ճ�ʱʱ��
	m_pFile = nullptr;
	m_strHeaders = _T("");

	m_lByteRead = 0;
	m_fWebError = false;
	m_dwWebErrorCode = 0;
	m_strInquire = _T("");
	m_strWebDataInquireMiddle = m_strWebDataInquirePrefix = m_strWebDataInquireSuffix = _T("");
	m_fReadingWebData = false; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
	m_sBuffer.resize(__DefaultWebDataBufferSize__); // ���������£�1M������㹻�ˣ������ٴη����ڴ档

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
	if (m_pSession != nullptr) delete m_pSession;
}

void CVirtualWebInquiry::Reset(void) noexcept {
	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
	m_fWebError = false;
}

///////////////////////////////////////////////////////////////////////////
//
// �������ȡ���ݡ�ÿ�ζ�1KB��ֱ��������Ϊֹ��
// �����ô˺�����ȡ����������ʷ����ʱ��OpenFileż������ֳ�ʱ������������12002�����󣬿��Բ��ö��ȡ���ν��֮��
// ���ڷ���������·��ȡ�߳�Ҳż�����ֳ�ʱ���󣬶�����μ��ɽ��֮��--20211104
//
// ����ʵʱ���ݷ�������ʱ��Ϊ100�������ң�����ʵʱ���ݷ�������ʱ��Ϊ350�������ҡ�
//
// ��������120�볬ʱ�ͽ���120�볬ʱ������2�볬ʱ���ظ�2�Σ�����Ӧ���ܹ�������������Ҫ��Ŀǰ�����������������Tiingo stock������Ϊ60�롣--20220514
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

	if (OpenFile(GetInquiringString())) {
		try {
			do {
				if (gl_systemStatus.IsExitingSystem()) { // ��ϵͳ�˳�ʱ��Ҫ�����жϴ˽��̣��Է�ֹ�ڴ�й¶��
					fReadingSuccess = false;
					break;
				}
				lCurrentByteReaded = ReadWebFileOneTime(); // ÿ�ζ�ȡ1K���ݡ�

				if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) { // ���ݿɴ洢�ռ䲻��128Kʱ
					m_sBuffer.resize(m_sBuffer.size() + 1024 * 1024); // ����1M���ݷ�Χ
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
			strMessage += _T("Message: ");
			strMessage += m_strInquire;
			SetWebError(true);
			gl_systemMessage.PushErrorMessage(strMessage);
		}
	}
	else fReadingSuccess = false;

	gl_ThreadStatus.DecreaseWebInquiringThread();

	return fReadingSuccess;
}

/// <summary>
/// �����ô˺�����ȡ����������ʷ����ʱ��ż������ֳ�ʱ������������12002�����󣬿��Բ��ö��ȡ���ν��֮
/// Ŀǰ���������Ƕ�ȡfinnhub.ioʱ��������վ��ǽ�����¾޴���ʱ��
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
	CHighPerformanceCounter counter;

	counter.Start();

	ASSERT(m_pSession != nullptr);
	ASSERT(m_pFile == nullptr);
	do {
		try {
			// ��������ʵʱ���ݷ�������Ҫ�ṩͷ��֤���ݣ��ʶ�OpenURL����ʹ��Ĭ��ֵ����������Ҫ����m_strHeaders��Ĭ��Ϊ�գ���
			// ������������δ��Ҫ�ṩͷ��֤���ݡ�
			if (gl_systemStatus.IsExitingSystem()) {
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
			if (iCounter++ >= 0) { // ������Դ�������0�Σ��򱨸����
				counter.Stop();
				m_dwWebErrorCode = exception->m_dwError;
				sprintf_s(buffer, _T("%d"), exception->m_dwError);
				strErrorNo = buffer;
				strMessage = _T("Net Error #") + strErrorNo;
				sprintf_s(buffer, _T(" %lld����"), counter.GetElapsedMilliSecond());
				strMessage += _T(" ��ʱ:");
				strMessage += buffer;
				strMessage += _T(" message: ");
				strMessage += m_strInquire;
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

////////////////////////////////////////////////////////////////////////////////////////////
//
// ÿ�ζ�ȡ1K���ݣ�Ȼ�󽫶�ȡ�������ݴ��뻺����
// �˺����ĺ�ʱ������Read�������ʶ��޷��ӿ�ִ���ٶ��ˡ�
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
	CString str = _T("�������ݳ��Ȳ�����Ԥ�ڳ��ȣ�");

	if (m_lContentLength > 0) {
		if (m_lContentLength != byteReaded) {
			sprintf_s(buffer, _T("%d"), m_lContentLength);
			str += buffer;
			str += _T("��ʵ�ʳ��ȣ�");
			sprintf_s(buffer, _T("%I64d"), byteReaded);
			str += buffer;
			str += m_strInquire.Left(120);
			gl_systemMessage.PushErrorMessage(str);
		}
	}
	m_sBuffer.resize(byteReaded);
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // ʹ��std::move�Լ���ִ���ٶ�
	pWebData->SetBufferLength(byteReaded);
	pWebData->ResetCurrentPos();

	return true;
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
	CHighPerformanceCounter counter;

	counter.Start();
	ASSERT(IsReadingWebData());
	PrepareReadingWebData();
	if (ReadingWebData()) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		//counter.start();
		TransferData(pWebData);
		ParseData(pWebData);
		ResetBuffer();

		SetTime(pWebData);
		UpdateStatusWhenSecceed(pWebData);
		StoreWebData(pWebData);
	}
	else { // error handling
		ClearUpIfReadingWebDataFailed();
	}
	UpdateAfterReadingWebData();

	counter.Stop();
	SetCurrentInquiryTime(counter.GetElapsedMilliSecond()); // ��ʾ���Ǻ���ֵ

	SetReadingWebData(false);
}

bool CVirtualWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d��ʵʱ����\n", lNumberOfData);
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