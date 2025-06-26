#pragma once

#include<memory>

class CMarketHoliday final {
public:
	CMarketHoliday() {
		m_strExchange = _T("");
		m_strEventName = _T("");
		m_lDate = 0;
		m_strTradingHour = _T("");
		m_strTimeZone = _T("");
	}
	~CMarketHoliday() = default;

public:
	string m_strExchange;
	string m_strEventName;
	long m_lDate;
	string m_strTradingHour;
	string m_strTimeZone;
};

using CMarketHolidayPtr = shared_ptr<CMarketHoliday>;
using CMarketHolidaysPtr = shared_ptr<vector<CMarketHolidayPtr>>;
