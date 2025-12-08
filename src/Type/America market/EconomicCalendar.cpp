#include"pch.h"

#include"ConvertToString.h"
#include "EconomicCalendar.h"

#include "CharSetTransfer.h"

CEconomicCalendar::CEconomicCalendar() {
	m_strCountry = " ";
	m_iTime = 0;
	m_strEvent = " ";
	m_strImpact = " ";
	m_dActual = 0.0;
	m_dEstimate = 0.0;
	m_dPrev = 0.0;
	m_strUnit = " ";
}

void CEconomicCalendar::Load(const CSetEconomicCalendar& setEconomicCalendar) {
	m_strTime = T2Utf8(setEconomicCalendar.m_Time);
	m_strCountry = T2Utf8(setEconomicCalendar.m_Country);
	m_strEvent = T2Utf8(setEconomicCalendar.m_Event);
	m_strImpact = T2Utf8(setEconomicCalendar.m_Impact);
	m_dActual = _tstof(setEconomicCalendar.m_Actual);
	m_dEstimate = _tstof(setEconomicCalendar.m_Estimate);
	m_dPrev = _tstof(setEconomicCalendar.m_Prev);
	m_strUnit = T2Utf8(setEconomicCalendar.m_Unit);
}

void CEconomicCalendar::Append(CSetEconomicCalendar& setEconomicCalendar) const {
	setEconomicCalendar.AddNew();
	Save(setEconomicCalendar);
	setEconomicCalendar.Update();
}

void CEconomicCalendar::Save(CSetEconomicCalendar& setEconomicCalendar) const {
	setEconomicCalendar.m_Time = m_strTime.c_str();
	setEconomicCalendar.m_Country = m_strCountry.c_str();
	setEconomicCalendar.m_Event = m_strEvent.c_str();
	setEconomicCalendar.m_Impact = m_strImpact.c_str();
	setEconomicCalendar.m_Actual = ConvertValueToCString(m_dActual);
	setEconomicCalendar.m_Estimate = ConvertValueToCString(m_dEstimate);
	setEconomicCalendar.m_Prev = ConvertValueToCString(m_dPrev);
	setEconomicCalendar.m_Unit = m_strUnit.c_str();
}
