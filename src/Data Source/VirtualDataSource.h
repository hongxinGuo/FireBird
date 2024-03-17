#pragma once

#include"afxinet.h"

#include"concurrentqueue/concurrentqueue.h"
using namespace moodycamel;

#include"VirtualWebProduct.h"

using std::counting_semaphore;
using std::list;
using std::atomic_bool;
using std::atomic_long;
using std::atomic_int64_t;

constexpr auto WEB_SOURCE_DATA_BUFFER_SIZE_ = 1024 * 16;

class CVirtualDataSource : public std::enable_shared_from_this<CVirtualDataSource> {
	friend CVirtualWebProduct;

public:
	CVirtualDataSource();
	// ֻ����һ��ʵ��,������ֵ��
	CVirtualDataSource(const CVirtualDataSource&) = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&) = delete;
	CVirtualDataSource(const CVirtualDataSource&&) noexcept = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&&) noexcept = delete;
	virtual ~CVirtualDataSource() = default;

	std::shared_ptr<CVirtualDataSource> GetShared() { return shared_from_this(); }

	virtual bool Reset() { return true; }

	void Run2(long lMarketTime);
	void RunWorkingThread(long lMarketTime);
	void Run(long lMarketTime);
	virtual bool GenerateInquiryMessage(const long) { return true; } // �̳������ʵ�ָ��ԵĲ�ѯ����. ����Ϊ��ǰ�г�ʱ�䣨hhmmss��
	void GetWebDataAndProcessIt();
	virtual void GenerateCurrentInquiryMessage();
	virtual void GetWebData() { GetWebDataImp(); } // �����ȡ��Ϊ��Mock���㣬����Ϊ�麯�������е�Mock�࣬��ӦMock�˺����Է�ֹ����ʵ����������
	virtual void ProcessWebDataReceived();
	virtual void CheckInaccessible(const CWebDataPtr&) const {
	}

	void SetDefaultSessionOption() const;

	void GetWebDataImp(); // ʵ�ʶ�ȡ��������GeeWebData()��ʵ�֣���ɹ����̵߳�ʵ�ʹ���
	virtual void ReadWebData(); // ����ʵ�ʶ�ȡ����
	virtual void OpenFile(const CString& strInquiring);
	virtual void GetFileHeaderInformation();
	void DeleteWebFile();
	void QueryDataLength();
	virtual UINT ReadWebFileOneTime(); // �޷����ԣ��ʶ����⻯��ʹ��Mock�ࡣ
	void XferReadingToBuffer(UINT uByteRead);
	bool IncreaseBufferSizeIfNeeded(long lIncreaseSize);
	virtual CWebDataPtr CreateWebData();

	void VerifyDataLength() const;
	static void SetDataTime(const CWebDataPtr& pData, const time_t time) noexcept { pData->SetTime(time); }
	void TransferDataToWebData(const CWebDataPtr& pWebData); // �����յ���������pWebData��

	virtual void PrepareReadingWebData(); // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����m_pSession״̬��
	virtual void ConfigureSession() {
		ASSERT(false); // �����˻��ຯ��ConfigureSession
	} // ����m_pSession���̳������ʵ�ִ˹��ܣ�ÿ����վ��״̬����һ�����ʶ���Ҫ�������á�
	virtual void UpdateStatus(CWebDataPtr pData) {
	} //�ɹ����պ����ϵͳ״̬��Ĭ���޶���

	void CreateTotalInquiringString();
	CString GetInquiringString() const noexcept { return m_strInquiry; }
	void SetInquiringString(const CString& str) noexcept { m_strInquiry = str; }
	void AppendInquiringString(const CString& str) noexcept { m_strInquiry += str; }

	long GetByteRead() const noexcept { return m_lByteRead; }
	void SetByteRead(const long lValue) noexcept { m_lByteRead = lValue; }
	void SetBufferSize(int size) { m_sBuffer.resize(size); }
	size_t GetBufferSize() const noexcept { return m_sBuffer.size(); }
	string_view GetBuffer() noexcept { return m_sBuffer; }

	virtual ULONGLONG GetTickCount() { return ::GetTickCount64(); } // Ϊ�˲��Է��㣬��GetTickCount64������һ�㡣

	// ��״̬
	CVirtualProductWebDataPtr GetCurrentInquiry() const noexcept { return m_pCurrentProduct; }
	void SetCurrentInquiry(const CVirtualProductWebDataPtr& p) { m_pCurrentProduct = p; }

	size_t InquiryQueueSize() const noexcept { return m_qProduct.size(); }
	void StoreInquiry(const CVirtualProductWebDataPtr& p) { m_qProduct.push(p); }
	CVirtualProductWebDataPtr GetCurrentProduct() {
		m_pCurrentProduct = m_qProduct.front();
		m_qProduct.pop();
		return m_pCurrentProduct;
	}
	bool HaveInquiry() const { return !m_qProduct.empty(); }
	void DiscardAllInquiry() { while (m_qProduct.size() > 0) m_qProduct.pop(); }

	void StoreReceivedData(const CWebDataPtr& pData) noexcept { m_qReceivedData.enqueue(pData); }
	CWebDataPtr GetReceivedData() noexcept {
		CWebDataPtr pData = nullptr;
		m_qReceivedData.try_dequeue(pData);
		return pData;
	}
	size_t GetReceivedDataSize() const noexcept { return m_qReceivedData.size_approx(); }
	bool HaveReceivedData() const { return m_qReceivedData.size_approx() > 0; }
	void DiscardReceivedData() {
		CWebDataPtr pData;
		while (m_qReceivedData.try_dequeue(pData));
	}

	bool IsInquiring() const noexcept { return m_fInquiring; }
	void SetInquiring(const bool fFlag) noexcept { m_fInquiring = fFlag; }

	bool IsWorkingThreadRunning() const noexcept { return m_bIsWorkingThreadRunning; }
	void SetWorkingThreadRunning(const bool fFlag) noexcept { m_bIsWorkingThreadRunning = fFlag; }

	bool IsEnable() const noexcept { return m_fEnable; }
	void Enable(const bool fFlag) noexcept { m_fEnable = fFlag; }

	CString GetHeaders() const noexcept { return m_strHeaders; }
	void SetHeaders(const CString& strHeaders) noexcept { m_strHeaders = strHeaders; }

	CString GetInquiryFunction() const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(const CString& strPrefix) noexcept { m_strInquiryFunction = strPrefix; }
	CString GetInquirySuffix() const noexcept { return m_strSuffix; }
	void SetInquirySuffix(const CString& strSuffix) noexcept { m_strSuffix = strSuffix; }
	CString GetInquiryToken() const noexcept { return m_strInquiryToken; }
	void SetInquiryToken(const CString& strToken) noexcept { m_strInquiryToken = strToken; }

	bool IsWebError() const noexcept { return m_fWebError; }
	void SetWebError(bool fFlag) noexcept { m_fWebError = fFlag; }

	long GetInquiringNumber() const noexcept { return m_lInquiringNumber; }
	void SetInquiringNumber(const long lValue) noexcept { m_lInquiringNumber = lValue; }

	static long GetTotalByteRead() noexcept { return sm_lTotalByteRead; }
	static void SetTotalByteRead(const long lValue) noexcept { sm_lTotalByteRead = lValue; }
	static void ClearTotalByteRead() noexcept { sm_lTotalByteRead = 0; }
	static long GetTotalByteReadPerSecond() noexcept { return sm_lTotalByteReadPerSecond; }
	static void CalcTotalBytePerSecond();

	void SetCurrentInquiryTime(const time_t tt) noexcept { m_tCurrentInquiryTime = tt; }
	virtual time_t GetCurrentInquiryTime() const noexcept { return m_tCurrentInquiryTime; }

	void SetContentLength(long length) noexcept { m_lContentLength = length; }

