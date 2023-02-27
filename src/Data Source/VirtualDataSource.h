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

// 此信号量用于解析WebSource中的数据。
// 将ParseAndStoreData线程限制至最多3个，这样既能保证足够的计算速度，也不会发生系统颠簸。当改为4个时，就能观察到系统颠簸。
extern counting_semaphore<3> gl_WebSourceParseAndStoreData;

class CVirtualDataSource {
public:
	CVirtualDataSource(void);
	// 只能有一个实例,不允许赋值。
	CVirtualDataSource(const CVirtualDataSource&) = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&) = delete;
	CVirtualDataSource(const CVirtualDataSource&&) noexcept = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&&) noexcept = delete;
	virtual ~CVirtualDataSource(void) = default;
	virtual bool Reset(void) { return true; }

	void Run(long lCurrentTime);
	virtual bool GenerateInquiryMessage(const long) { return true; } // 继承类必须实现各自的查询任务. 参数为当前市场时间（hhmmss）
	virtual void CreateThreadGetWebDataAndProcessIt();
	bool GetWebDataAndProcessIt();
	virtual void GenerateCurrentInquiryMessage(void);
	virtual bool GetWebData(void); // 网络读取。为了Mock方便，声明为虚函数。
	virtual bool ProcessWebDataReceived(void);
	virtual void UpdateStatus(void) { }

	void SetDefaultSessionOption(void);

	bool Read(void); // 实际读取处理函数，完成工作线程的实际功能
	virtual void ReadWebData(void); // 网络实际读取函数
	virtual void OpenFile(const CString& strInquiring);
	virtual void GetFileHeaderInformation();
	void DeleteWebFile();
	long QueryDataLength();
	virtual UINT ReadWebFileOneTime(void); // 无法测试，故而虚拟化后使用Mock类。
	void XferReadingToBuffer(long lPosition, UINT uByteRead);
	bool IncreaseBufferSizeIfNeeded(long lIncreaseSize = 1024 * 1024);
	virtual CWebDataPtr CreateWebDataAfterSucceedReading();

	void VerifyDataLength() const;
	static void SetDataTime(CWebDataPtr pData, time_t time) noexcept { pData->SetTime(time); }
	virtual bool TransferDataToWebData(CWebDataPtr pWebData); // 将接收到的数移至pWebData中
	virtual bool ParseData(CWebDataPtr pWebData) {
		TRACE("调用了基类函数ParseData()\n");
		return false;
	} //解析接收到的数据。继承类必须实现此函数。
	void ResetBuffer(void) { m_sBuffer.resize(DefaultWebDataBufferSize_); }

	virtual void PrepareReadingWebData(void); // 在读取网络数据前的准备工作，默认为设置m_pSession状态。
	virtual void ConfigureSession(void) {
		TRACE("调用了基类函数ConfigureSession()\n");
	} // 配置m_pSession。继承类必须实现此功能，每个网站的状态都不一样，故而需要单独配置。
	virtual void UpdateStatusAfterReading(CWebDataPtr pData) {} //成功接收后更新系统状态。默认无动作

	void CreateTotalInquiringString();
	CString GetInquiringString(void) const noexcept { return m_strInquiry; }
	void SetInquiringString(const CString& str) noexcept { m_strInquiry = str; }
	void AppendInquiringString(const CString& str) noexcept { m_strInquiry += str; }

	char GetData(const long lIndex) const { return m_sBuffer.at(lIndex); }
	void SetData(const long lIndex, const char value) { m_sBuffer.at(lIndex) = value; }

	long GetByteRead(void) const noexcept { return m_lByteRead; }
	void SetByteRead(const long lValue) noexcept { m_lByteRead = lValue; }
	void AddByteRead(const long lValue) noexcept { m_lByteRead += lValue; }
	size_t GetBufferSize(void) const noexcept { return m_sBuffer.size(); }

	virtual ULONGLONG GetTickCount() { return ::GetTickCount64(); } // 为了测试方便，将GetTickCount64包裹上一层。

	// 各状态
	CVirtualProductWebDataPtr GetCurrentInquiry(void) const noexcept { return m_pCurrentProduct; }
	void SetCurrentInquiry(const CVirtualProductWebDataPtr& p) { m_pCurrentProduct = p; }

	size_t GetInquiryQueueSize(void) const noexcept { return m_qProduct.size(); }
	void StoreInquiry(const CVirtualProductWebDataPtr& p) { m_qProduct.push(p); }

	CVirtualProductWebDataPtr GetCurrentProduct(void) {
		m_pCurrentProduct = m_qProduct.front();
		m_qProduct.pop();
		return m_pCurrentProduct;
	}

	bool HaveInquiry(void) const {
		if (m_qProduct.empty()) return false;
		return true;
	}

	void StoreReceivedData(const CWebDataPtr pData) noexcept { m_qReceivedData.PushData(pData); }
	CWebDataPtr GetReceivedData(void) noexcept { return m_qReceivedData.PopData(); }
	size_t GetReceivedDataSize(void) noexcept { return m_qReceivedData.Size(); }

	bool HaveReceivedData(void) {
		if (m_qReceivedData.Empty()) return false;
		return true;
	}

	bool IsInquiring(void) const noexcept { return m_fInquiring; }
	void SetInquiring(const bool fFlag) noexcept { m_fInquiring = fFlag; }
	bool IsGetWebDataAndProcessItThreadRunning() const noexcept { return m_bIsGetWebDataAndProcessItThreadRunning; }
	void SetGetWebDataAndProcessItThreadRunning(bool fFlag) noexcept { m_bIsGetWebDataAndProcessItThreadRunning = fFlag; }

	bool IsInquiringAndClearFlag(void) noexcept {
		const bool fInquiring = m_fInquiring.exchange(false);
		return fInquiring;
	}

	bool IsEnable(void) const noexcept { return m_fEnable; }
	void Enable(const bool fFlag) noexcept { m_fEnable = fFlag; }

	CString GetHeaders(void) const noexcept { return m_strHeaders; }
	void SetHeaders(const CString& strHeaders) noexcept { m_strHeaders = strHeaders; }

	CString GetInquiryFunction(void) const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(const CString& strPrefix) noexcept { m_strInquiryFunction = strPrefix; }
	CString GetInquirySuffix(void) const noexcept { return m_strSuffix; }
	void SetInquirySuffix(const CString& strSuffix) noexcept { m_strSuffix = strSuffix; }
	CString GetInquiryToken(void) const noexcept { return m_strInquiryToken; }
	void SetInquiryToken(const CString& strToken) noexcept { m_strInquiryToken = strToken; }

	bool IsWebError(void) const noexcept { return m_dwWebErrorCode != 0; }
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

	// 辅助函数
	void DiscardProduct() { while (m_qProduct.size() > 0) m_qProduct.pop(); }
	void DiscardReceivedData() { while (GetReceivedDataSize() > 0) GetReceivedData(); }

