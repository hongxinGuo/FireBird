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

	CWebDataPtr GetWebData(); // 实际读取处理函数，GeeWebData()的实现，完成工作线程的实际功能
	void ReadWebData(); // 网络实际读取函数
	void OpenFile();
	void GetFileHeaderInformation();
	void DeleteWebFile();
	void QueryDataLength();
	UINT ReadWebFileOneTime(); // 无法测试，故而虚拟化后使用Mock类。
	void XferReadingToBuffer(UINT uByteRead);
	bool IncreaseBufferSizeIfNeeded(long lIncreaseSize);
	CWebDataPtr CreateWebData();

	void VerifyDataLength() const;
	void TransferDataToWebData(const CWebDataPtr& pWebData); // 将接收到的数移至pWebData中

	long GetByteRead() const noexcept { return m_lByteRead; }

	// 各状态
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
	// 以下为测试用函数
	void TESTSetBuffer(const char* buffer, INT64 lTotalNumber);
	void TESTSetBuffer(CString str);
	void TESTSetWebBuffer(const char* buffer, INT64 lTotalNumber);

protected:
	CVirtualProductWebDataPtr m_pCurrentProduct;

	shared_ptr<CInternetSession> m_pSession;
	CHttpFile* m_pFile; // 网络文件指针
	DWORD m_dwHTTPStatusCode; //网络状态码

	DWORD m_dwWebErrorCode; //网络读取错误代码。也用于网络错误判断的依据：当为零时无错误。
	string m_sBuffer; // 接收到数据的缓冲区
	long m_lByteRead; // 接收到的字符数.

	CString m_strInquiry; // 查询所需的字符串（m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	CString m_strHeaders;

	long m_lContentLength; // 预期的网络数据长度（使用QueryInfo(HTTP_QUERY_CONTENT_LENGTH)得到的数据）

	atomic_int64_t m_tCurrentInquiryTime; // 当前接收数据所需时间（以毫秒计）

	long long m_llLastTimeTickCount;

private:
	char m_dataBuffer[1024 * 16]; //网络数据缓存
};

using CDataInquireEnginePtr = shared_ptr<CInquireEngine>;
