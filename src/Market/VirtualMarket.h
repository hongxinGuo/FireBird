#pragma once

#include <concurrentqueue/moodycamel/concurrentqueue.h>

#include"MarketTaskQueue.h"

class CStockExchange;

using std::chrono::sys_seconds;
using std::chrono::weekday;
using std::chrono::time_zone;
using std::chrono::local_days;
using std::chrono::local_seconds;
using std::chrono::year_month_day;
using std::chrono::hh_mm_ss;
using std::chrono::days;
using std::chrono::floor;

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
	void RunDataSource() const;

	virtual int ProcessTask() {
		ASSERT(0);// 每日定时任务调度,由ScheduleTask调度，由各市场定义其各自的任务,不允许调用本基类函数
		return 0;
	}
	virtual int ProcessCurrentImmediateTask() {
		ASSERT(0);// 即时任务调度,由ScheduleTask调度，由各市场定义其各自的任务,不允许调用本基类函数
		return 0;
	}

	virtual void ResetMarket();
	bool IsResetTime();

	virtual local_seconds GetResetTime();

	bool IsResetting() const noexcept { return m_fResettingMarket; }

	virtual bool UpdateMarketInfo(); // 更新本市场信息。

	// MarketTask
	bool IsMarketTaskEmpty() const { return m_marketTask.Empty(); }
	void AddTask(const shared_ptr<CMarketTask>& pTask);
	void AddTask(long lTaskType, long lExecuteTime);
	void AddTask(long lTaskType, local_seconds executeTime);
	shared_ptr<CMarketTask> GetMarketTask() const { return m_marketTask.GetTask(); }
	void DiscardCurrentMarketTask() { m_marketTask.DiscardCurrentTask(); }
	void DiscardAllMarketTask() {
		while (!m_marketTask.Empty()) {
			m_marketTask.DiscardCurrentTask();
		}
	}
	void AdjustTaskTime();

	// MarketImmediateTask
	void AddImmediateTask(const shared_ptr<CMarketTask>& pTask);
	void AddImmediateTask(long lTaskType);

	// MarketDisplayTask
	bool HaveNewTask() const;
	vector<shared_ptr<CMarketTask>> DiscardOutDatedTask(local_seconds lCurrentMarketTime);
	vector<shared_ptr<CMarketTask>> GetDisplayMarketTask();

	// 时间函数
	void CalculateTime() noexcept; // 计算本市场的各时间
	void CreateLocalTimeZone(const string& strLocalNameOfMarket); // 系统启动时执行一次。
	local_seconds GetMarketOpenTime() const;
	local_seconds GetMarketCloseTime() const;

	local_seconds GetMarketClock() const noexcept { return m_marketClock; } // 这个是市场时间
	local_days GetMarketDate() const noexcept { return local_days(floor<days>(m_marketClock)); }// 市场日期
	local_seconds GetMarketTime() const noexcept { return local_seconds(m_marketClock - floor<days>(m_marketClock)); } // 市场时间的当天秒数
	weekday GetWeekDay() const noexcept { return weekday{ GetMarketDate() }; }
	year_month_day GetMarketYMD() const noexcept { return year_month_day{ GetMarketDate() }; }
	hh_mm_ss<seconds> GetMarketTimeHMS() const noexcept { return hh_mm_ss{ m_marketClock - floor<days>(m_marketClock) }; }

	bool IsWorkingDay() const noexcept;
	bool IsWorkingDay(const local_days& date) noexcept;

	local_days GetLastTradeDate();// 当前交易日的前一个交易日（从昨日开市时间至本日开市时间）计算当前交易日的上一个交易日。周二至周五为上一日，周六和周日为周四，周一为周五。
	local_days GetCurrentTradeDate();// 当前交易日（从本日九点半至次日开市时间）,计算当前交易日。周一至周五为当日，周六和周日为周五
	local_days GetNextTradeDate();// 下一个交易日（从次日开市时间至后日开市时间）

	string GetStringOfLocalTime() const; // 得到本地时间的字符串
	string GetStringOfMarketTime() const; // 得到本市场时间的字符串
	string GetStringOfMarketDate() const;
	string GetStringOfMarketDateTime() const;
	string GetStringOfLocalDateTime() const;

	local_seconds ToLocalTime(const sys_seconds& sysTime) const { return m_marketTimeZone->to_local(sysTime); } // 将UTC时间转化为本市场的当地时间
	sys_seconds ToSysTime(const local_seconds& localTime) const { return m_marketTimeZone->to_sys(localTime); } // 将本市场的当地时间转化为UTC时间
	sys_seconds ConvertToUTCTime(int marketDate, long lMarketTime) const; // 将市场时间转化为UTC时间
	long ConvertToDate(sys_seconds tp) const noexcept;

	// 测试用
	void TEST_SetMarketTime(local_seconds ls) noexcept;// 此函数只用于测试
	void TEST_SetMarketDate(local_days ld) noexcept;// 此函数只用于测试

	/////
	string GetMarketID() const noexcept { return m_strMarketId; }

	virtual bool IsOrdinaryTradeTime() { return true; } // 日常交易时间
	virtual bool IsOrdinaryTradeTime(local_seconds) { return true; } // 参数为市场当前时间hhmmss
	virtual bool IsWorkingTime() { return true; } // 工作时间（日常交易时间 + 延长的交易时间）
	virtual bool IsWorkingTime(local_seconds) { return true; } // 参数为市场当前时间hhmmss
	virtual bool IsDummyTime() { return false; } // 空闲时间
	virtual bool IsDummyTime(local_seconds) { return false; } // 参数为市场当前时间hhmmss

	bool IsMarketClosed() const { return GetMarketTime() > GetMarketCloseTime(); }

	virtual bool IsReadyToInquireWebData() { return true; }

	virtual bool IsTimeToResetSystem(local_seconds) { return false; } // 默认永远处于非重启市场状态，继承类需要各自设置之
	bool IsSystemReady() const noexcept { return m_fSystemReady; }
	void SetSystemReady(const bool fFlag) noexcept { m_fSystemReady = fFlag; }

	virtual void PrepareToCloseMarket() {} // 准备退出本市场（完成系统退出前的准备工作）。

	// 存储数据源
	void StoreDataSource(const CVirtualDataSourcePtr& pDataSource) { m_vDataSource.push_back(pDataSource); }

