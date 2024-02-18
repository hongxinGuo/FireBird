#pragma once

#include"afxinet.h"

#include"concurrentqueue/concurrentqueue.h"
using namespace moodycamel;

#include"VirtualWebProduct.h"

#include<semaphore>
#include<list>
#include<memory>

using std::counting_semaphore;
using std::list;
using std::atomic_bool;
using std::atomic_long;

constexpr auto DATA_BUFFER_SIZE_ = 1024 * 16;

class CVirtualDataSource : public std::enable_shared_from_this<CVirtualDataSource> {
	friend CVirtualWebProduct;

public:
	CVirtualDataSource();
	// 只能有一个实例,不允许赋值。
	CVirtualDataSource(const CVirtualDataSource&) = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&) = delete;
	CVirtualDataSource(const CVirtualDataSource&&) noexcept = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&&) noexcept = delete;
	virtual ~CVirtualDataSource() = default;

	std::shared_ptr<CVirtualDataSource> GetShared() { return shared_from_this(); }

	virtual bool Reset() { return true; }

	void Run(long lCurrentLocalMarketTime);
	virtual bool GenerateInquiryMessage(const long) { return true; } // 继承类必须实现各自的查询任务. 参数为当前市场时间（hhmmss）
	virtual void CreateThreadGetWebDataAndProcessIt();
	void GetWebDataAndProcessIt();
	virtual void GenerateCurrentInquiryMessage();
	virtual void GetWebData() { GetWebDataImp(); } // 网络读取。为了Mock方便，声明为虚函数。所有的Mock类，皆应Mock此函数以防止出现实际网络申请
	virtual void ProcessWebDataReceived();
	virtual void CheckInaccessible(const CWebDataPtr&) const {
	};

	void SetDefaultSessionOption() const;

	void GetWebDataImp(); // 实际读取处理函数，GeeWebData()的实现，完成工作线程的实际功能
	virtual void ReadWebData(); // 网络实际读取函数
	virtual void OpenFile(const CString& strInquiring);
	virtual void GetFileHeaderInformation();
	void DeleteWebFile();
	long QueryDataLength();
	virtual UINT ReadWebFileOneTime(); // 无法测试，故而虚拟化后使用Mock类。
	void XferReadingToBuffer(long lPosition, UINT uByteRead);
	bool IncreaseBufferSizeIfNeeded(long lIncreaseSize = 1024 * 1024);
	virtual CWebDataPtr CreateWebDataAfterSucceedReading();

	void VerifyDataLength() const;
	static void SetDataTime(const CWebDataPtr& pData, const time_t time) noexcept { pData->SetTime(time); }
	void TransferDataToWebData(const CWebDataPtr& pWebData); // 将接收到的数移至pWebData中
	void ResetBuffer(int size = DefaultWebDataBufferSize_) { m_sBuffer.resize(size); }

	virtual void PrepareReadingWebData(); // 在读取网络数据前的准备工作，默认为设置m_pSession状态。
	virtual void ConfigureSession() {
		ASSERT(false); // 调用了基类函数ConfigureSession
	} // 配置m_pSession。继承类必须实现此功能，每个网站的状态都不一样，故而需要单独配置。
	virtual void UpdateStatusAfterReading(CWebDataPtr pData) {
	} //成功接收后更新系统状态。默认无动作

	void CreateTotalInquiringString();
	CString GetInquiringString() const noexcept { return m_strInquiry; }
	void SetInquiringString(const CString& str) noexcept { m_strInquiry = str; }
	void AppendInquiringString(const CString& str) noexcept { m_strInquiry += str; }

	long GetByteRead() const noexcept { return m_lByteRead; }
	void SetByteRead(const long lValue) noexcept { m_lByteRead = lValue; }
	void AddByteRead(const long lValue) noexcept { m_lByteRead += lValue; }
	size_t GetBufferSize() const noexcept { return m_sBuffer.size(); }
	string_view GetBuffer() noexcept { return m_sBuffer; }

	virtual ULONGLONG GetTickCount() { return ::GetTickCount64(); } // 为了测试方便，将GetTickCount64包裹上一层。

	// 各状态
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
	bool IsInquiringAndClearFlag() noexcept {
		const bool fInquiring = m_fInquiring.exchange(false);
		return fInquiring;
	}

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

	bool IsWebError() const noexcept { return m_dwWebErrorCode != 0; }
	DWORD GetErrorCode() const noexcept { return m_dwWebErrorCode; }
	void SetErrorCode(const DWORD dwErrorCode) noexcept { m_dwWebErrorCode = dwErrorCode; }
	bool IsTimeout() const noexcept { return m_dwWebErrorCode == 12002; }

	long GetInquiringNumber() const noexcept { return m_lInquiringNumber; }
	void SetInquiringNumber(const long lValue) noexcept { m_lInquiringNumber = lValue; }

	static long GetTotalByteRead() noexcept { return sm_lTotalByteRead; }
	static void SetTotalByteRead(const long lValue) noexcept { sm_lTotalByteRead = lValue; }
	static void ClearTotalByteRead() noexcept { sm_lTotalByteRead = 0; }

	void SetCurrentInquiryTime(const time_t tt) noexcept { m_tCurrentInquiryTime = tt; }
	virtual time_t GetCurrentInquiryTime() const noexcept { return m_tCurrentInquiryTime; }

public:
	// 以下为测试用函数
	void TESTSetBuffer(const char* buffer, INT64 lTotalNumber);
	void TESTSetBuffer(CString str);
	void TESTSetWebBuffer(const char* buffer, INT64 lTotalNumber);

protected:
	queue<CVirtualProductWebDataPtr, list<CVirtualProductWebDataPtr>> m_qProduct; // 网络查询命令队列
	CVirtualProductWebDataPtr m_pCurrentProduct;
	ConcurrentQueue<CWebDataPtr> m_qReceivedData; // 网络数据暂存队列

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

	atomic_bool m_fEnable; // 允许执行标识
	atomic_bool m_fInquiring;
	atomic_bool m_bIsWorkingThreadRunning;

private:
	char m_dataBuffer[DATA_BUFFER_SIZE_]; //网络数据缓存
};

using CVirtualDataSourcePtr = shared_ptr<CVirtualDataSource>;
