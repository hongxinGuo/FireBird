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
	m_sBuffer.resize(DefaultWebDataBufferSize_); // ���������£�2M������㹻�ˣ������ٴη����ڴ档

	m_lInquiringNumber = 500; // ÿ�β�ѯ����Ĭ��ֵΪ500
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
/// DataSource�Ķ��㺯����
/// ��������ϢΪ��ʱ�����ɵ�ǰ��ѯ�ַ�����
/// ������������Ϣ��û���������еĲ�ѯ�߳�ʱ�����ɲ�ѯ�̡߳�
/// </summary>
/// <param name="lCurrentTime"����ǰ�г�ʱ��></param>
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
		SetGetWebDataAndProcessItThreadRunning(true); // �ڵ��ù����߳�ǰ�����ø�ֵ
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

	ASSERT(IsGetWebDataAndProcessItThreadRunning());// �ڵ��ù����߳�ǰ������
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
// ֻ����ͬʱ����һ����Ϣ������Ϣ��������������֮ǰ������������һ����Ϣ�������ܹ���֤ͬһ�ԡ�������Finnhub��վ���ٶ����ƣ�
// ÿ����ֻ����60�����룬�ʶ�û�б�Ҫǿ�������ٶȡ�
//
//
//////////////////////////////////////////////
bool CVirtualDataSource::ProcessWebDataReceived() {
	bool bProcessed = false;
	//ASSERT(HaveReceivedData());
	if (HaveReceivedData()) {
		// ����ǰ��������
		ASSERT(m_pCurrentProduct != nullptr);
		const CWebDataPtr pWebData = GetReceivedData();
		CheckInaccessible(pWebData);
		m_pCurrentProduct->ParseAndStoreWebData(pWebData);
		bProcessed = true;
	}
	ASSERT(IsInquiring()); // ִ�е���ʱ���в����������´ε����ݡ�
	if (!HaveInquiry()) { // û���ִ������ʱ
		SetInquiring(false); // �˱�ʶ��������Ҫλ��λ�����һ��
	}
	return bProcessed;
}

void CVirtualDataSource::CheckInaccessible(const CWebDataPtr& pWebData) const {
	ASSERT(m_pCurrentProduct != nullptr);
	if (m_pCurrentProduct->CheckInaccessible(pWebData)) {
		// ���ϵͳ������Ȩ��ѯ��������, Ŀǰ�������ϵͳ��Ϣ�б���
		gl_systemMessage.PushInnerSystemInformationMessage(_T("No right to access: ") + m_pCurrentProduct->GetInquiry() + _T(",  Exchange = ") + m_pCurrentProduct->GetInquiringExchange());
	}
}

void CVirtualDataSource::SetDefaultSessionOption() const {
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
		DiscardAllInquiry(); // ��һ�β�ѯ�����������ʱ����Щ���붼�Ǹ�����صģ�ֻҪ����һ�δ���������������������ˡ�
		DiscardReceivedData();
		ASSERT(IsInquiring());
		SetInquiring(false); // �������̳߳��ֹ���ʱ��ֱ���������������־��
	}

	return bSucceed;
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
void CVirtualDataSource::ReadWebData() {
	SetByteRead(0);

	ASSERT(m_pFile == nullptr);
	try {
		OpenFile(GetInquiringString());
		GetFileHeaderInformation();
		UINT lCurrentByteRead;
		do {
			if (gl_systemStatus.IsExitingSystem()) {// ��ϵͳ�˳�ʱ��Ҫ�����жϴ˽��̣��Է�ֹ�ڴ�й¶��
				break;
			}
			lCurrentByteRead = ReadWebFileOneTime(); // ÿ�ζ�ȡ1K���ݡ�
			XferReadingToBuffer(m_lByteRead, lCurrentByteRead);
			m_lByteRead += lCurrentByteRead;
			IncreaseBufferSizeIfNeeded();
		} while (lCurrentByteRead > 0);
		sm_lTotalByteRead += m_lByteRead;
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
// �����ô˺�����ȡ����������ʷ����ʱ��ż������ֳ�ʱ������������12002������
// Ŀǰ���������Ƕ�ȡfinnhub.ioʱ��������վ��ǽ���������Ӵ���--20220401
//
// ��������ʵʱ���ݷ�������Ҫ�ṩͷ����֤���ݣ��ʶ�OpenURL����ʹ��Ĭ��ֵ����������Ҫ��������m_strHeaders��Ĭ��Ϊ�գ���
// ������������δ��Ҫ�ṩͷ����֤���ݡ�
// ���ú�����Ҫ����exception��
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
// ÿ�ζ�ȡ16K���ݣ�Ȼ�󽫶�ȡ�������ݴ��뻺����
// �˺����ĺ�ʱ������Read�������ʶ��޷��ӿ�ִ���ٶ��ˡ�
//
////////////////////////////////////////////////////////////////////////////////////////////
UINT CVirtualDataSource::ReadWebFileOneTime() {
	const UINT uByteRead = m_pFile->Read(m_dataBuffer, DATA_BUFFER_SIZE_);
	return uByteRead;
}

//
// release����ģʽ�£�ʹ�����ֽڿ�����16KB��ʱ11΢�룬ʹ��memcpy������ɣ���ʱ120���롣
//
void CVirtualDataSource::XferReadingToBuffer(long lPosition, UINT uByteRead) {
	ASSERT((lPosition + uByteRead) < m_sBuffer.size());
	char* p = &m_sBuffer.at(lPosition);
	memcpy(p, m_dataBuffer, uByteRead);
}

bool CVirtualDataSource::IncreaseBufferSizeIfNeeded(long lIncreaseSize) {
	if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) {
		// ���ݿɴ洢�ռ䲻��128Kʱ
		m_sBuffer.resize(m_sBuffer.size() + lIncreaseSize); // ����lSize��Ĭ��Ϊ1M�����ݷ�Χ
	}
	return true;
}

CWebDataPtr CVirtualDataSource::CreateWebDataAfterSucceedReading() {
	const auto pWebData = make_shared<CWebData>();
	pWebData->ResetCurrentPos();
	pWebData->SetTime(GetUTCTime());
	TransferDataToWebData(pWebData); // �����յ�������ת����pWebData�С�����ʹ��std::move���ӿ��ٶȣ�Դ���ݲ����ٱ�ʹ�á�
	ParseData(pWebData);
	UpdateStatusAfterReading(pWebData);

	return pWebData;
}

void CVirtualDataSource::VerifyDataLength() const {
	const auto byteRead = GetByteRead();

	if (m_lContentLength > 0) {
		if (m_lContentLength != byteRead) {
			CString str = _T("�������ݳ��Ȳ�����Ԥ�ڳ��ȣ�");
			char buffer[100];
			sprintf_s(buffer, _T("%d"), m_lContentLength);
			str += buffer;
			str += _T("��ʵ�ʳ��ȣ�");
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
	pWebData->m_sDataBuffer = std::move(m_sBuffer); // ʹ��std::move�Լ���ִ���ٶ�
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