public:
	// 以下为测试用函数
	void TESTSetBuffer(char* buffer, INT64 lTotalNumber);
	void TESTSetBuffer(CString str);
	void TESTSetWebBuffer(char* buffer, INT64 lTotalNumber);

protected:
	queue<CVirtualProductWebDataPtr, list<CVirtualProductWebDataPtr>> m_qProduct; // 网络查询命令队列
	CVirtualProductWebDataPtr m_pCurrentProduct;
	CTemplateMutexAccessQueue<CWebData> m_qReceivedData; // 网络数据暂存队列

	shared_ptr<CInternetSession> m_pSession;
	CHttpFile* m_pFile; // 网络文件指针
	DWORD m_dwHTTPStatusCode; //网络状态码

	CString m_strHeaders; // OpenURL时的headers字符串值， 默认为_T("")
	DWORD m_dwWebErrorCode; //网络读取错误代码。也用于网络错误判断的依据：当为零时无错误。
	string m_sBuffer; // 接收到数据的缓冲区
	long m_lByteRead; // 接收到的字符数.

	CString m_strInquiry; // 查询所需的字符串（m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	CString m_strInquiryFunction; // 查询字符串功能部分
	CString m_strParam; // 查询字符串的参数
	CString m_strSuffix; // 查询字符串的后缀部分
	CString m_strInquiryToken; // 查询字符串令牌

	long m_lContentLength; // 预期的网络数据长度（使用QueryInfo(HTTP_QUERY_CONTENT_LENGTH)得到的数据）

	long m_lInquiringNumber; // 每次查询数量
	time_t m_tCurrentInquiryTime; // 当前接收数据所需时间（以毫秒计）

	static atomic_long sm_lTotalByteRead; // 当前网络读取字节数。所有的网络读取器都修改此变量，故而声明为静态。

	long long m_llLastTimeTickCount;

	bool m_fEnable; // 允许执行标识
	atomic_bool m_fInquiring;
	atomic_bool m_bIsGetWebDataAndProcessItThreadRunning;

private:
	char m_dataBuffer[DATA_BUFFER_SIZE_]; //网络数据缓存
};

using CVirtualDataSourcePtr = shared_ptr<CVirtualDataSource>;
