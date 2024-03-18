#pragma once

#include"afxinet.h"

struct InternetOption {
public:
	int option_connect_retries{1};
	int option_connect_timeout{5};
	int option_receive_timeout{5000};
	int option_data_receive_timeout{12000};
	int option_send_timeout{2000};
};

#include"concurrentqueue/concurrentqueue.h"
using namespace moodycamel;

using std::atomic_int64_t;

constexpr auto WEB_SOURCE_DATA_BUFFER_SIZE_ = 1024 * 16;

class CInquireEngine {
	friend CVirtualWebProduct;

public:
	CInquireEngine();
	CInquireEngine(const CString& strInquire, const CString& strHeaders);

	void ConfigureSession(const InternetOption& option);
	CWebDataPtr GetWebData(); // ʵ�ʶ�ȡ������
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
	void SetInquiryString(const CString& strInquiry) { m_strInquiry = strInquiry; }
	void SetInquiryHeader(const CString& strHeader) { m_strHeaders = strHeader; }

	bool IsWebError() const noexcept { return m_fWebError; }
	void SetWebError(bool fFlag) noexcept { m_fWebError = fFlag; }

	void SetContentLength(long length) noexcept { m_lContentLength = length; }

	void SetBufferSize(long size) { m_sBuffer.resize(size); }

public:
	// ����Ϊ�����ú���
	void TESTSetBuffer(const char* buffer, INT64 lTotalNumber);
	void TESTSetBuffer(CString str);
	void TESTSetWebBuffer(const char* buffer, INT64 lTotalNumber);

protected:
	shared_ptr<CInternetSession> m_pSession;
	CHttpFile* m_pFile; // �����ļ�ָ��
	DWORD m_dwHTTPStatusCode; //����״̬��

	bool m_fWebError; //�����ȡ������롣Ҳ������������жϵ����ݣ���Ϊ��ʱ�޴���
	string m_sBuffer; // ���յ����ݵĻ�����
	long m_lByteRead; // ���յ����ַ���.

	CString m_strInquiry; // ��ѯ������ַ�����m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	CString m_strHeaders;

	long m_lContentLength; // Ԥ�ڵ��������ݳ��ȣ�ʹ��QueryInfo(HTTP_QUERY_CONTENT_LENGTH)�õ������ݣ�

private:
	char m_dataBuffer[WEB_SOURCE_DATA_BUFFER_SIZE_]; //�������ݻ���
};

using CDataInquireEnginePtr = shared_ptr<CInquireEngine>;
