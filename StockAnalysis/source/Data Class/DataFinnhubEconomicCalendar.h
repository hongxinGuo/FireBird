#pragma once

#include"EconomicCalendar.h"

using namespace std;
#include<vector>
#include<map>

class CDataFinnhubEconomicCalendar : public CObject {
public:
	CDataFinnhubEconomicCalendar();
	~CDataFinnhubEconomicCalendar();
	void Reset(void);

	bool LoadDB(void);
	bool UpdateDB(void);
	bool Update(vector<CEconomicCalendarPtr> vEconomicCalendar);

	bool IsNeedUpdate(void) noexcept { if (m_lLastTotalEconomicCalendar < m_vEconomicCalendar.size()) return true; return false; }

protected:
	vector<CEconomicCalendarPtr> m_vEconomicCalendar;
	map<CString, long> m_mapEconomicCalendar;
	long m_lLastTotalEconomicCalendar;
};
