#pragma once

#include"afxinet.h"

#include"InternetOption.h"

#include"concurrentqueue/concurrentqueue.h"
using namespace moodycamel;

constexpr auto WEB_SOURCE_DATA_BUFFER_SIZE_ = 1024 * 16;

class CInquireEngine {
	friend CVirtualWebProduct;

public:
	CInquireEngine();
	CInquireEngine(const InternetOption& internetOption, const string& strInquire, const string& strHeaders);
	virtual ~CInquireEngine() {
	}

	void ConfigureSession(const InternetOption& option) const;
	CWebDataPtr GetWebData(); // ʵ�ʶ�ȡ������
	virtual void OpenFile(); // �޷����ԣ��ʶ����⻯��ʹ��Mock�������ԡ�
	virtual void GetFileHeaderInformation(); // �޷����ԣ��ʶ����⻯��ʹ��Mock�������ԡ�
	void DeleteWebFile();
	void QueryDataLength();
	virtual UINT ReadWebFileOneTime(); // �޷����ԣ��ʶ����⻯��ʹ��Mock�������ԡ�
	void XferReadingToBuffer(UINT uByteRead);
	bool IncreaseBufferSizeIfNeeded(long lIncreaseSize);
	CWebDataPtr CreateWebData();

	void VerifyDataLength() const;
	void TransferDataToWebData(const CWebDataPtr& pWebData); // �����յ���������pWebData��

	long GetByteRead() const noexcept { return m_lByteRead; }
	void SetByteRead(long number) { m_lByteRead = number; } // �����ڲ�����

	// ��״̬
	void SetInquiryString(const string& strInquiry) { m_strInquiry = strInquiry; }
	void SetInquiryHeader(const string& strHeader) { m_strHeaders = strHeader; }

	bool IsWebError() const noexcept { return m_dwErrorCode != 0; }
	DWORD GetErrorCode() const noexcept { return m_dwErrorCode; }

	DWORD GetHTTPStatusCode() const noexcept { return m_dwHTTPStatusCode; }

	void SetContentLength(long length) noexcept { m_lContentLength = length; }
	long GetContentLength() const noexcept { return m_lContentLength; } // �����ڲ�����
	void SetBufferSize(long size) { m_sBuffer.resize(size); }
	size_t GetBufferSize() const noexcept { return m_sBuffer.size(); } // �����ڲ�����

public:
	// ����Ϊ�����ú���
	void TESTSetBuffer(const char* buffer, INT64 lTotalNumber);
	void TESTSetBuffer(string str);
	void TESTSetWebBuffer(const char* buffer, INT64 lTotalNumber);

protected:
	shared_ptr<CInternetSession> m_pSession{ nullptr }; // ��ʼ��ʱ��������ָ�롣
	CHttpFile* m_pFile{ nullptr }; // �����ļ�ָ��
	DWORD m_dwHTTPStatusCode{ 0 }; //����״̬�롣�޴���ʱС��200.
	DWORD m_dwErrorCode{ 0 }; // ��������롣�޴���ʱΪ�㡣

	string m_sBuffer{}; // ���յ����ݵĻ�����
	long m_lByteRead{ 0 }; // ���յ����ַ���.

	string m_strInquiry{ _T("") }; // ��ѯ������ַ�����m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	string m_strHeaders{ _T("") };

	long m_lContentLength{ 0 }; // Ԥ�ڵ��������ݳ��ȣ�ʹ��QueryInfo(HTTP_QUERY_CONTENT_LENGTH)�õ������ݣ�

private:
	char m_dataBuffer[WEB_SOURCE_DATA_BUFFER_SIZE_]; //�������ݻ���
};

using CInquireEnginePtr = shared_ptr<CInquireEngine>;
