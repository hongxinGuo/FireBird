#pragma once

class CMarketTime {
public:
	CMarketTime();
	CMarketTime(const CString& strLocalPosition);
	~CMarketTime() = default;

public:
	void Initialize(const CString& strLocalPosition, long OpenTime);

	long GetMarketTimeZone() const noexcept { return m_lMarketTimeZone; }
	long GetMarketTime() const noexcept { return m_lMarketTime; } //得到本市场的当地时间，格式为：hhmmss
	long GetMarketDate() const noexcept { return m_lMarketDate; } // 得到本市场的当地日期， 格式为：yyyymmdd
	long GetDayOfWeek() const noexcept { return m_tmMarket.tm_wday; } // days since Sunday - [0, 6]
	long GetMonthOfYear() const noexcept { return m_tmMarket.tm_mon + 1; }
	long GetDateOfMonth() const noexcept { return m_tmMarket.tm_mday; }
	long GetYear() const noexcept { return m_tmMarket.tm_year + 1900; }
	auto GetMarketTM() { return &m_tmMarket; }

	void CalculateTime() noexcept; // 计算本市场的各时间

	long CalculateNextTradeDate() noexcept;
	long CalculateCurrentTradeDate() noexcept; // 计算当前交易日。周一至周五为当日，周六和周日为周五
	long CalculateLastTradeDate() noexcept; // 计算当前交易日的上一个交易日。周二至周五为上一日，周六和周日为周四，周一为周五。

	// 时间函数
	tm GetMarketTime(time_t tUTC) const; // 得到本市场的时间（从UTC时间）
	time_t TransferToUTCTime(tm* tmMarketTime) const; // 将市场时间结构转化为UTC时间
	time_t TransferToUTCTime(long lMarketDate, long lMarketTime = 150000) const; // 将市场时间结构转化为UTC时间
	long GetMarketDate(time_t tUTC) const; // 得到本市场的日期

	long GetLastTradeDate() noexcept { return CalculateLastTradeDate(); }// 当前交易日的前一个交易日（从昨日开市时间至本日开市时间）
	long GetCurrentTradeDate() noexcept { return CalculateCurrentTradeDate(); }// 当前交易日（从本日九点半至次日开市时间）
	long GetNextTradeDate() noexcept { return CalculateNextTradeDate(); }// 下一个交易日（从次日开市时间至后日开市时间）

	bool IsWorkingDay() const noexcept;
	static bool IsWorkingDay(CTime timeCurrent) noexcept;
	static bool IsWorkingDay(long lDate) noexcept;

	CString GetStringOfLocalTime() const; // 得到本地时间的字符串
	CString GetStringOfMarketTime() const; // 得到本市场时间的字符串
	CString GetStringOfMarketDate() const;
	CString GetStringOfMarketDateTime() const;
	CString GetStringOfLocalDateTime() const;

	// 辅助函数
	long ConvertToDate(time_t tUTC) const noexcept;
	static long ConvertToDate(const tm* ptm) noexcept { return ((ptm->tm_year + 1900) * 10000 + (ptm->tm_mon + 1) * 100 + ptm->tm_mday); }
	static long ConvertToTime(const tm* ptm) noexcept { return (ptm->tm_hour * 10000 + ptm->tm_min * 100 + ptm->tm_sec); }

	time_t ConvertBufferToTime(CString strFormat, const char* BufferMarketTime);
	time_t ConvertStringToTime(CString strFormat, CString strMarketTime);

private:
	time_t GetMarketLocalTimeOffset(CString strLocalNameOfMarket);

public:
	// 测试用函数
	static void TEST_SetUTCTime(const time_t Time) noexcept { gl_tUTCTime = Time; }
	void TEST_SetFormattedMarketTime(const long lTime) noexcept { m_lMarketTime = lTime; } // 此函数只用于测试
	void TEST_SetMarketTM(const tm& tm_) noexcept { m_tmMarket = tm_; }
	void TEST_SetFormattedMarketDate(const long lDate) noexcept { m_lMarketDate = lDate; }

protected:
	long m_lMarketTimeZone{ 0 }; // 该市场的时区与GMT之差（以秒计，负值处于东十二区（超前），正值处于西十二区（滞后））。与_get_timezone函数相符。
	CString m_strLocalMarketTimeZone{ _T("") }; // 本市场当地时区名称 Asia/Shanghai, America/New_York, ...
	const chrono::time_zone* m_tzMarket{ nullptr }; // 本市场当地时区
	chrono::sys_info m_localMarketTimeZoneSystemInformation; // 当地时区系统信息
	chrono::local_info m_localMarketTimeZoneLocalInformation; // 当地时区本地信息

	// 以下时间日期为本市场的标准日期和时间（既非GMT时间也非软件使用时所处的当地时间，而是该市场所处地区的标准时间，如中国股市永远为东八区）。
	long m_lMarketDate{ 0 }; //本市场的日期
	long m_lMarketTime{ 0 }; // 本市场的时间
	long m_lMarketLastTradeDate{ 0 }; // 本市场的上次交易日
	long m_lMarketCurrentTradeDate{ 0 }; // 本市场当前交易日
	long m_lMarketNextTradeDate{ 0 }; // 本市场下一个交易日
	tm m_tmMarket{ 0, 0, 0, 1, 0, 1970 }; // 本市场时间结构

	long m_lOpenMarketTime{ 0 }; // 市场开市时间（由各具体市场实际确定）
};

using CMarketTimePtr = shared_ptr<CMarketTime>;
