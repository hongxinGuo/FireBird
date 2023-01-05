///////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ��������ʱʹ����MFC��CInternetSession��OpenURL���������ٶȳ���libcurl�е�curl_easy_perform()��
///	ͬʱ�������ɸ�����ʱ�������ܵ���������Ӱ�죬�����ٶȻụ��Ӱ�죬����ʱ���ӳ���������Tiingo��StockSymbolʱ��
///	˲�������ٶȻ�ﵽ70M bit/�룬 ����sinaʵʱ���ݵ�����ʱ���ɼ�ʮ���� �ӳ���300-700���롣�����Խ��ܡ�
/// ʹ��libcurl��curl_multi_perform���߳�ģʽ���������������ƣ����о�Ҳ�����MFC�ö��١�
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include "VirtualWebInquiry.h"
#include"ThreadStatus.h"
#include"Thread.h"

#include"InfoReport.h"
#include"HighPerformanceCounter.h"

#include"ChinaMarket.h"
#include"curl/curl.h"

#include<thread>
using std::thread;

atomic_long CVirtualWebInquiry::sm_lTotalByteRead = 0;

CVirtualWebInquiry::CVirtualWebInquiry() {
	m_pDataSource = nullptr;

	m_pSession = new CInternetSession(_T("StockAnalysis")); // �˴���Ҫ���ϵ��ó�������ƣ������޷����е�Ԫ���Գ���ԭ��������
	SetDefaultSessionOption();
	m_pFile = nullptr;
	m_strHeaders = _T("");

	m_lByteRead = 0;
	m_dwWebErrorCode = 0;
	m_strInquiry = _T("");
	m_strInquiryFunction = _T("");
	m_strInquiryToken = _T("");
	m_fReadingWebData = false; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
	m_sBuffer.resize(DefaultWebDataBufferSize_); // ���������£�2M������㹻�ˣ������ٴη����ڴ档

	m_lInquiringNumber = 500; // ÿ�β�ѯ����Ĭ��ֵΪ500
	m_tCurrentInquiryTime = 0;

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
}

/////////////////////////////////////////////////////////////////////////
//
// ���Ǵ���Ψһ�Ľӿں���
//
//////////////////////////////////////////////////////////////////////////
bool CVirtualWebInquiry::GetWebData(void) {
	ASSERT(m_pDataSource != nullptr);

	if (!IsReadingWebData()) {
		// �����߳�û��������
		if (PrepareNextInquiringString()) {
			SetReadingWebData(true); // �����������߳�ǰ�����ã��Է������߳��ӳٵ������롣
			StartReadingThread();
			return true;
		}
		else return false;
	}
	else return false; // �����߳�����ִ�н������ݵ�����
}

