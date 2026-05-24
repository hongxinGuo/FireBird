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
	void Reserve(size_t lSize);

	bool LoadDB();
	bool UpdateDB();
	bool Update(const vector<CEconomicCalendar>& vEconomicCalendar);

	bool IsUpdateDB() const noexcept { return m_lLastTotalEconomicCalendar < m_vEconomicCalendar.size(); }

protected:
	vector<CEconomicCalendar> m_vEconomicCalendar;
	unordered_map<string, size_t> m_mapEconomicCalendar;
	size_t m_lLastTotalEconomicCalendar;
};
