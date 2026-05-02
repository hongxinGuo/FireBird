#pragma once

#include<memory>

class CMarketHoliday final {
public:
	CMarketHoliday() {
		m_strExchange = "";
		m_strEventName = "";
		m_lDate = 0;
		m_strTradingHour = "";
		m_strTimeZone = "";
	}
	CMarketHoliday(const CMarketHoliday&) = default;
	CMarketHoliday& operator=(const CMarketHoliday&) = default;
	CMarketHoliday(CMarketHoliday&&) = default;
	CMarketHoliday& operator=(CMarketHoliday&&) = default;
	~CMarketHoliday() = default;

public:
	string m_strExchange;
	string m_strEventName;
	long m_lDate;
	string m_strTradingHour;
	string m_strTimeZone;
};

using CMarketHolidayPtr = shared_ptr<CMarketHoliday>;
using CMarketHolidaysPtr = shared_ptr<vector<CMarketHoliday>>;
