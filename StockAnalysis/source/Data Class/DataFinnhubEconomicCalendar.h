#pragma once

#include"EconomicCalendar.h"

#include<vector>
#include<map>

class CDataFinnhubEconomicCalendar final : public CObject {
public:
	CDataFinnhubEconomicCalendar();
	~CDataFinnhubEconomicCalendar() override = default;
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
