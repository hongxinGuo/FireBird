#pragma once

#include"afxinet.h"

#include"TemplateMutexAccessQueue.h"

#include"VirtualWebProduct.h"

#include<semaphore>
#include<list>

using std::counting_semaphore;
using std::list;
using std::atomic_bool;
using std::atomic_long;

constexpr auto DATA_BUFFER_SIZE_ = 1024 * 16;

// ���ź������ڽ���WebSource�е����ݡ�
// ��ParseAndStoreData�߳����������3�����������ܱ�֤�㹻�ļ����ٶȣ�Ҳ���ᷢ��ϵͳ����������Ϊ4��ʱ�����ܹ۲쵽ϵͳ������
extern counting_semaphore<3> gl_WebSourceParseAndStoreData;

class CVirtualDataSource {
public:
	CVirtualDataSource(void);
	// ֻ����һ��ʵ��,������ֵ��
	CVirtualDataSource(const CVirtualDataSource&) = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&) = delete;
	CVirtualDataSource(const CVirtualDataSource&&) noexcept = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&&) noexcept = delete;
	virtual ~CVirtualDataSource(void) = default;

	virtual bool Reset(void);

	void Run(long lCurrentTime);
	virtual bool GenerateInquiryMessage(const long) { return true; } // �̳���ʵ�ָ��ԵĲ�ѯ����. ����Ϊ��ǰ�г�ʱ�䣨hhmmss��
	virtual bool GetWebData(void);
	virtual bool ProcessWebDataReceived(void);
	virtual void ParseAndStoreData(CVirtualProductWebDataPtr pProductWebData, CWebDataPtr pWebData); // Ĭ�����ڴ����걾�����ݺ󷽲������ٴν��ա�
	virtual bool UpdateStatus(void) {
		ASSERT(0);
		return true;
	}

	bool HaveInquiry(void) const {
		if (m_qProduct.empty()) return false;
		return true;
	}

	size_t GetInquiryQueueSize(void) const noexcept { return m_qProduct.size(); }
	void StoreInquiry(const CVirtualProductWebDataPtr& p) { m_qProduct.push(p); }

	CVirtualProductWebDataPtr GetCurrentProduct(void) {
		m_pCurrentProduct = m_qProduct.front();
		m_qProduct.pop();
		return m_pCurrentProduct;
	}

	CVirtualProductWebDataPtr GetCurrentInquiry(void) const noexcept { return m_pCurrentProduct; }
	void SetCurrentInquiry(const CVirtualProductWebDataPtr& p) { m_pCurrentProduct = p; }

	bool IsInquiring(void) const noexcept { return m_fInquiring; }
	void SetInquiring(const bool fFlag) noexcept { m_fInquiring = fFlag; }

	bool IsInquiringAndClearFlag(void) noexcept {
		const bool fInquiring = m_fInquiring.exchange(false);
		return fInquiring;
	}

	void StoreReceivedData(const CWebDataPtr pData) noexcept { m_qReceivedData.PushData(pData); }
	CWebDataPtr GetReceivedData(void) noexcept { return m_qReceivedData.PopData(); }
	size_t GetReceivedDataSize(void) noexcept { return m_qReceivedData.Size(); }

	bool HaveReceivedData(void) {
		if (m_qReceivedData.Empty()) return false;
		return true;
	}

	bool IsEnable(void) const noexcept { return m_fEnable; }
	void Enable(const bool fFlag) noexcept { m_fEnable = fFlag; }

	// ����ΪVirtualWebInquiry�ĺ���
	void SetDefaultSessionOption(void);

	virtual bool ReadingWebData(void); // ����ʵ�ʶ�ȡ����
	virtual void OpenFile(const CString& strInquiring);
	virtual void GetFileHeaderInformation();
	void DeleteWebFile();
	long QueryDataLength();
	virtual UINT ReadWebFileOneTime(void); // �޷����ԣ��ʶ����⻯��ʹ��Mock�ࡣ
	void XferReadingToBuffer(long lPosition, UINT uByteRead);
	bool IncreaseBufferSizeIfNeeded(long lIncreaseSize = 1024 * 1024);

	void VerifyDataLength() const;
	virtual bool TransferDataToWebData(CWebDataPtr pWebData); // �����յ���������pWebData��
	virtual bool ParseData(CWebDataPtr pWebData) {
		TRACE("�����˻��ຯ��\n");
		return false;
	} //�������յ������ݡ��̳������ʵ�ִ˺�����
	void ResetBuffer(void) { m_sBuffer.resize(DefaultWebDataBufferSize_); }

	// Ψһ�Ĺ����ӿں���
	virtual void ProcessInquiryMessage(void);

	void Read(void); // ʵ�ʶ�ȡ����������ɹ����̵߳�ʵ�ʹ���

	// ����Ϊ�̳������ʵ�ֵļ������ܺ�������ɾ������񡣲�������ñ����ຯ��
	// ���ڲ��Ե�ԭ�򣬴˴������˺������壬û�н�������Ϊ=0.
	virtual void CreateInquiryMessageFromCurrentProduct(void);

	virtual void PrepareReadingWebData(void); // �ڶ�ȡ��������ǰ��׼��������Ĭ��Ϊ����m_pSession״̬��
	virtual void ConfigureSession(void) {
		TRACE("�����˻��ຯ��ConfigureSession\n");
	} // ����m_pSession���̳������ʵ�ִ˹��ܣ�ÿ����վ��״̬����һ�����ʶ���Ҫ�������á�
	virtual void StartReadingThread(void); // ���������ȡ�̡߳�Ϊ��Mock���㣬����Ϊ�麯����
	virtual void UpdateStatusAfterSucceed(CWebDataPtr pData) {} //�ɹ����պ����ϵͳ״̬��Ĭ���޶���

	void CreateTotalInquiringString();
	CString GetInquiringString(void) const noexcept { return m_strInquiry; }
	void SetInquiringString(const CString& str) noexcept { m_strInquiry = str; }
	void AppendInquiringString(const CString& str) noexcept { m_strInquiry += str; }

	CString GetHeaders(void) const noexcept { return m_strHeaders; }
	void SetHeaders(const CString& strHeaders) noexcept { m_strHeaders = strHeaders; }

	char GetData(const long lIndex) const { return m_sBuffer.at(lIndex); }
	void SetData(const long lIndex, const char value) { m_sBuffer.at(lIndex) = value; }

	long GetByteRead(void) const noexcept { return m_lByteRead; }
	void SetByteRead(const long lValue) noexcept { m_lByteRead = lValue; }
	void AddByteRead(const long lValue) noexcept { m_lByteRead += lValue; }
	size_t GetBufferSize(void) const noexcept { return m_sBuffer.size(); }

	CString GetInquiryFunction(void) const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(const CString& strPrefix) noexcept { m_strInquiryFunction = strPrefix; }
	CString GetInquirySuffix(void) const noexcept { return m_strSuffix; }
	void SetInquirySuffix(const CString& strSuffix) noexcept { m_strSuffix = strSuffix; }
	CString GetInquiryToken(void) const noexcept { return m_strInquiryToken; }
	void SetInquiryToken(const CString& strToken) noexcept { m_strInquiryToken = strToken; }

	bool IsInquiringWebData(void) const noexcept { return m_fInquiringWebData; }
	void SetInquiringWebData(const bool fFlag) noexcept { m_fInquiringWebData = fFlag; }

	bool IsWebError(void) const noexcept { return m_dwWebErrorCode > 0; }
	DWORD GetErrorCode(void) const noexcept { return m_dwWebErrorCode; }
	void SetErrorCode(const DWORD dwErrorCode) noexcept { m_dwWebErrorCode = dwErrorCode; }

	bool IsTimeout(void) const noexcept {
		if (m_dwWebErrorCode == 12002) return true;
		return false;
	}

	long GetInquiringNumber(void) const noexcept { return m_lInquiringNumber; }
	void SetInquiringNumber(const long lValue) noexcept { m_lInquiringNumber = lValue; }

	static long GetTotalByteRead(void) noexcept { return sm_lTotalByteRead; }
	static void SetTotalByteRead(const long lValue = 0) noexcept { sm_lTotalByteRead = lValue; }
	static void ClearTotalByteRead(void) noexcept { sm_lTotalByteRead = 0; }

	void SetCurrentInquiryTime(const time_t tt) noexcept { m_tCurrentInquiryTime = tt; }
	time_t GetCurrentInquiryTime(void) const noexcept { return m_tCurrentInquiryTime; }

	void DiscardProduct() { while (m_qProduct.size() > 0) m_qProduct.pop(); }
	void DiscardReceivedData() { while (GetReceivedDataSize() > 0) GetReceivedData(); }

