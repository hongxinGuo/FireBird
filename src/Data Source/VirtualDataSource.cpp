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
	m_fEnable = true; // Ĭ��Ϊ����ִ��

	m_pSession = make_shared<CInternetSession>(_T("FireBird")); // �˴���Ҫ���ϵ��ó�������ƣ������޷����е�Ԫ���Գ���ԭ��������
	SetDefaultSessionOption();
	m_pFile = nullptr;
	m_strHeaders = _T("");

	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
	m_strInquiry = _T("");
	m_strInquiryFunction = _T("");
	m_strSuffix = _T("");
	m_strInquiryToken = _T("");

	m_lInquiringNumber = 500; // ÿ�β�ѯ����Ĭ��ֵΪ500
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
/// DataSource�Ķ��㺯����
/// ��������ϢΪ��ʱ�����ɵ�ǰ��ѯ�ַ�����
/// ������������Ϣ��û���������еĲ�ѯ�߳�ʱ�����ɲ�ѯ�̡߳�
///
/// Note ���ú�������ʹ��thread_pool_executor����background_executor��ֻ��ʹ��thread_executor������thread_pool_executor�������̵߳ķ���ֵ�޷���ȡ��ԭ����顣
///
/// </summary>
///
/// ����ʹ�ö�����thread_executor�������У�����ʹ��thread_pool_executor����background_executor��
//  �����������ʹ�õ�thread_pool_executor����֮������ͻ�����²���ͬ�����⡣
///
/// lCurrentLocalMarketTime����ǰ�г�ʱ��
///
////////////////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::Run(const long lCurrentLocalMarketTime) {
	if (!IsInquiring()) {
		ASSERT(!HaveInquiry());
		GenerateInquiryMessage(lCurrentLocalMarketTime);
	}

	if (HaveInquiry() && !IsWorkingThreadRunning()) {
		ASSERT(gl_systemConfiguration.IsWorkingMode()); // ���������
		ASSERT(IsInquiring());
		GetCurrentProduct();
		GenerateCurrentInquiryMessage();
		SetWorkingThreadRunning(true); // �ڵ��ù����߳�ǰ�����ø�ֵ
		gl_runtime.thread_executor()->post([this] { //Note ����ʹ�ö�����thread_executor�������У�����ʹ��thread_pool_executor����background_executor
				gl_ThreadStatus.IncreaseWebInquiringThread();
				this->GetWebDataAndProcessIt();
				gl_ThreadStatus.DecreaseWebInquiringThread();
			});
	}
}

void CVirtualDataSource::GetWebDataAndProcessIt() {
	CHighPerformanceCounter counter;

	ASSERT(IsWorkingThreadRunning());// �ڵ��ù����߳�ǰ������
	counter.start();
	GetWebData();
	if (!IsWebError()) {
		if (IsEnable()) ProcessWebDataReceived(); // ֻ�е�������������ʹ��ʱ���Ŵ�����յ�����������
		else DiscardReceivedData(); // ����������
	}
	counter.stop();
	SetWorkingThreadRunning(false);
	SetCurrentInquiryTime(counter.GetElapsedMillisecond());
}

//////////////////////////////////////////////
//
//
// ֻ����ͬʱ����һ����Ϣ������Ϣ��������������֮ǰ������������һ����Ϣ�������ܹ���֤ͬһ�ԡ�������Finnhub��վ���ٶ����ƣ�
// ÿ����ֻ����60�����룬�ʶ�û�б�Ҫǿ�������ٶȡ�
//
//
//////////////////////////////////////////////
void CVirtualDataSource::ProcessWebDataReceived() {
	if (HaveReceivedData()) {// ����ǰ��������
		ASSERT(m_pCurrentProduct != nullptr);
		const CWebDataPtr pWebData = GetReceivedData();
		CheckInaccessible(pWebData);
		m_pCurrentProduct->ParseAndStoreWebData(pWebData);
		m_pCurrentProduct->UpdateDataSourceStatus(this->GetShared()); // ���ﴫ�ݵ���ʵ��DataSource����ָ��
	}
	ASSERT(IsInquiring()); // ִ�е���ʱ���в����������´ε����ݡ�
	if (!HaveInquiry()) { // û���ִ������ʱ
		SetInquiring(false); // �˱�ʶ��������Ҫλ��λ�����һ��
	}
}

