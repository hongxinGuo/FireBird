#pragma once

#include"SetEconomicCalendar.h"

using std::shared_ptr;
using std::vector;

class CEconomicCalendar final {
public:
	CEconomicCalendar();
	~CEconomicCalendar() = default;

	void Load(const CSetEconomicCalendar& setEconomicCalendar);
	void Append(CSetEconomicCalendar& setEconomicCalendar) const;
	void Save(CSetEconomicCalendar& setEconomicCalendar) const;

public:
	INT64 m_iTime; // 不存储这个数据
	CString m_strTime;
	CString m_strCountry;
	CString m_strEvent;
	CString m_strImpact;
	double m_dActual;
	double m_dEstimate;
	double m_dPrev;
	CString m_strUnit;
};

using CEconomicCalendarPtr = shared_ptr<CEconomicCalendar>;
using CEconomicCalendarVectorPtr = shared_ptr<vector<CEconomicCalendarPtr>>;