public:
	// ����Ϊ�����ú���
	void TESTSetBuffer(char* buffer, INT64 lTotalNumber);
	void TESTSetBuffer(CString str);
	void TESTSetWebBuffer(char* buffer, INT64 lTotalNumber);

protected:
	queue<CVirtualProductWebDataPtr, list<CVirtualProductWebDataPtr>> m_qProduct; // �����ѯ�������
	CVirtualProductWebDataPtr m_pCurrentProduct;
	atomic_bool m_fInquiring;
	atomic_bool m_fInquiringWebData; // ����ʵʱ�����߳��Ƿ�ִ�б�ʶ
	CTemplateMutexAccessQueue<CWebData> m_qReceivedData; // ���������ݴ����

	bool m_fEnable; // ����ִ�б�ʶ

	shared_ptr<CInternetSession> m_pSession;
	CHttpFile* m_pFile; // �����ļ�ָ��
	DWORD m_dwHTTPStatusCode; //����״̬��

	CString m_strHeaders; // OpenURLʱ��headers�ַ���ֵ�� Ĭ��Ϊ_T("")
	DWORD m_dwWebErrorCode; //�����ȡ������롣Ҳ������������жϵ����ݣ���Ϊ��ʱ�޴���
	string m_sBuffer; // ���յ����ݵĻ�����
	long m_lByteRead; // ���յ����ַ���.

	CString m_strInquiry; // ��ѯ������ַ�����m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	CString m_strInquiryFunction; // ��ѯ�ַ������ܲ���
	CString m_strParam; // ��ѯ�ַ����Ĳ���
	CString m_strSuffix; // ��ѯ�ַ����ĺ�׺����
	CString m_strInquiryToken; // ��ѯ�ַ�������

	long m_lContentLength; // Ԥ�ڵ��������ݳ��ȣ�ʹ��QueryInfo(HTTP_QUERY_CONTENT_LENGTH)�õ������ݣ�

	long m_lInquiringNumber; // ÿ�β�ѯ����
	time_t m_tCurrentInquiryTime; // ��ǰ������������ʱ�䣨�Ժ���ƣ�

	static atomic_long sm_lTotalByteRead; // ��ǰ�����ȡ�ֽ��������е������ȡ�����޸Ĵ˱������ʶ�����Ϊ��̬��

private:
	char m_dataBuffer[DATA_BUFFER_SIZE_]; //�������ݻ���
};

using CVirtualDataSourcePtr = shared_ptr<CVirtualDataSource>;
