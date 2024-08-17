#pragma once

#include"VirtualWebProduct.h"
#include "InquireEngine.h"

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

	void Run(long lMarketTime);
	void RunWorkingThread(long lMarketTime);
	virtual bool GenerateInquiryMessage(const long) { return true; } // 继承类必须实现各自的查询任务. 参数为当前市场时间（hhmmss）
	virtual void GenerateCurrentInquiryMessage();
	virtual void CheckInaccessible(const CWebDataPtr&) const {
	}

	void SetDefaultSessionOption();

	virtual void ConfigureInternetOption() {
		ASSERT(false); // 调用了基类函数ConfigureInternetOption
	} // 配置internet参数。继承类必须实现此功能，每个网站的状态都不一样，故而需要单独配置。
	virtual void UpdateStatus(CWebDataPtr pData) {
	} //成功接收后更新系统状态。

	void CreateTotalInquiringString();
	CString GetInquiringString() const noexcept { return m_strInquiry; }
	void SetInquiringString(const CString& str) noexcept { m_strInquiry = str; }
	void AppendInquiringString(const CString& str) noexcept { m_strInquiry += str; }

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
	DWORD GetWebErrorCode() const noexcept { return m_dwWebErrorCode.load(); }
	void SetWebErrorCode(INT64 dwErrorCode) noexcept { m_dwWebErrorCode = dwErrorCode; }

	long GetInquiringNumber() const noexcept { return m_lInquiringNumber; }
	void SetInquiringNumber(const long lValue) noexcept { m_lInquiringNumber = lValue; }

	static long GetTotalByteRead() noexcept { return sm_lTotalByteRead.load(); }
	static void SetTotalByteRead(const long lValue) noexcept { sm_lTotalByteRead = lValue; }
	static void ClearTotalByteRead() noexcept { sm_lTotalByteRead = 0; }
	static long GetTotalByteReadPerSecond() noexcept { return sm_lTotalByteReadPerSecond.load(); }
	static void CalcTotalBytePerSecond() { sm_lTotalByteReadPerSecond = sm_lTotalByteRead.exchange(0); }

	void SetCurrentInquiryTime(const time_t tt) noexcept { m_tCurrentInquiryTime = tt; }
	virtual time_t GetCurrentInquiryTime() const noexcept { return m_tCurrentInquiryTime.load(); }

	void SetWebBusy(bool busy) noexcept { m_bWebBusy = busy; }
	bool IsWebBusy() const noexcept { return m_bWebBusy.load(); }

protected:
	queue<CVirtualProductWebDataPtr, list<CVirtualProductWebDataPtr>> m_qProduct; // 网络查询命令队列
	CVirtualProductWebDataPtr m_pCurrentProduct;

	bool m_fWebError; //网络读取错误
	atomic_int64_t m_dwWebErrorCode{0}; // 网络错误码

	InternetOption m_internetOption;
	CString m_strInquiry; // 查询所需的字符串（m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	CString m_strInquiryFunction; // 查询字符串功能部分
	CString m_strParam; // 查询字符串的参数
	CString m_strSuffix; // 查询字符串的后缀部分
	CString m_strInquiryToken; // 查询字符串令牌
	CString m_strHeaders; // OpenURL时的headers字符串值， 默认为_T("")

	long m_lInquiringNumber; // 每次查询数量
	int m_iMaxNormalInquireTime{300}; // 最大正常查询时间（每个具体的数据源皆不同）
	atomic_int64_t m_tCurrentInquiryTime; // 当前接收数据所需时间（以毫秒计）

	static atomic_long sm_lTotalByteRead; // 当前网络读取字节数。所有的网络读取器都修改此变量，故而声明为静态。
	static atomic_long sm_lTotalByteReadPerSecond; // 

	long long m_llLastTimeTickCount;

	atomic_bool m_fEnable; // 允许执行标识
	atomic_bool m_fInquiring;
	atomic_bool m_bIsWorkingThreadRunning;
	atomic_bool m_bWebBusy{false};
};

using CVirtualDataSourcePtr = shared_ptr<CVirtualDataSource>;
