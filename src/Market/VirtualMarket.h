#pragma once

#include "globedef.h"
#include"VirtualDataSource.h"

#include"MarketTaskQueue.h"

using std::vector;
using std::weak_ptr;

class CVirtualMarket {
public:
	CVirtualMarket();
	// 不允许赋值。
	CVirtualMarket(const CVirtualMarket&) = delete;
	CVirtualMarket& operator=(const CVirtualMarket&) = delete;
	CVirtualMarket(const CVirtualMarket&&) noexcept = delete;
	CVirtualMarket& operator=(const CVirtualMarket&&) noexcept = delete;
	virtual ~CVirtualMarket() = default;

public:
	void ScheduleTask(); // 唯一的调度函数

	// 申请并处理Data source的数据，由最终衍生类的ScheduleMarketTask函数来调度。
	// 此函数在VirtualMarket中定义，但由最终衍生类来调用，因为lCurrentTime必须为该衍生类的当前市场时间。
	void RunDataSource(long lCurrentLocalMarketTime) const;

	virtual bool ProcessTask(long);

	virtual void ResetMarket();
	bool InResetTime(long lCurrentTime);
	virtual long GetResetTime() {
		ASSERT(0);
		return 0;
	}
	bool IsResettingMarket() const noexcept { return m_fResettingMarket; }

	virtual bool UpdateMarketInfo(); // 更新本市场信息。

	// MarketTask
	bool IsMarketTaskEmpty() const { return m_marketTask.Empty(); }
	void AddTask(const CMarketTaskPtr& pTask);
	void AddTask(const long lTaskType, const long lExecuteTime);
	CMarketTaskPtr GetMarketTask() const { return m_marketTask.GetTask(); }
	void DiscardCurrentMarketTask() { m_marketTask.DiscardCurrentTask(); }
	vector<CMarketTaskPtr> GetMarketTaskVector() { return m_marketTask.GetTaskVector(); }
	void AdjustTaskTime();

	// MarketDisplayTask
	bool HaveNewTask() const;
	shared_ptr<vector<CMarketTaskPtr>> DiscardOutDatedTask(long m_lCurrentMarketTime);
	vector<CMarketTaskPtr> GetDisplayMarketTask();

	// 时间函数
	tm GetMarketTime(time_t tUTC) const; // 得到本市场的时间（从UTC时间）
	time_t TransferToUTCTime(tm* tmMarketTime) const; // 将市场时间结构转化为UTC时间
	time_t TransferToUTCTime(long lMarketDate, long lMarketTime = 150000) const; // 将市场时间结构转化为UTC时间
	long GetMarketDate(time_t tUTC) const; // 得到本市场的日期

	long GetMarketTimeZone() const noexcept { return m_lMarketTimeZone; }
	CString GetMarketID() const noexcept { return m_strMarketId; }
	long GetMarketTime() const noexcept { return m_lMarketTime; } //得到本市场的当地时间，格式为：hhmmss
	long GetMarketDate() const noexcept { return m_lMarketDate; } // 得到本市场的当地日期， 格式为：yyyymmdd
	long GetDayOfWeek() const noexcept { return m_tmMarket.tm_wday; } // days since Sunday - [0, 6]
	long GetMonthOfYear() const noexcept { return m_tmMarket.tm_mon + 1; }
	long GetDateOfMonth() const noexcept { return m_tmMarket.tm_mday; }
	long GetYear() const noexcept { return m_tmMarket.tm_year + 1900; }

	long GetLastTradeDate() noexcept {
		CalculateLastTradeDate();
		return m_lMarketLastTradeDate;
	}

	bool IsWorkingDay() const noexcept;
	static bool IsWorkingDay(CTime timeCurrent) noexcept;
	static bool IsWorkingDay(long lDate) noexcept;

