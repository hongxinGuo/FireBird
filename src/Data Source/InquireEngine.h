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

class CInquireEngine {
	friend CVirtualWebProduct;

public:
	CInquireEngine();
	CInquireEngine(const CString& strInquire, const CString& strHeaders);

	CWebDataPtr GetWebData(); // ʵ�ʶ�ȡ��������GeeWebData()��ʵ�֣���ɹ����̵߳�ʵ�ʹ���
	void ReadWebData(); // ����ʵ�ʶ�ȡ����
	void OpenFile();
	void GetFileHeaderInformation();
	void DeleteWebFile();
	void QueryDataLength();
	UINT ReadWebFileOneTime(); // �޷����ԣ��ʶ����⻯��ʹ��Mock�ࡣ
	void XferReadingToBuffer(UINT uByteRead);
	bool IncreaseBufferSizeIfNeeded(long lIncreaseSize);
	CWebDataPtr CreateWebData();

	void VerifyDataLength() const;
	void TransferDataToWebData(const CWebDataPtr& pWebData); // �����յ���������pWebData��

	long GetByteRead() const noexcept { return m_lByteRead; }

	// ��״̬
	CVirtualProductWebDataPtr GetCurrentInquiry() const noexcept { return m_pCurrentProduct; }
	void SetCurrentInquiry(const CVirtualProductWebDataPtr& p) { m_pCurrentProduct = p; }

	void SetInquiryString(const CString& strInquiry) { m_strInquiry = strInquiry; }
	void SetInquiryHeader(const CString& strHeader) { m_strHeaders = strHeader; }

	bool IsWebError() const noexcept { return m_dwWebErrorCode != 0; }
	DWORD GetErrorCode() const noexcept { return m_dwWebErrorCode; }
	void SetErrorCode(const DWORD dwErrorCode) noexcept { m_dwWebErrorCode = dwErrorCode; }
	bool IsTimeout() const noexcept { return m_dwWebErrorCode == 12002; }

	void SetCurrentInquiryTime(const time_t tt) noexcept { m_tCurrentInquiryTime = tt; }
	time_t GetCurrentInquiryTime() const noexcept { return m_tCurrentInquiryTime; }

	void SetContentLength(long length) noexcept { m_lContentLength = length; }

public:
	// ����Ϊ�����ú���
	void TESTSetBuffer(const char* buffer, INT64 lTotalNumber);
	void TESTSetBuffer(CString str);
	void TESTSetWebBuffer(const char* buffer, INT64 lTotalNumber);

protected:
	CVirtualProductWebDataPtr m_pCurrentProduct;

	shared_ptr<CInternetSession> m_pSession;
	CHttpFile* m_pFile; // �����ļ�ָ��
	DWORD m_dwHTTPStatusCode; //����״̬��

	DWORD m_dwWebErrorCode; //�����ȡ������롣Ҳ������������жϵ����ݣ���Ϊ��ʱ�޴���
	string m_sBuffer; // ���յ����ݵĻ�����
	long m_lByteRead; // ���յ����ַ���.

	CString m_strInquiry; // ��ѯ������ַ�����m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	CString m_strHeaders;

	long m_lContentLength; // Ԥ�ڵ��������ݳ��ȣ�ʹ��QueryInfo(HTTP_QUERY_CONTENT_LENGTH)�õ������ݣ�

	atomic_int64_t m_tCurrentInquiryTime; // ��ǰ������������ʱ�䣨�Ժ���ƣ�

	long long m_llLastTimeTickCount;

private:
	char m_dataBuffer[1024 * 16]; //�������ݻ���
};

using CDataInquireEnginePtr = shared_ptr<CInquireEngine>;
