#pragma once

#include "globedef.h"
#include"VirtualDataSource.h"

class CVirtualMarket {
public:
	CVirtualMarket(void);
	// 不允许赋值。
	CVirtualMarket(const CVirtualMarket&) = delete;
	CVirtualMarket& operator=(const CVirtualMarket&) = delete;
	CVirtualMarket(const CVirtualMarket&&) noexcept = delete;
	CVirtualMarket& operator=(const CVirtualMarket&&) noexcept = delete;
	virtual ~CVirtualMarket(void) = default;

public:
	virtual bool SchedulingTask(void); // 由程序的定时器调度，大约每100毫秒一次
	// 申请并处理Data source的数据，被最终衍生类的SchedulingTask函数来调度。
	// 此函数在VirtualMarket中定义，但由最终衍生类来调用，因为lCurrentTime必须为该衍生类的当前市场时间。
	void RunDataSource(long lCurrentTime) const;

	bool SchedulingTaskPerSecond(long lSeconds); // 每秒调度一次
	bool SchedulingTaskPerMinute(long lSeconds, long lCurrentTime); // 每一分钟调度一次
	virtual void ResetMarket(void);
	virtual bool UpdateMarketInfo(void); // 更新本市场信息。

	// 时间函数
	tm TransferToMarketTime(time_t tUTC = gl_tUTC) const; // 得到本市场的时间（从UTC时间）
	time_t TransferToUTCTime(tm* tmMarketTime) const; // 将市场时间结构转化为UTC时间
	time_t TransferToUTCTime(long lMarketDate, long lMarketTime = 150000) const; // 将市场时间结构转化为UTC时间
	long TransferToMarketDate(time_t tUTC = gl_tUTC) const; // 得到本市场的日期

	long GetMarketTimeZone(void) const noexcept { return m_lMarketTimeZone; }
	CString GetMarketID(void) const noexcept { return m_strMarketId; }
	long GetMarketTime(void) const noexcept { return m_lMarketTime; } //得到本市场的当地时间，格式为：hhmmss
	long GetMarketDate(void) const noexcept { return m_lMarketDate; } // 得到本市场的当地日期， 格式为：yyyymmdd
	long GetDayOfWeek(void) const noexcept { return m_tmMarket.tm_wday; } // days since Sunday - [0, 6]
	long GetMonthOfYear(void) const noexcept { return m_tmMarket.tm_mon + 1; }
	long GetDateOfMonth(void) const noexcept { return m_tmMarket.tm_mday; }
	long GetYear(void) const noexcept { return m_tmMarket.tm_year + 1900; }

	long GetLastTradeDate(void) noexcept {
		CalculateLastTradeDate();
		return m_lMarketLastTradeDate;
	}

	bool IsWorkingDay(void) const noexcept;
	bool IsWorkingDay(CTime timeCurrent) const noexcept;
	bool IsWorkingDay(long lDate) const noexcept;

	virtual bool IsOrdinaryTradeTime(void) { return true; } // 日常交易时间
	virtual bool IsOrdinaryTradeTime(long) { return true; } // 参数为市场当前时间hhmmss
	virtual bool IsWorkingTime(void) { return true; } // 工作时间（日常交易时间 + 延长的交易时间）
	virtual bool IsWorkingTime(long) { return true; } // 参数为市场当前时间hhmmss
	virtual bool IsDummyTime(void) { return false; } // 空闲时间
	virtual bool IsDummyTime(long) { return false; } // 参数为市场当前时间hhmmss

	//bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays) const noexcept;
	//long GetNextDay(long lDate, long lTimeSpanDays = 1) const noexcept;
	//long GetPrevDay(long lDate, long lTimeSpanDays = 1) const noexcept;

	CString GetStringOfLocalTime(void) const; // 得到本地时间的字符串
	CString GetStringOfLocalDateTime(void) const;
	CString GetStringOfMarketTime(void) const; // 得到本市场时间的字符串
	CString GetStringOfMarketDate(void) const;
	CString GetStringOfMarketDateTime(void) const;

