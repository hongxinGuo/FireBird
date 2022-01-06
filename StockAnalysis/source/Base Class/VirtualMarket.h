#pragma once

#include"time.h"

using namespace std;
#include<memory>

class CVirtualMarket : public CObject {
public:
	DECLARE_DYNCREATE(CVirtualMarket)
	CVirtualMarket(void);
	virtual ~CVirtualMarket(void);

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

public:
	virtual bool SchedulingTask(void); // 由程序的定时器调度，大约每100毫秒一次
	virtual void ResetMarket(void);
	virtual bool UpdateMarketInfo(void); // 更新本市场信息。

	bool SchedulingTaskPerSecond(long lSecondNumber); // 每秒调度一次
	bool SchedulingTaskPerMinute(long lSecondNumber, long lCurrentTime); // 每一分钟调度一次

	// 时间函数
	tm TransferToMarketTime(time_t tUTC = sm_tUTC); // 得到本市场的时间（从UTC时间）
	time_t TransferToUTCTime(tm* tmMarketTime); // 将市场时间结构转化为UTC时间
	time_t TransferToUTCTime(long lMarketDate, long lMarketTime = 150000); // 将市场时间结构转化为UTC时间
	long TransferToMarketDate(time_t tUTC = sm_tUTC); // 得到本市场的日期

	long GetMarketTimeZone(void) const noexcept { return m_lMarketTimeZone; }
	CString GetMarketID(void) const { return m_strMarketId; }
	time_t GetUTCTime(void) const noexcept { return sm_tUTC; }
	long GetMarketTime(void) const noexcept { return m_lMarketTime; } //得到本市场的当地时间，格式为：hhmmss
	long GetMarketDate(void) const noexcept { return m_lMarketDate; }// 得到本市场的当地日期， 格式为：yyyymmdd
	long GetDayOfWeek(void) const noexcept { return m_tmMarket.tm_wday; } // days since Sunday - [0, 6]
	long GetMonthOfYear(void) const noexcept { return m_tmMarket.tm_mon + 1; }
	long GetDateOfMonth(void) const noexcept { return m_tmMarket.tm_mday; }
	long GetYear(void) const noexcept { return m_tmMarket.tm_year + 1900; }
	long GetLastTradeDate(void) noexcept { CalculateLastTradeDate(); return m_lMarketLastTradeDate; }
	bool IsWorkingDay(void) const noexcept;
	bool IsWorkingDay(CTime timeCurrent) const noexcept;
	bool IsWorkingDay(long lDate) const noexcept;

	bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays) const noexcept;
	long GetNextDay(long lDate, long lTimeSpanDays = 1) const noexcept;
	long GetPrevDay(long lDate, long lTimeSpanDays = 1) const noexcept;

	CString GetStringOfLocalTime(void) const; // 得到本地时间的字符串
	CString GetStringOfLocalDateTime(void) const;
	CString GetStringOfMarketTime(void); // 得到本市场时间的字符串
	CString GetStringOfMarketDate(void) const;
	CString GetStringOfMarketDateTime(void);
	CString GetStringOfDate(long lDate) const;

	void CalculateTime(void) noexcept;// 计算本市场的各时间
	void CalculateLastTradeDate(void) noexcept;
	void TaskResetMarketFlagAtMidnight(long lCurrentTime);

	bool IsReadyToRun(void) const noexcept { return m_fReadyToRun; }
	void SetReadyToRun(bool fFlag) noexcept { m_fReadyToRun = fFlag; }
	bool IsPermitResetMarket(void) const noexcept { return m_fPermitResetMarket; }
	void SetPermitResetMarket(bool fFlag) noexcept { m_fPermitResetMarket = fFlag; } // 此函数只用于测试时使用
	bool IsResetMarket(void) const noexcept { return m_fResetMarket; }
	void SetResetMarket(bool fFlag) noexcept { m_fResetMarket = fFlag; }

	virtual bool IsTimeToResetSystem(long) { return false; } // 默认永远处于非重启市场状态，继承类需要各自设置之
	virtual bool IsSystemReady(void) const noexcept { return m_fSystemReady; }
	virtual void SetSystemReady(bool fFlag) noexcept { m_fSystemReady = fFlag; }

	virtual bool PreparingExitMarket(void) { return true; } // 准备退出本市场（完成系统退出前的准备工作）。

public:
	// 测试用函数
	void __TEST_SetUTCTime(time_t Time) noexcept { sm_tUTC = Time; }
	void __TEST_SetFormatedMarketTime(long lTime) noexcept { m_lMarketTime = lTime; }// 此函数只用于测试
	void __TEST_SetMarketTM(tm tm_) noexcept { m_tmMarket = tm_; }
	void __TEST_SetFormatedMarketDate(long lDate) noexcept { m_lMarketDate = lDate; }

public:
	// Finnhub.io提供的信息
	CString m_strCode;
	CString m_strName;
	CString m_strMic;
	CString m_strTimeZone;
	CString m_strHour;
	CString m_strCloseDate;
	CString m_strCountry;
	CString m_strSource;

protected:
	long m_lMarketTimeZone; // 该市场的时区与GMT之差（以秒计，负值处于东十二区（超前），正值处于西十二区（滞后））。与_get_timezone函数相符。
	CString m_strMarketId; // 该市场标识字符串

	static time_t sm_tUTC; // 软件运行时的UTC时间。所有的市场都使用同一个UTC时间，故而为静态数据。

	// 以下时间日期为本市场的标准日期和时间（既非GMT时间也非软件使用时所处的当地时间，而是该市场所处地区的标准时间，如中国股市永远为东八区）。
	long m_lMarketDate; //本市场的日期
	long m_lMarketTime; // 本市场的时间
	long m_lMarketLastTradeDate; // 本市场的上次交易日期
	tm m_tmMarket; // 本市场时间结构

	//系统状态区
	bool m_fSystemReady; // 市场初始态已经设置好

private:
	bool m_fReadyToRun; // 市场准备好运行标识。目前永远为真。
	bool m_fPermitResetMarket; // 允许重置系统（如果不断机多日运行的话，需要每日重置系统）初始值必须为真。
	bool m_fResetMarket; // 重启系统标识

	int m_i10SecondCounter;  // 十秒一次的计数器
	int m_i1MinuteCounter;  // 一分钟一次的计数器
	int m_i5MinuteCounter;  // 五分钟一次的计数器
	int m_i1HourCounter;  // 一小时一次的计数器

	time_t m_timeLast; // 上次运行时的市场时间
};

typedef shared_ptr<CVirtualMarket> CVirtualMarketPtr;
