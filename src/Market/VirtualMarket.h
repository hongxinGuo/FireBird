#pragma once

#include"VirtualDataSource.h"
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

public:
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

	long GetLastTradeDate() noexcept { // 最后交易日的前一个交易日
		CalculateLastTradeDate();
		return m_lMarketLastTradeDate;
	}

	long GetCurrentTradeDate() noexcept { // 最后交易日
		CalculateCurrentTradeDate();
		return m_lMarketNewestTradeDate;
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
	void CalculateCurrentTradeDate() noexcept; // 计算当前交易日。周一至周五为当日，周六和周日为周五
	void CalculateLastTradeDate() noexcept; // 计算当前交易日的上一个交易日。周二至周五为上一日，周六和周日为周四，周一为周五。

	virtual int XferMarketTimeToIndex() {// 将本市场的市场时间变成显示位置的偏移（各市场分别实现）
		ASSERT(false);
		return 0;
	}

	virtual bool IsReadyToInquireWebData(long /*lCurrentMarketTime*/) { return true; }

	virtual bool IsTimeToResetSystem(long) { return false; } // 默认永远处于非重启市场状态，继承类需要各自设置之
	bool IsSystemReady() const noexcept { return m_fSystemReady; }
	void SetSystemReady(const bool fFlag) noexcept { m_fSystemReady = fFlag; }

	virtual void PrepareToCloseMarket() {} // 准备退出本市场（完成系统退出前的准备工作）。

	// 存储数据源
	void StoreDataSource(const CVirtualDataSourcePtr& pDataSource) { m_vDataSource.push_back(pDataSource); }

public:
	// 测试用函数
	static void TEST_SetUTCTime(const time_t Time) noexcept { gl_tUTCTime = Time; }
	void TEST_SetFormattedMarketTime(const long lTime) noexcept { m_lMarketTime = lTime; } // 此函数只用于测试
	void TEST_SetMarketTM(const tm& tm_) noexcept { m_tmMarket = tm_; }
	void TEST_SetFormattedMarketDate(const long lDate) noexcept { m_lMarketDate = lDate; }

protected:
	CString m_strMarketId{ _T("Warning: CVirtualMarket Called.") }; // 该市场标识字符串

	CMarketTaskQueue m_marketTask; // 本市场当前任务队列
	CMarketTaskQueue m_marketImmediateTask; // 本市场当前即时任务队列（此任务序列一次执行完毕，无需等待）
	ConcurrentQueue<CMarketTaskPtr> m_qMarketDisplayTask; // 当前任务显示队列
	long m_lLastQueueLength{ 0 };

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

	long m_lMarketTimeZone{ -8 * 3600 }; // 该市场的时区与GMT之差（以秒计，负值处于东十二区（超前），正值处于西十二区（滞后））。与_get_timezone函数相符。

	// 以下时间日期为本市场的标准日期和时间（既非GMT时间也非软件使用时所处的当地时间，而是该市场所处地区的标准时间，如中国股市永远为东八区）。
	long m_lMarketDate{ 0 }; //本市场的日期
	long m_lMarketTime{ 0 }; // 本市场的时间
	long m_lMarketLastTradeDate{ 0 }; // 本市场的上次交易日期
	long m_lMarketNewestTradeDate{ 0 }; // 本市场最新的交易日期
	tm m_tmMarket{ 0, 0, 0, 1, 0, 1970 }; // 本市场时间结构

	//系统状态区
	bool m_fSystemReady{ false }; // 市场初始态已经设置好.默认为假
	bool m_fResettingMarket{ false }; // 市场正在重启标识，默认为假

private:
	bool m_fResetMarket{ true }; // 重启系统标识
};

using CVirtualMarketWeakPtr = weak_ptr<CVirtualMarket>;
using CVirtualMarketPtr = shared_ptr<CVirtualMarket>;
extern vector<CVirtualMarketPtr> gl_vMarket; // 各市场指针的容器，只用于执行各市场的ScheduleTask
