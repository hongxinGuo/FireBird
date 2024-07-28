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

constexpr auto WEB_SOURCE_DATA_BUFFER_SIZE_ = 1024 * 16;

class CInquireEngine {
	friend CVirtualWebProduct;

public:
	CInquireEngine();
	CInquireEngine(const InternetOption& internetOption, const CString& strInquire, const CString& strHeaders);
	virtual ~CInquireEngine() {
	}

	void ConfigureSession(const InternetOption& option) const;
	CWebDataPtr GetWebData(); // 实际读取处理函数
	virtual void OpenFile(); // 无法测试，故而虚拟化后使用Mock类来测试。
	virtual void GetFileHeaderInformation(); // 无法测试，故而虚拟化后使用Mock类来测试。
	void DeleteWebFile();
	void QueryDataLength();
	virtual UINT ReadWebFileOneTime(); // 无法测试，故而虚拟化后使用Mock类来测试。
	void XferReadingToBuffer(UINT uByteRead);
	bool IncreaseBufferSizeIfNeeded(long lIncreaseSize);
	CWebDataPtr CreateWebData();

	void VerifyDataLength() const;
	void TransferDataToWebData(const CWebDataPtr& pWebData); // 将接收到的数移至pWebData中

	long GetByteRead() const noexcept { return m_lByteRead; }
	void SetByteRead(long number) { m_lByteRead = number; } // 仅用于测试中

	// 各状态
	void SetInquiryString(const CString& strInquiry) { m_strInquiry = strInquiry; }
	void SetInquiryHeader(const CString& strHeader) { m_strHeaders = strHeader; }

	bool IsWebError() const noexcept { return m_fWebError; }
	void SetWebError(bool fFlag) noexcept { m_fWebError = fFlag; }

	void SetContentLength(long length) noexcept { m_lContentLength = length; }
	long GetContentLength() const noexcept { return m_lContentLength; } // 仅用于测试中
	void SetBufferSize(long size) { m_sBuffer.resize(size); }
	size_t GetBufferSize() const noexcept { return m_sBuffer.size(); } // 仅用于测试中

public:
	// 以下为测试用函数
	void TESTSetBuffer(const char* buffer, INT64 lTotalNumber);
	void TESTSetBuffer(CString str);
	void TESTSetWebBuffer(const char* buffer, INT64 lTotalNumber);

protected:
	shared_ptr<CInternetSession> m_pSession;
	CHttpFile* m_pFile; // 网络文件指针
	DWORD m_dwHTTPStatusCode; //网络状态码

	bool m_fWebError; //网络读取错误代码。也用于网络错误判断的依据：当为零时无错误。
	string m_sBuffer; // 接收到数据的缓冲区
	long m_lByteRead; // 接收到的字符数.

	CString m_strInquiry; // 查询所需的字符串（m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	CString m_strHeaders;

	long m_lContentLength; // 预期的网络数据长度（使用QueryInfo(HTTP_QUERY_CONTENT_LENGTH)得到的数据）

private:
	char m_dataBuffer[WEB_SOURCE_DATA_BUFFER_SIZE_]; //网络数据缓存
};

using CDataInquireEnginePtr = shared_ptr<CInquireEngine>;
