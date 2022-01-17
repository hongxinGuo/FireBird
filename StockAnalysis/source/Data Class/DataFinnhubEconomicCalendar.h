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

protected:
	vector<CEconomicCalendarPtr> m_vEconomicCalendar;
	map<CString, long> m_mapEconomicCalendar;
	long m_lLastTotalEconomicCalendar;
};
