#pragma once

#include"EconomicCalendar.h"

class CContainerFinnhubEconomicCalendar final {
public:
	CContainerFinnhubEconomicCalendar();
	~CContainerFinnhubEconomicCalendar() = default;
	void Reset();

	bool LoadDB();
	bool UpdateDB();
	bool Update(const vector<CEconomicCalendarPtr>& vEconomicCalendar);

	bool IsNeedUpdate() const noexcept {
		if (m_lLastTotalEconomicCalendar < m_vEconomicCalendar.size()) return true;
		return false;
	}

protected:
	vector<CEconomicCalendarPtr> m_vEconomicCalendar;
	map<CString, long> m_mapEconomicCalendar;
	long m_lLastTotalEconomicCalendar;
};