	virtual bool IsOrdinaryTradeTime() { return true; } // 日常交易时间
	virtual bool IsOrdinaryTradeTime(long) { return true; } // 参数为市场当前时间hhmmss
	virtual bool IsWorkingTime() { return true; } // 工作时间（日常交易时间 + 延长的交易时间）
	virtual bool IsWorkingTime(long) { return true; } // 参数为市场当前时间hhmmss
	virtual bool IsDummyTime() { return false; } // 空闲时间
	virtual bool IsDummyTime(long) { return false; } // 参数为市场当前时间hhmmss

	CString GetStringOfLocalTime() const; // 得到本地时间的字符串
	CString GetStringOfLocalDateTime() const;
	CString GetStringOfMarketTime() const; // 得到本市场时间的字符串
	CString GetStringOfMarketDate() const;
	CString GetStringOfMarketDateTime() const;

	void CalculateTime() noexcept; // 计算本市场的各时间
	void CalculateLastTradeDate() noexcept;

	bool IsReadyToRun() const noexcept { return m_fReadyToRun; }
	void SetReadyToRun(const bool fFlag) noexcept { m_fReadyToRun = fFlag; }

	virtual bool IsTimeToResetSystem(long) { return false; } // 默认永远处于非重启市场状态，继承类需要各自设置之
	bool IsSystemReady() const noexcept { return m_fSystemReady; }
	void SetSystemReady(const bool fFlag) noexcept { m_fSystemReady = fFlag; }

	virtual void PrepareToCloseMarket() {
	} // 准备退出本市场（完成系统退出前的准备工作）。

	// 存储数据源
	void StoreDataSource(const CVirtualDataSourcePtr& pDataSource) { m_vDataSource.push_back(pDataSource); }

public:
	// 测试用函数
	static void TEST_SetUTCTime(const time_t Time) noexcept { gl_tUTCTime = Time; }
	void TEST_SetFormattedMarketTime(const long lTime) noexcept { m_lMarketTime = lTime; } // 此函数只用于测试
	void TEST_SetMarketTM(const tm& tm_) noexcept { m_tmMarket = tm_; }
	void TEST_SetFormattedMarketDate(const long lDate) noexcept { m_lMarketDate = lDate; }

protected:
	CString m_strMarketId{_T("Warning: CVirtualMarket Called.")}; // 该市场标识字符串

	CMarketTaskQueue m_marketTask; // 本市场当前任务队列
	ConcurrentQueue<CMarketTaskPtr> m_qMarketDisplayTask; // 当前任务显示队列
	long m_lLastQueueLength{0};

	vector<CVirtualDataSourcePtr> m_vDataSource; // 本市场中的各网络数据源。

	// Finnhub.io提供的信息
	CString m_strCode;
	CString m_strName;
	CString m_strMic;
	CString m_strTimeZone;
	CString m_strHour;
	CString m_strCloseDate;
	CString m_strCountry;
	CString m_strSource;

	long m_lMarketTimeZone{-8 * 3600}; // 该市场的时区与GMT之差（以秒计，负值处于东十二区（超前），正值处于西十二区（滞后））。与_get_timezone函数相符。

	// 以下时间日期为本市场的标准日期和时间（既非GMT时间也非软件使用时所处的当地时间，而是该市场所处地区的标准时间，如中国股市永远为东八区）。
	long m_lMarketDate{0}; //本市场的日期
	long m_lMarketTime{0}; // 本市场的时间
	long m_lMarketLastTradeDate{0}; // 本市场的上次交易日期
	tm m_tmMarket{0, 0, 0, 1, 0, 1970}; // 本市场时间结构

	//系统状态区
	bool m_fSystemReady{false}; // 市场初始态已经设置好.默认为假
	bool m_fResettingMarket{false}; // 市场正在重启标识

private:
	bool m_fReadyToRun{true}; // 市场准备好运行标识。目前永远为真。
	bool m_fResetMarket{true}; // 重启系统标识
};

using CVirtualMarketWeakPtr = weak_ptr<CVirtualMarket>;
using CVirtualMarketPtr = shared_ptr<CVirtualMarket>;
extern vector<CVirtualMarketPtr> gl_vMarketPtr; // 各市场指针的容器，只用于执行各市场的ScheduleTask
