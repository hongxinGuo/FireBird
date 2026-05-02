#include "pch.h"
#include "ContainerFinnhubEconomicCalendar.h"
#include"SetEconomicCalendar.h"

CContainerFinnhubEconomicCalendar::CContainerFinnhubEconomicCalendar() {
	Reset();
}

void CContainerFinnhubEconomicCalendar::Reset() {
	m_lLastTotalEconomicCalendar = 0;
	m_mapEconomicCalendar.clear();
	m_vEconomicCalendar.resize(0);
}

bool CContainerFinnhubEconomicCalendar::LoadDB() {
	CSetEconomicCalendar setEconomicCalendar;
	CEconomicCalendarPtr pEconomicCalendar = nullptr;

	setEconomicCalendar.Open();
	while (!setEconomicCalendar.IsEOF()) {
		CEconomicCalendar economicCalendar;
		economicCalendar.Load(setEconomicCalendar);
		string strSymbol = economicCalendar.m_strCountry + economicCalendar.m_strEvent + economicCalendar.m_strTime;
		m_mapEconomicCalendar[strSymbol] = m_vEconomicCalendar.size();
		m_vEconomicCalendar.push_back(economicCalendar);
		setEconomicCalendar.MoveNext();
	}
	setEconomicCalendar.Close();
	m_lLastTotalEconomicCalendar = m_vEconomicCalendar.size();

	return true;
}

bool CContainerFinnhubEconomicCalendar::UpdateDB() {
	//return true; //todo 经济数据中有非法字符，暂时不存储了。
	if (m_lLastTotalEconomicCalendar < m_vEconomicCalendar.size()) {
		CSetEconomicCalendar setEconomicCalendar;
		setEconomicCalendar.Open();
		setEconomicCalendar.m_pDatabase->BeginTrans();
		for (auto l = m_lLastTotalEconomicCalendar; l < m_vEconomicCalendar.size(); l++) {
			const CEconomicCalendar economicCalendar = m_vEconomicCalendar.at(l);
			economicCalendar.Append(setEconomicCalendar);
		}
		setEconomicCalendar.m_pDatabase->CommitTrans();
		setEconomicCalendar.Close();
		m_lLastTotalEconomicCalendar = m_vEconomicCalendar.size();
	}

	return true;
}

bool CContainerFinnhubEconomicCalendar::Update(const vector<CEconomicCalendar>& vEconomicCalendar) {
	for (auto& economicCalendar : vEconomicCalendar) {
		string strSymbol = economicCalendar.m_strCountry + economicCalendar.m_strEvent + economicCalendar.m_strTime;
		if (!m_mapEconomicCalendar.contains(strSymbol)) {	// 新事件？
			m_mapEconomicCalendar[strSymbol] = m_vEconomicCalendar.size();
			m_vEconomicCalendar.push_back(economicCalendar);
		}
	}
	return true;
}
