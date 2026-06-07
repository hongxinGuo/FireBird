#pragma once

#include"MarketTaskQueue.h"

class CVirtualMarket {
public:
	CVirtualMarket();
	// 不允许赋值。
	CVirtualMarket(const CVirtualMarket&) = delete;
	CVirtualMarket& operator=(const CVirtualMarket&) = delete;
	CVirtualMarket(const CVirtualMarket&&) noexcept = delete;
	CVirtualMarket& operator=(const CVirtualMarket&&) noexcept = delete;
	virtual ~CVirtualMarket() = default;

	void ScheduleTask(); // 唯一的调度函数

	// 申请并处理Data source的数据，由最终衍生类的ScheduleMarketTask函数来调度。
	// 此函数在VirtualMarket中定义，但由最终衍生类来调用，因为lCurrentTime必须为该衍生类的当前市场时间。
	void RunDataSource(long lMarketTime) const;

	virtual int ProcessTask(long) {
		ASSERT(0);// 每日定时任务调度,由ScheduleTask调度，由各市场定义其各自的任务,不允许调用本基类函数
		return 0;
	}
	virtual int ProcessCurrentImmediateTask(long) {
		ASSERT(0);// 即时任务调度,由ScheduleTask调度，由各市场定义其各自的任务,不允许调用本基类函数
		return 0;
	}

	virtual void ResetMarket();
	bool IsResetTime(long lCurrentTime);
	virtual long GetResetTime() {
		if (gl_systemConfiguration.IsWorkingMode()) // 不允许在运行状态时调用此函数
			ASSERT(0);
		return 0;
	}
	bool IsResetting() const noexcept { return m_fResettingMarket; }

	virtual bool UpdateMarketInfo(); // 更新本市场信息。

	// MarketTask
	bool IsMarketTaskEmpty() const { return m_marketTask.Empty(); }
	void AddTask(const CMarketTaskPtr& pTask);
	void AddTask(long lTaskType, long lExecuteTime);
	CMarketTaskPtr GetMarketTask() const { return m_marketTask.GetTask(); }
	void DiscardCurrentMarketTask() { m_marketTask.DiscardCurrentTask(); }
	vector<CMarketTaskPtr> GetMarketTasks() { return m_marketTask.GetTasks(); }
	void AdjustTaskTime();

	// MarketImmediateTask
	void AddImmediateTask(const CMarketTaskPtr& pTask);
	void AddImmediateTask(long lTaskType);

	// MarketDisplayTask
	bool HaveNewTask() const;
	vector<CMarketTaskPtr> DiscardOutDatedTask(long m_lCurrentMarketTime);
	vector<CMarketTaskPtr> GetDisplayMarketTask();

	// 时间函数
	void CalculateTime() noexcept; // 计算本市场的各时间

	void SetMarketLocalTimeOffset(const string& strLocalNameOfMarket);
	long GetMarketOpenTime() const { return m_exchange->m_lMarketOpenTime; }
	long GetMarketCloseTime() const { return m_exchange->m_lMarketCloseTime; }

	auto GetTimeZoneOffset() const { return m_TimeZoneOffset; }
	long GetTimeZone() const noexcept { return m_TimeZoneOffset.count(); }
	long GetMarketTime() const noexcept { return m_lMarketTime; } //得到本市场的当地时间，格式为：hhmmss
	long GetMarketDate() const noexcept { return m_lMarketDate; } // 得到本市场的当地日期， 格式为：yyyymmdd
	auto GetDayOfWeek() const noexcept { return m_marketWeekDay; } // days since Sunday - [0, 6]

	bool IsWorkingDay() const noexcept;
	static bool IsWorkingDay(long lDate) noexcept;

	long GetLastTradeDate();// 当前交易日的前一个交易日（从昨日开市时间至本日开市时间）计算当前交易日的上一个交易日。周二至周五为上一日，周六和周日为周四，周一为周五。
	long GetCurrentTradeDate();// 当前交易日（从本日九点半至次日开市时间）,计算当前交易日。周一至周五为当日，周六和周日为周五
	long GetNextTradeDate();// 下一个交易日（从次日开市时间至后日开市时间）

	string GetStringOfLocalTime() const; // 得到本地时间的字符串
	string GetStringOfMarketTime() const; // 得到本市场时间的字符串
	string GetStringOfMarketDate() const;
	string GetStringOfMarketDateTime() const;
	string GetStringOfLocalDateTime() const;

	chrono::local_seconds ToLocalTime(const chrono::sys_seconds& sysTime) const { return m_marketTimeZone->to_local(sysTime); } // 将UTC时间转化为本市场的当地时间
	chrono::sys_seconds ToSysTime(const chrono::local_seconds& localTime) const { return m_marketTimeZone->to_sys(localTime); } // 将本市场的当地时间转化为UTC时间
	chrono::sys_seconds ConvertToUTCTime(const chrono::hh_mm_ss<chrono::seconds>& hhMmSs); // 将市场时间转化为UTC时间
	chrono::sys_seconds ConvertToUTCTime(long lMarketDate, long lMarketTime) const; // 将市场时间转化为UTC时间
	long ConvertToDate(time_t tUTC) const noexcept;
	long ConvertToDate(chrono::sys_seconds tp) const noexcept;

