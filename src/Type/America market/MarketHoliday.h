#pragma once

#include<memory>

class CMarketHoliday final {
public:
	CMarketHoliday() {
		m_strExchange = _T("");
		m_strHoliday = _T("");
		m_bOpen = false;
		m_strSession = _T("");
		m_strTimeZone = _T("");
		m_tt = 0;
	}
	~CMarketHoliday() = default;

public:
	CString m_strExchange;
	CString m_strHoliday;
	bool m_bOpen;
	CString m_strSession;
	CString m_strTimeZone;
	time_t m_tt;
};

using CMarketHolidayPtr = shared_ptr<CMarketHoliday>;
using CMarketHolidayVectorPtr = shared_ptr<vector<CMarketHolidayPtr>>;