public:
	// ����Ϊ�����ú���
	void TESTSetBuffer(const char* buffer, INT64 lTotalNumber);
	void TESTSetBuffer(CString str);
	void TESTSetWebBuffer(const char* buffer, INT64 lTotalNumber);

protected:
	queue<CVirtualProductWebDataPtr, list<CVirtualProductWebDataPtr>> m_qProduct; // �����ѯ�������
	CVirtualProductWebDataPtr m_pCurrentProduct;
	ConcurrentQueue<CWebDataPtr> m_qReceivedData; // ���������ݴ����

	shared_ptr<CInternetSession> m_pSession;
	CHttpFile* m_pFile; // �����ļ�ָ��
	DWORD m_dwHTTPStatusCode; //����״̬��

	CString m_strHeaders; // OpenURLʱ��headers�ַ���ֵ�� Ĭ��Ϊ_T("")
	bool m_fWebError; //�����ȡ����
	string m_sBuffer; // ���յ����ݵĻ�����
	long m_lByteRead; // ���յ����ַ���.

	CString m_strInquiry; // ��ѯ������ַ�����m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	CString m_strInquiryFunction; // ��ѯ�ַ������ܲ���
	CString m_strParam; // ��ѯ�ַ����Ĳ���
	CString m_strSuffix; // ��ѯ�ַ����ĺ�׺����
	CString m_strInquiryToken; // ��ѯ�ַ�������

	long m_lContentLength; // Ԥ�ڵ��������ݳ��ȣ�ʹ��QueryInfo(HTTP_QUERY_CONTENT_LENGTH)�õ������ݣ�

	long m_lInquiringNumber; // ÿ�β�ѯ����
	atomic_int64_t m_tCurrentInquiryTime; // ��ǰ������������ʱ�䣨�Ժ���ƣ�

	static atomic_long sm_lTotalByteRead; // ��ǰ�����ȡ�ֽ��������е������ȡ�����޸Ĵ˱������ʶ�����Ϊ��̬��
	static atomic_long sm_lTotalByteReadPerSecond; // 

	long long m_llLastTimeTickCount;

	atomic_bool m_fEnable; // ����ִ�б�ʶ
	atomic_bool m_fInquiring;
	atomic_bool m_bIsWorkingThreadRunning;

private:
	char m_dataBuffer[WEB_SOURCE_DATA_BUFFER_SIZE_]; //�������ݻ���
};

using CVirtualDataSourcePtr = shared_ptr<CVirtualDataSource>;
