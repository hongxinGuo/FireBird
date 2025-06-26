#pragma once

#include<memory>

class CMarketStatus final {
public:
	CMarketStatus() {
		m_strExchange = _T("");
		m_strHoliday = _T("");
		m_bOpen = false;
		m_strSession = _T("");
		m_strTimeZone = _T("");
		m_tt = 0;
	}
	~CMarketStatus() = default;

public:
	string m_strExchange;
	string m_strHoliday;
	bool m_bOpen;
	string m_strSession;
	string m_strTimeZone;
	time_t m_tt;
};

using CMarketStatusPtr = shared_ptr<CMarketStatus>;
using CMarketStatusesPtr = shared_ptr<vector<CMarketStatusPtr>>;
