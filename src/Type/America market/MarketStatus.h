#pragma once

#include<memory>

class CMarketStatus final {
public:
	CMarketStatus() {
		m_strExchange = "";
		m_strHoliday = "";
		m_bOpen = false;
		m_strSession = "";
		m_strTimeZone = "";
		m_tt = 0;
	}
	CMarketStatus(const CMarketStatus&) = default;
	CMarketStatus& operator=(const CMarketStatus&) = default;
	CMarketStatus(CMarketStatus&&) noexcept = default;
	CMarketStatus& operator=(CMarketStatus&&) noexcept = default;

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
using CMarketStatusesPtr = shared_ptr<vector<CMarketStatus>>;
