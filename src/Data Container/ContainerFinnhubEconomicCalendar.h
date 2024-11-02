#pragma once

#include"EconomicCalendar.h"

using std::map;

class CContainerFinnhubEconomicCalendar final {
public:
	CContainerFinnhubEconomicCalendar();
	CContainerFinnhubEconomicCalendar(const CContainerFinnhubEconomicCalendar& other) = delete;
	CContainerFinnhubEconomicCalendar(CContainerFinnhubEconomicCalendar&& other) noexcept = delete;
	CContainerFinnhubEconomicCalendar& operator=(const CContainerFinnhubEconomicCalendar& other) = delete;
	CContainerFinnhubEconomicCalendar& operator=(CContainerFinnhubEconomicCalendar&& other) noexcept = delete;
	~CContainerFinnhubEconomicCalendar() = default;
	void Reset();

	bool LoadDB();
	bool UpdateDB();
	bool Update(const vector<CEconomicCalendarPtr>& vEconomicCalendar);

	bool IsUpdateDB() const noexcept { return m_lLastTotalEconomicCalendar < m_vEconomicCalendar.size(); }

protected:
	vector<CEconomicCalendarPtr> m_vEconomicCalendar;
	map<CString, size_t> m_mapEconomicCalendar;
	size_t m_lLastTotalEconomicCalendar;
};
