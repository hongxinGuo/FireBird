#pragma once

#include"EconomicCalendar.h"

class CContainerFinnhubEconomicCalendar final {
public:
	CContainerFinnhubEconomicCalendar();
	~CContainerFinnhubEconomicCalendar() = default;
	void Reset(void);

	bool LoadDB(void);
	bool UpdateDB(void);
	bool Update(vector<CEconomicCalendarPtr> vEconomicCalendar);

	bool IsNeedUpdate(void) const noexcept {
		if (m_lLastTotalEconomicCalendar < m_vEconomicCalendar.size()) return true;
		return false;
	}

protected:
	vector<CEconomicCalendarPtr> m_vEconomicCalendar;
	map<CString, long> m_mapEconomicCalendar;
	long m_lLastTotalEconomicCalendar;
};