	void CalculateTime(void) noexcept; // 计算本市场的各时间
	void CalculateLastTradeDate(void) noexcept;
	void TaskResetMarketFlagAtMidnight(long lCurrentTime);

	bool IsReadyToRun(void) const noexcept { return m_fReadyToRun; }
	void SetReadyToRun(bool fFlag) noexcept { m_fReadyToRun = fFlag; }
	bool HaveResetMarketPermission(void) const noexcept { return m_fResetMarketPermission; }
	void SetResetMarketPermission(bool fFlag) noexcept { m_fResetMarketPermission = fFlag; }
	bool IsResetMarket(void) const noexcept { return m_fResetMarket; }
	void SetResetMarket(bool fFlag) noexcept { m_fResetMarket = fFlag; }

	virtual bool IsTimeToResetSystem(long) { return false; } // 默认永远处于非重启市场状态，继承类需要各自设置之
	virtual bool IsSystemReady(void) const noexcept { return m_fSystemReady; }
	virtual void SetSystemReady(const bool fFlag) noexcept { m_fSystemReady = fFlag; }

	virtual bool PreparingExitMarket(void) { return true; } // 准备退出本市场（完成系统退出前的准备工作）。

	// 存储数据源
	void StoreDataSource(CVirtualDataSourcePtr pDataSource) { m_vDataSource.push_back(pDataSource); }

public:
	// 测试用函数
	void TEST_SetUTCTime(time_t Time) noexcept { gl_tUTC = Time; }
	void TEST_SetFormattedMarketTime(long lTime) noexcept { m_lMarketTime = lTime; } // 此函数只用于测试
	void TEST_SetMarketTM(tm tm_) noexcept { m_tmMarket = tm_; }
	void TEST_SetFormattedMarketDate(long lDate) noexcept { m_lMarketDate = lDate; }

protected:
	// Finnhub.io提供的信息
	CString m_strCode;
	CString m_strName;
	CString m_strMic;
	CString m_strTimeZone;
	CString m_strHour;
	CString m_strCloseDate;
	CString m_strCountry;
	CString m_strSource;

	vector<CVirtualDataSourcePtr> m_vDataSource; // 本市场中的网络数据源。

	long m_lMarketTimeZone; // 该市场的时区与GMT之差（以秒计，负值处于东十二区（超前），正值处于西十二区（滞后））。与_get_timezone函数相符。
	CString m_strMarketId; // 该市场标识字符串

	// 以下时间日期为本市场的标准日期和时间（既非GMT时间也非软件使用时所处的当地时间，而是该市场所处地区的标准时间，如中国股市永远为东八区）。
	long m_lMarketDate; //本市场的日期
	long m_lMarketTime; // 本市场的时间
	long m_lMarketLastTradeDate; // 本市场的上次交易日期
	tm m_tmMarket{0, 0, 0, 1, 0, 1970}; // 本市场时间结构

	//系统状态区
	bool m_fSystemReady; // 市场初始态已经设置好

	time_t m_lastTimeSchedulingTask;
	int m_iCount1Hour; // 与五分钟每次的错开11秒钟，与一分钟每次的错开22秒钟
	int m_iCount5Minute; // 与一分钟每次的错开11秒钟
	int m_iCount1Minute; // 与10秒每次的错开1秒钟
	int m_iCount10Second;

private:
	time_t m_tLastTime;

	bool m_fReadyToRun; // 市场准备好运行标识。目前永远为真。
	bool m_fResetMarketPermission; // 允许重置系统（如果不断机多日运行的话，需要每日重置系统）初始值必须为真。
	bool m_fResetMarket; // 重启系统标识

	int m_i10SecondCounter; // 十秒一次的计数器
	int m_i1MinuteCounter; // 一分钟一次的计数器
	int m_i5MinuteCounter; // 五分钟一次的计数器
	int m_i1HourCounter; // 一小时一次的计数器
};

using CVirtualMarketPtr = shared_ptr<CVirtualMarket>;
extern vector<CVirtualMarketPtr> gl_vMarketPtr; // 各市场指针的容器，只用于执行各市场的SchedulingTask