void CVirtualDataSource::SetDefaultSessionOption() const {
	DWORD dwValue = 0;

	m_pSession->QueryOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, dwValue); // ��ѯ���ճ�ʱʱ��
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 120000); // �������ӳ�ʱʱ��Ϊ120��
	m_pSession->QueryOption(INTERNET_OPTION_RECEIVE_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 120000); // ���ý��ճ�ʱʱ��Ϊ120��
	m_pSession->QueryOption(INTERNET_OPTION_SEND_TIMEOUT, dwValue);
	m_pSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000); // ���÷��ͳ�ʱʱ��Ϊ2��
	m_pSession->QueryOption(INTERNET_OPTION_CONNECT_RETRIES, dwValue); // ��ѯ��������
	m_pSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 5); // 5������
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
		// �������ݷ���������ʹ��ʱ�Ϳ��ܱ���ֹ���ʶ��洢��ǰ����ʱ��Ҫ�ж�
		if (IsEnable()) StoreReceivedData(pWebData); // ����������������й��г���ʵʱ���ݣ�ʱ��Ҫ��֤�������������������ǰ���յ�������
	}
	else { // error handling
		DiscardAllInquiry(); // ��һ�β�ѯ�����������ʱ����Щ���붼�Ǹ�����صģ�ֻҪ����һ�δ���������������������ˡ�
		DiscardReceivedData();
		ASSERT(IsInquiring());
		SetInquiring(false); // �������̳߳��ֹ���ʱ��ֱ���������������־��
	}
}

///////////////////////////////////////////////////////////////////////////
//
// �������ȡ���ݡ�ÿ�ζ�1KB��ֱ��������Ϊֹ��
//
// ����ʵʱ���ݷ�������ʱ��Ϊ100�������ң�����ʵʱ���ݷ�������ʱ��Ϊ350�������ҡ�
//
// ��������120�볬ʱ�ͽ���120�볬ʱ������2�볬ʱ���ظ�2�Σ�����Ӧ���ܹ�������������Ҫ��Ŀǰ�����������������Tiingo stock������Ϊ60�롣--20220514
//
///////////////////////////////////////////////////////////////////////////
void CVirtualDataSource::ReadWebData() {
	ASSERT(m_pFile == nullptr);
	try {
		OpenFile(GetInquiringString());
		GetFileHeaderInformation();
		if (m_lContentLength > 0) {
			m_sBuffer.resize(m_lContentLength + 1); // ������������С����ʵ�����ݴ�1�ֽ�,�Է�ֹԽ����ʡ�
		}
		else {
			m_sBuffer.resize(1024 * 1024);// ���������ر����ݳ���ʱ�����ó�ʼ������Ϊ1M��
		}
		UINT lCurrentByteRead;
		do {
			if (gl_systemConfiguration.IsExitingSystem()) {
				CInternetException* e = new CInternetException(1); // ��Ϊ�㼴�ɡ�ʹ����λ�����µ����ݣ��ܿ�ϵͳԤ�������ķ�Χ
				throw(e); // ��ϵͳ�˳�ʱ��Ҫ�����жϴ˽��̣��Է�ֹ�ڴ�й¶��
			}
			lCurrentByteRead = ReadWebFileOneTime(); // ÿ�ζ�ȡ16K���ݡ�
			XferReadingToBuffer(lCurrentByteRead);
			m_lByteRead += lCurrentByteRead;
			if (m_lContentLength == 0) IncreaseBufferSizeIfNeeded(1024 * 1024);
		} while (lCurrentByteRead > 0);
		// �������������Ķ�����ֻ�ڴ˴����С��Ա�ֻ֤�е�˳����ȡ���������ݺ󣬷������֮ǰ�Ĵ����ʶ��
		m_dwWebErrorCode = 0; // ���������루����еĻ�����ֻ�ڴ˴����øô�����롣
	}
	catch (CInternetException* exception) {//����һ����ʹ�����á�����׼���ڴ�������ɾ��������⣬�ʶ�ֱ��ʹ��ָ�롣��������ϵͳ����������⣬�ᵼ�³����Զ��˳���
		m_dwWebErrorCode = exception->m_dwError;
		ReportWebError(m_dwWebErrorCode, m_strInquiry);
		exception->Delete();
	}
	DeleteWebFile();
}