protected:
	string m_strMarketId{ "Warning: CVirtualMarket Called." }; // 该市场标识字符串,即交易所的代码。中国为SS,美国为US....
	shared_ptr<CStockExchange> m_exchange{ nullptr };
	CMarketTaskQueue m_marketTask; // 本市场当前任务队列
	CMarketTaskQueue m_marketImmediateTask; // 本市场当前即时任务队列（此任务序列一次执行完毕，无需等待）
	moodycamel::ConcurrentQueue<CMarketTaskPtr> m_qMarketDisplayTask{ 32 * 4 }; // 当前任务显示队列
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

	// 时间区
	//string m_strLocalMarketTimeZone{ "Europe/London" }; // 本市场当地时区名称 Asia/Shanghai, America/New_York, ...
	string m_strLocalMarketTimeZone{ "America/New_York" }; // 本市场当地时区名称 Asia/Shanghai, America/New_York, ...
	const time_zone* m_marketTimeZone{ nullptr }; // 本市场当地时区
	local_seconds m_marketClock; // 本市场的当地时钟 m_marketClock = m_marketDate + m_marketTimeOfDay

private:
	bool m_fResetMarket{ true }; // 重启系统标识
};

using CVirtualMarketWeakPtr = weak_ptr<CVirtualMarket>;
using CVirtualMarketPtr = shared_ptr<CVirtualMarket>;
extern vector<CVirtualMarketPtr> gl_vMarket; // 各市场指针的容器，只用于执行各市场的ScheduleTask
