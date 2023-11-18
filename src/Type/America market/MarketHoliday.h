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
	CString m_strExchange;
	CString m_strEventName;
	long m_lDate;
	CString m_strTradingHour;
	CString m_strTimeZone;
};

using CMarketHolidayPtr = shared_ptr<CMarketHoliday>;
using CMarketHolidayVectorPtr = shared_ptr<vector<CMarketHolidayPtr>>;
