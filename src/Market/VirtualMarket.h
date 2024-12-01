#pragma once

#include"VirtualDataSource.h"
#include"MarketTaskQueue.h"
#include "MarketTime.h"

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

	// 时间函数套层
	void InitializeMarketTime(const CString& strLocalPosition, long OpenTime) { m_marketTime.Initialize(strLocalPosition, OpenTime); }
	auto GetMarketTimeZone() const { return m_marketTime.GetMarketTimeZone(); }
	void CalculateTime() { m_marketTime.CalculateTime(); }
	CString GetStringOfMarketTime() const { return m_marketTime.GetStringOfMarketTime(); }
	long GetMarketDate() const { return m_marketTime.GetMarketDate(); }
	auto GetMarketDate(time_t tUTC) const { return m_marketTime.GetMarketDate(tUTC); }
	long GetMarketTime() const { return m_marketTime.GetMarketTime(); }
	tm GetMarketTime(time_t tUTC) const { return m_marketTime.GetMarketTime(tUTC); }

	bool IsWorkingDay() const { return m_marketTime.IsWorkingDay(); }
	auto IsWorkingDay(long date) const { return m_marketTime.IsWorkingDay(date); }
	auto IsWorkingDay(CTime ct) const { return m_marketTime.IsWorkingDay(ct); }

	auto GetDayOfWeek() const { return m_marketTime.GetDayOfWeek(); }

	auto CalculateLastTradeDate() { return m_marketTime.CalculateLastTradeDate(); }
	auto CalculateCurrentTradeDate() { return m_marketTime.CalculateCurrentTradeDate(); }
	auto CalculateNextTradeDate() { return m_marketTime.CalculateNextTradeDate(); }

	auto GetCurrentTradeDate() { return m_marketTime.GetCurrentTradeDate(); }
	auto GetLastTradeDate() { return m_marketTime.GetLastTradeDate(); }
	auto GetNextTradeDate() { return m_marketTime.GetNextTradeDate(); }

	auto TransferToUTCTime(tm* tm_) const { return m_marketTime.TransferToUTCTime(tm_); }
	auto TransferToUTCTime(long Date) const { return m_marketTime.TransferToUTCTime(Date); }
	auto TransferToUTCTime(long date, long time) const { return m_marketTime.TransferToUTCTime(date, time); }

	auto GetMonthOfYear() const { return m_marketTime.GetMonthOfYear(); }
	auto GetDateOfMonth() const { return m_marketTime.GetDateOfMonth(); }
	auto GetYear() const { return m_marketTime.GetYear(); }
	auto GetMarketTM() { return m_marketTime.GetMarketTM(); }

	auto GetStringOfMarketDate() const { return m_marketTime.GetStringOfMarketDate(); }
	auto GetStringOfLocalTime() const { return m_marketTime.GetStringOfLocalTime(); }
	auto GetStringOfLocalDateTime() const { return m_marketTime.GetStringOfLocalDateTime(); }
	auto GetStringOfMarketDateTime() const { return m_marketTime.GetStringOfMarketDateTime(); }

	long ConvertToDate(time_t tUTC) const noexcept { return m_marketTime.ConvertToDate(tUTC); }
	long ConvertToDate(const tm* ptm) const noexcept { return m_marketTime.ConvertToDate(ptm); }

	time_t ConvertBufferToTime(const CString& strFormat, const char* BufferMarketTime) { return m_marketTime.ConvertBufferToTime(strFormat, BufferMarketTime); }
	time_t ConvertStringToTime(const CString& strFormat, const CString& strMarketTime) { return m_marketTime.ConvertStringToTime(strFormat, strMarketTime); }

	// 测试用
	auto TEST_SetUTCTime(time_t tUTC) const { m_marketTime.TEST_SetUTCTime(tUTC); }
	auto TEST_SetMarketTM(const tm& tm_) { m_marketTime.TEST_SetMarketTM(tm_); }
	auto TEST_SetFormattedMarketDate(long date) { m_marketTime.TEST_SetFormattedMarketDate(date); }
	auto TEST_SetFormattedMarketTime(long time) { m_marketTime.TEST_SetFormattedMarketTime(time); }

	/////
	CString GetMarketID() const noexcept { return m_strMarketId; }

	virtual bool IsOrdinaryTradeTime() { return true; } // 日常交易时间
	virtual bool IsOrdinaryTradeTime(long) { return true; } // 参数为市场当前时间hhmmss
	virtual bool IsWorkingTime() { return true; } // 工作时间（日常交易时间 + 延长的交易时间）
	virtual bool IsWorkingTime(long) { return true; } // 参数为市场当前时间hhmmss
	virtual bool IsDummyTime() { return false; } // 空闲时间
	virtual bool IsDummyTime(long) { return false; } // 参数为市场当前时间hhmmss

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

protected:
	CString m_strMarketId{ _T("Warning: CVirtualMarket Called.") }; // 该市场标识字符串

	CMarketTaskQueue m_marketTask; // 本市场当前任务队列
	CMarketTaskQueue m_marketImmediateTask; // 本市场当前即时任务队列（此任务序列一次执行完毕，无需等待）
	ConcurrentQueue<CMarketTaskPtr> m_qMarketDisplayTask; // 当前任务显示队列
	size_t m_lLastQueueLength{ 0 };

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

	//系统状态区
	bool m_fSystemReady{ false }; // 市场初始态已经设置好.默认为假
	bool m_fResettingMarket{ false }; // 市场正在重启标识，默认为假

	CMarketTime m_marketTime;

private:
	bool m_fResetMarket{ true }; // 重启系统标识
};

using CVirtualMarketWeakPtr = weak_ptr<CVirtualMarket>;
using CVirtualMarketPtr = shared_ptr<CVirtualMarket>;
extern vector<CVirtualMarketPtr> gl_vMarket; // 各市场指针的容器，只用于执行各市场的ScheduleTask
