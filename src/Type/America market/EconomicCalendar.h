#pragma once

class CEconomicCalendar final {
public:
	CEconomicCalendar();
	CEconomicCalendar(const CEconomicCalendar&) = default;
	CEconomicCalendar& operator=(const CEconomicCalendar&) = default;
	CEconomicCalendar(CEconomicCalendar&&) noexcept = default;
	CEconomicCalendar& operator=(CEconomicCalendar&&) noexcept = default;
	~CEconomicCalendar() = default;

public:
	string m_strTime;
	string m_strCountry;
	string m_strEvent;
	string m_strImpact;
	double m_dActual;
	double m_dEstimate;
	double m_dPrev;
	string m_strUnit;
};

using CEconomicCalendarPtr = shared_ptr<CEconomicCalendar>;
using CEconomicCalendarsPtr = shared_ptr<vector<CEconomicCalendar>>;