// <summary>
//
// ��������ʵʱ���ݷ�������Ҫ�ṩͷ����֤���ݣ��ʶ�OpenURL����ʹ��Ĭ��ֵ����������Ҫ��������m_strHeaders��Ĭ��Ϊ�գ���
// ������������δ��Ҫ�ṩͷ����֤���ݡ�
// ���ú�����Ҫ����exception��
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
	if (str.GetLength() > 0) { // ����ʱ���ַ�����Ϊ��
		char* p;
		m_lContentLength = strtol(str.GetBuffer(), &p, 10);
		ASSERT(m_lContentLength > 0);
	}
	else { // ����������Ӧ
		m_lContentLength = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//
// ÿ�ζ�ȡ16K���ݣ�Ȼ�󽫶�ȡ�������ݴ��뻺����
// �˺����ĺ�ʱ������Read�������ʶ��޷��ӿ�ִ���ٶ��ˡ�
//
////////////////////////////////////////////////////////////////////////////////////////////
UINT CVirtualDataSource::ReadWebFileOneTime() {
	const UINT uByteRead = m_pFile->Read(m_dataBuffer, WEB_SOURCE_DATA_BUFFER_SIZE_);
	sm_lTotalByteRead += uByteRead;
	return uByteRead;
}

//
// Debug����ģʽ�£�ʹ��memcpy������ɣ���ʱ154����
// release����ģʽ�£�ʹ�����ֽڿ�����16KB��ʱ11΢�룬ʹ��memcpy������ɣ���ʱ120���롣
//
void CVirtualDataSource::XferReadingToBuffer(UINT uByteRead) {
	ASSERT(m_sBuffer.size() >= m_lByteRead + uByteRead);
	memcpy(&m_sBuffer.at(m_lByteRead), m_dataBuffer, uByteRead);
}

bool CVirtualDataSource::IncreaseBufferSizeIfNeeded(long lIncreaseSize) {
	if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) {
		// ���ݿɴ洢�ռ䲻��128Kʱ
		m_sBuffer.resize(m_sBuffer.size() + lIncreaseSize); // ����lSize��Ĭ��Ϊ1M�����ݷ�Χ
	}
	return true;
}

CWebDataPtr CVirtualDataSource::CreateWebData() {
	const auto pWebData = make_shared<CWebData>();
	pWebData->ResetCurrentPos();
	pWebData->SetTime(GetUTCTime());
	TransferDataToWebData(pWebData); // �����յ�������ת����pWebData�С�����ʹ��std::move���ӿ��ٶȣ�Դ���ݲ����ٱ�ʹ�á�
	UpdateStatus(pWebData);

	return pWebData;
}

void CVirtualDataSource::VerifyDataLength() const {
	if (m_lContentLength > 0) {
		if (m_lContentLength != m_lByteRead) {
			CString str = _T("�������ݳ��Ȳ�����Ԥ�ڳ��ȣ�");
			char buffer[100];
			sprintf_s(buffer, _T("%d"), m_lContentLength);
			str += buffer;
			str += _T("��ʵ�ʳ��ȣ�");
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
	ASSERT(m_sBuffer.size() > m_lByteRead); // Note ��ʹ֪�������ܳ��ȣ�ҲҪ�����һ���ֽ��Է�ֹԽ�硣
	m_sBuffer.resize(m_lByteRead); // Note ��������СΪʵ��������
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // ʹ��std::move�Լ���ִ���ٶ�
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
