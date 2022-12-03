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

	m_pSession = new CInternetSession(_T("StockAnalysis")); // �˴���Ҫ���ϵ��ó�������ƣ������޷����е�Ԫ���Գ���ԭ��������
	SetDefaultSessionOption();
	m_pFile = nullptr;
	m_strHeaders = _T("");

	m_lByteRead = 0;
	m_fWebError = false;
	m_dwWebErrorCode = 0;
	m_strInquiry = _T("");
	m_strInquiryFunction = _T("");
	m_strInquiryToken = _T("");
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
	m_pDataSource = nullptr;
	if (m_pSession != nullptr) {
		delete m_pSession;
		m_pSession = nullptr;
	}
}

void CVirtualWebInquiry::SetDefaultSessionOption(void) {
	DWORD dwValue = 0;

	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // �������ӳ�ʱʱ��Ϊ120��
	m_pSession->QueryOption(INTERNET_OPTION_RECEIVE_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // ���ý��ճ�ʱʱ��Ϊ120��
	m_pSession->QueryOption(INTERNET_OPTION_SEND_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // ���÷��ͳ�ʱʱ��Ϊ2��
	m_pSession->QueryOption(INTERNET_OPTION_CONNECT_RETRIES, dwValue); // ��ѯ��������
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 5); // 5������
	m_pSession->QueryOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, dwValue); // ��ѯ���ճ�ʱʱ��
}

void CVirtualWebInquiry::Reset(void) noexcept {
	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
	m_fWebError = false;
}

/////////////////////////////////////////////////////////////////////////
//
// ���Ǵ���Ψһ�Ľӿں���
//
//////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::GetWebData(void) {
	ASSERT(m_pDataSource != nullptr);

	if (!IsReadingWebData()) { // �����߳�û��������
		if (PrepareNextInquiringString()) {
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
	ULONG64 llCurrentTickCount = GetTickCount64();

	ASSERT(IsReadingWebData());
	ASSERT(m_pDataSource != nullptr);
	PrepareReadingWebData();
	if (ReadingWebData()) {
		CWebDataPtr pWebData = make_shared<CWebData>();
		VerifyDataLength();
		TransferDataToWebData(pWebData); // �����յ�������ת����pWebData�С�����ʹ��std::move���ӿ��ٶȣ�Դ���ݲ����ٱ�ʹ�á�
		ResetBuffer();
		ParseData(pWebData);
		UpdateStatusWhenSecceed(pWebData);

		pWebData->SetTime(gl_pChinaMarket->GetUTCTime());
		m_pDataSource->StoreReceivedData(pWebData);
		m_pDataSource->SetDataReceived(true);
	}
	else { // error handling
		while (m_pDataSource->GetReceivedDataSize() > 0) m_pDataSource->GetReceivedData();
		m_pDataSource->SetInquiring(false); // �������̳߳��ֹ���ʱ��ֱ���������������־��
	}
	SetCurrentInquiryTime(GetTickCount64() - llCurrentTickCount); // ������ʹ��GetTickCount()�����汾��Ӧ��ռ�õ�ʱ���١�

	SetReadingWebData(false);
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
	bool fReadingSuccess = true;
	long lCurrentByteReaded = 0;

	ASSERT(IsReadingWebData());
	gl_ThreadStatus.IncreaseWebInquiringThread();
	SetByteReaded(0);

	ASSERT(m_pFile == nullptr);
	if (OpenFile(GetInquiringString())) {
		try {
			do {
				if (gl_systemStatus.IsExitingSystem()) { // ��ϵͳ�˳�ʱ��Ҫ�����жϴ˽��̣��Է�ֹ�ڴ�й¶��
					fReadingSuccess = false;
					break;
				}
				lCurrentByteReaded = ReadWebFileOneTime(); // ÿ�ζ�ȡ1K���ݡ�
				IncreaseBufferSizeIfNeeded();
			} while (lCurrentByteReaded > 0);
			m_lTotalByteReaded += m_lByteRead;
			// �������������Ķ�����ֻ�ڴ˴����С��Ա�ֻ֤�е�˳����ȡ���������ݺ󣬷������֮ǰ�Ĵ����ʶ��
			ClearWebError();// ���������루����еĻ���
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
/// �����ô˺�����ȡ����������ʷ����ʱ��ż������ֳ�ʱ������������12002������
/// Ŀǰ���������Ƕ�ȡfinnhub.ioʱ��������վ��ǽ���������Ӵ���--20220401
///
/// ���ú�����Ҫ����exception��
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
		// ��������ʵʱ���ݷ�������Ҫ�ṩͷ����֤���ݣ��ʶ�OpenURL����ʹ��Ĭ��ֵ����������Ҫ��������m_strHeaders��Ĭ��Ϊ�գ���
		// ������������δ��Ҫ�ṩͷ����֤���ݡ�
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

bool CVirtualWebInquiry::IncreaseBufferSizeIfNeeded(long lSize) {
	if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) { // ���ݿɴ洢�ռ䲻��128Kʱ
		m_sBuffer.resize(m_sBuffer.size() + lSize); // ����lSize��Ĭ��Ϊ1M�����ݷ�Χ
	}
	return true;
}

bool CVirtualWebInquiry::VerifyDataLength() {
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
			str += m_strInquiry.Left(120);
			gl_systemMessage.PushErrorMessage(str);
		}
		return false;
	}
	return true;
}

bool CVirtualWebInquiry::TransferDataToWebData(CWebDataPtr pWebData) {
	m_sBuffer.resize(m_lByteRead);
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // ʹ��std::move�Լ���ִ���ٶ�
	pWebData->SetBufferLength(m_lByteRead);
	pWebData->ResetCurrentPos();

	return true;
}

bool CVirtualWebInquiry::ReportStatus(long lNumberOfData) const {
	TRACE("����%d��ʵʱ����\n", lNumberOfData);
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