void CVirtualWebInquiry::PrepareReadingWebData(void) {
	ConfigureSession();
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

	ASSERT(IsReadingWebData());
	ASSERT(m_pDataSource != nullptr);
	PrepareReadingWebData();
	counter.start();
	if (ReadingWebData()) {
		const auto pWebData = make_shared<CWebData>();
		VerifyDataLength();
		TransferDataToWebData(pWebData); // �����յ�������ת����pWebData�С�����ʹ��std::move���ӿ��ٶȣ�Դ���ݲ����ٱ�ʹ�á�
		ResetBuffer();
		ParseData(pWebData);
		UpdateStatusAfterSucceed(pWebData);

		pWebData->SetTime(gl_pChinaMarket->GetUTCTime());
		m_pDataSource->StoreReceivedData(pWebData);
		ASSERT(m_pDataSource->IsInquiring());
	}
	else {
		// error handling
		while (m_pDataSource->GetReceivedDataSize() > 0) m_pDataSource->GetReceivedData();
		m_pDataSource->SetInquiring(false); // �������̳߳��ֹ���ʱ��ֱ���������������־��
	}
	m_pDataSource->SetWebInquiryFinished(true); // ���۳ɹ���񣬶�Ҫ���ô˱�ʶ
	counter.stop();
	SetCurrentInquiryTime(counter.GetElapsedMilliSecond());

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

	ASSERT(IsReadingWebData());
	gl_ThreadStatus.IncreaseWebInquiringThread();
	SetByteRead(0);

	ASSERT(m_pFile == nullptr);
	if (OpenFile(GetInquiringString())) {
		try {
			UINT lCurrentByteRead;
			do {
				if (gl_systemStatus.IsExitingSystem()) {
					// ��ϵͳ�˳�ʱ��Ҫ�����жϴ˽��̣��Է�ֹ�ڴ�й¶��
					fReadingSuccess = false;
					break;
				}
				lCurrentByteRead = ReadWebFileOneTime(); // ÿ�ζ�ȡ1K���ݡ�
				IncreaseBufferSizeIfNeeded();
			} while (lCurrentByteRead > 0);
			sm_lTotalByteRead += m_lByteRead;
			// �������������Ķ�����ֻ�ڴ˴����С��Ա�ֻ֤�е�˳����ȡ���������ݺ󣬷������֮ǰ�Ĵ����ʶ��
			m_dwWebErrorCode = 0; // ���������루����еĻ�����ֻ�ڴ˴����øô�����롣
		}
		catch (CInternetException* exception) {//����һ����ʹ�����á�����׼���ڴ�������ɾ����������ˣ��ʶ�ֱ��ʹ��ָ�롣��������ϵͳ����������⣬�ᵼ�³����Զ��˳���  // NOLINT(misc-throw-by-value-catch-by-reference)
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
bool CVirtualWebInquiry::OpenFile(const CString& strInquiring) {
	bool fSucceedOpen = true;
	const long lHeadersLength = m_strHeaders.GetLength();

	const ULONG64 llCurrentTickCount = GetTickCount64();
	ASSERT(m_pSession != nullptr);
	ASSERT(m_pFile == nullptr);
	try {
		// ��������ʵʱ���ݷ�������Ҫ�ṩͷ����֤���ݣ��ʶ�OpenURL����ʹ��Ĭ��ֵ����������Ҫ��������m_strHeaders��Ĭ��Ϊ�գ���
		// ������������δ��Ҫ�ṩͷ����֤���ݡ�
		if (gl_systemStatus.IsExitingSystem()) { return false; }
		//CStdioFile* file = m_pSession->OpenURL((LPCTSTR)strInquiring, 1, INTERNET_FLAG_TRANSFER_ASCII, (LPCTSTR)m_strHeaders, lHeadersLength);
		//auto p = typeid(*file).name();
		//ASSERT(std::strcmp(typeid(*file).name(), _T("class CHttpFile")) == 0);
		//m_pFile = dynamic_cast<CHttpFile*>(file);
		m_pFile = static_cast<CHttpFile*>(m_pSession->OpenURL((LPCTSTR)strInquiring, 1, INTERNET_FLAG_TRANSFER_ASCII, (LPCTSTR)m_strHeaders, lHeadersLength));
		ASSERT(std::strcmp(typeid(*m_pFile).name(), _T("class CHttpFile")) == 0);
	}
	catch (CInternetException* exception) { //����һ����ʹ�����á�����׼���ڴ�������ɾ����������ˣ��ʶ�ֱ��ʹ��ָ�롣��������ϵͳ����������⣬�ᵼ�³����Զ��˳���  // NOLINT(misc-throw-by-value-catch-by-reference)
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
		char* p;
		m_lContentLength = strtol(str.GetBuffer(), &p, 10);
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
	char buffer[1024 * 16];
	const UINT uByteRead = m_pFile->Read(buffer, 1024 * 16);
	for (UINT i = 0; i < uByteRead; i++) {
		m_sBuffer.at(m_lByteRead++) = buffer[i];
	}

	return uByteRead;
}

bool CVirtualWebInquiry::IncreaseBufferSizeIfNeeded(long lIncreaseSize) {
	if (m_sBuffer.size() < (m_lByteRead + 128 * 1024)) {
		// ���ݿɴ洢�ռ䲻��128Kʱ
		m_sBuffer.resize(m_sBuffer.size() + lIncreaseSize); // ����lSize��Ĭ��Ϊ1M�����ݷ�Χ
	}
	return true;
}

bool CVirtualWebInquiry::VerifyDataLength() const {
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
			str += m_strInquiry.Left(120);
			gl_systemMessage.PushErrorMessage(str);
			str = m_sBuffer.c_str();
			str = str.Left(200);
			gl_systemMessage.PushErrorMessage(str);
		}
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// ʹ��libcurl��curl_easy_perform()��ȡ�������ݵ�ģʽ��
///	��Windows�Դ���CHttpFile��ȡģʽ��ȣ�û���κ��ٶ��ϵ���ߣ��ҽ����˲��١�
///	��ʹ�ô�ģʽ�������ڴ˽�Ϊ���ա�
///
/////////////////////////////////////////////////////////////////////////////////////////////////
void CVirtualWebInquiry::Read2(void) {
	ASSERT(0); // ��ģʽ�ٶȽ����˲��٣���ʹ�ô�ģʽ��������ԭ�������ڶԱȡ�
	ASSERT(IsReadingWebData());
	ASSERT(m_pDataSource != nullptr);
	m_pWebData = make_shared<CWebData>();
	PrepareReadingWebData();
	if (ReadingWebData2()) {
		ResetBuffer();
		ParseData(m_pWebData);
		UpdateStatusAfterSucceed(m_pWebData);

		m_pWebData->SetTime(gl_pChinaMarket->GetUTCTime());
		m_pDataSource->StoreReceivedData(m_pWebData);
		ASSERT(m_pDataSource->IsInquiring());
	}
	else {
		// error handling
		while (m_pDataSource->GetReceivedDataSize() > 0) m_pDataSource->GetReceivedData();
		m_pDataSource->SetInquiring(false); // �������̳߳��ֹ���ʱ��ֱ���������������־��
	}
	m_pDataSource->SetWebInquiryFinished(true); // ���۳ɹ���񣬶�Ҫ���ô˱�ʶ

	SetReadingWebData(false);
}

static size_t receive_data_func(void* ptr, size_t size, size_t nmemb, void* userOp) {
	const size_t realSize = size * nmemb;

	(static_cast<CWebData*>(userOp))->StoreData(static_cast<char*>(ptr), realSize);

	return realSize;
}

bool CVirtualWebInquiry::ReadingWebData2(void) {
	gl_ThreadStatus.IncreaseWebInquiringThread();
	m_pWebData = make_shared<CWebData>();

	CURL* curl_handle = curl_easy_init();
	CURLcode code;
	if (curl_handle) {
		curl_easy_setopt(curl_handle, CURLOPT_URL, m_strInquiry);
		if (m_strHeaders.GetLength() > 0) {
			struct curl_slist* list = nullptr;
			list = curl_slist_append(list, m_strHeaders);
			curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, list); // �ύ��ͷ
			curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1); // �رս�����
			//curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 60); // TimeOut
			//curl_easy_setopt(curl_handle, CURLOPT_MAX_RECV_SPEED_LARGE, static_cast<curl_off_t>(2 * 512 * 1024)); // ��������ٶ�Ϊ2M Bytes/S
			code = curl_easy_setopt(curl_handle, CURLOPT_BUFFERSIZE, 1024 * 512);
		}
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0); // ��У��SSL
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, m_pWebData.get());
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, receive_data_func);
	}
	CURLcode res = curl_easy_perform(curl_handle);
	double d = 0.0;
	curl_easy_getinfo(curl_handle, CURLINFO_TOTAL_TIME, &d);
	m_tCurrentInquiryTime = d * 1000;
	curl_easy_cleanup(curl_handle);
	m_pWebData->Resize(m_pWebData->GetCurrentPos());
	sm_lTotalByteRead += m_pWebData->GetCurrentPos();
	m_pWebData->SetCurrentPos(0);
	// �������������Ķ�����ֻ�ڴ˴����С��Ա�ֻ֤�е�˳����ȡ���������ݺ󣬷������֮ǰ�Ĵ����ʶ��
	m_dwWebErrorCode = 0; // ���������루����еĻ�����ֻ�ڴ˴����øô�����롣

	gl_ThreadStatus.DecreaseWebInquiringThread();

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

void CVirtualWebInquiry::CreateTotalInquiringString(CString strMiddle) { m_strInquiry = m_strInquiryFunction + strMiddle + m_strInquiryToken; }

void CVirtualWebInquiry::TESTSetBuffer(char* buffer, INT64 lTotalNumber) {
	m_sBuffer.resize(lTotalNumber);
	for (INT64 i = 0; i < lTotalNumber; i++) { m_sBuffer.at(i) = buffer[i]; }
	m_lByteRead = lTotalNumber;
}

void CVirtualWebInquiry::TESTSetBuffer(CString str) {
	const INT64 lTotalNumber = str.GetLength();
	const char* buffer = str.GetBuffer();

	m_sBuffer.resize(lTotalNumber);
	for (INT64 i = 0; i < lTotalNumber; i++) { m_sBuffer.at(i) = buffer[i]; }
	m_lByteRead = lTotalNumber;
}