	void GetMarketTimeStruct(tm* tm_, time_t tUTC) const;

	// 测试用
	void TEST_SetFormattedMarketTime(const long lTime) noexcept { m_lMarketTime = lTime; } // 此函数只用于测试
	void TEST_SetFormattedMarketDate(const long lDate) noexcept { m_lMarketDate = lDate; }

	/////
	string GetMarketID() const noexcept { return m_strMarketId; }

	virtual bool IsOrdinaryTradeTime() { return true; } // 日常交易时间
	virtual bool IsOrdinaryTradeTime(long) { return true; } // 参数为市场当前时间hhmmss
	virtual bool IsWorkingTime() { return true; } // 工作时间（日常交易时间 + 延长的交易时间）
	virtual bool IsWorkingTime(long) { return true; } // 参数为市场当前时间hhmmss
	virtual bool IsDummyTime() { return false; } // 空闲时间
	virtual bool IsDummyTime(long) { return false; } // 参数为市场当前时间hhmmss

	bool IsMarketClosed() const { return GetMarketTime() > GetMarketCloseTime(); }

	virtual bool IsReadyToInquireWebData(long /*lCurrentMarketTime*/) { return true; }

	virtual bool IsTimeToResetSystem(long) { return false; } // 默认永远处于非重启市场状态，继承类需要各自设置之
	bool IsSystemReady() const noexcept { return m_fSystemReady; }
	void SetSystemReady(const bool fFlag) noexcept { m_fSystemReady = fFlag; }

	virtual void PrepareToCloseMarket() {} // 准备退出本市场（完成系统退出前的准备工作）。

	// 存储数据源
	void StoreDataSource(const CVirtualDataSourcePtr& pDataSource) { m_vDataSource.push_back(pDataSource); }

protected:
	string m_strMarketId{ "Warning: CVirtualMarket Called." }; // 该市场标识字符串,即交易所的代码。中国为SS,美国为US....
	CStockExchangePtr m_exchange{ nullptr };
	CMarketTaskQueue m_marketTask; // 本市场当前任务队列
	CMarketTaskQueue m_marketImmediateTask; // 本市场当前即时任务队列（此任务序列一次执行完毕，无需等待）
	ConcurrentQueue<CMarketTaskPtr> m_qMarketDisplayTask{ 32 * 4 }; // 当前任务显示队列
	size_t m_lLastQueueLength{ 0 };

	vector<CVirtualDataSourcePtr> m_vDataSource; // 本市场中的各网络数据源。

	// Finnhub.io提供的信息
	string m_strCode;
	string m_strName;
	string m_strMic;
	string m_strTimeZone;
	string m_strHour;
	string m_strCloseDate;
	string m_strCountry;
	string m_strSource;

	//系统状态区
	bool m_fSystemReady{ false }; // 市场初始态已经设置好.默认为假
	bool m_fResettingMarket{ false }; // 市场正在重启标识，默认为假

	// 系统时间区
	//string m_strLocalMarketTimeZone{ "Europe/London" }; // 本市场当地时区名称 Asia/Shanghai, America/New_York, ...
	string m_strLocalMarketTimeZone{ "America/New_York" }; // 本市场当地时区名称 Asia/Shanghai, America/New_York, ...
	const chrono::time_zone* m_marketTimeZone{ nullptr }; // 本市场当地时区
	chrono::local_seconds m_marketTime; // 本市场的当地时间
	chrono::year_month_day m_marketYearMonthDay; // 本市场的当地年、月、日
	chrono::weekday m_marketWeekDay; // 本市场的当地星期几
	chrono::hh_mm_ss<chrono::seconds> m_marketTimeOfDay; // 本市场的当地小时、分钟、秒
	chrono::seconds m_TimeZoneOffset{ 0 };

	// 以下时间日期为本市场的标准日期和时间（既非GMT时间也非软件使用时所处的当地时间，而是该市场所处地区的标准时间，如中国股市永远为东八区）。
	long m_lMarketDate{ 0 }; //本市场的日期
	long m_lMarketTime{ 0 }; // 本市场的时间

	long m_lMarketCloseTime{ 0 }; // 市场闭市时间。

private:
	bool m_fResetMarket{ true }; // 重启系统标识
};

using CVirtualMarketWeakPtr = weak_ptr<CVirtualMarket>;
using CVirtualMarketPtr = shared_ptr<CVirtualMarket>;
extern vector<CVirtualMarketPtr> gl_vMarket; // 各市场指针的容器，只用于执行各市场的ScheduleTask
