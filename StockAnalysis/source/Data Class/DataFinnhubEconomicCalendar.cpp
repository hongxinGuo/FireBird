#include "pch.h"
#include "DataFinnhubEconomicCalendar.h"
#include"SetEconomicCalendar.h"

CDataFinnhubEconomicCalendar::CDataFinnhubEconomicCalendar() {
	Reset();
}

CDataFinnhubEconomicCalendar::~CDataFinnhubEconomicCalendar() {
}

void CDataFinnhubEconomicCalendar::Reset(void) {
	m_lLastTotalEconomicCalendar = 0;
	m_mapEconomicCalendar.clear();
	m_vEconomicCalendar.resize(0);
}

bool CDataFinnhubEconomicCalendar::LoadDB(void) {
	CSetEconomicCalendar setEconomicCalendar;
	CEconomicCalendarPtr pEconomicCalendar = nullptr;
	CString strSymbol = _T("");

	setEconomicCalendar.Open();
	while (!setEconomicCalendar.IsEOF()) {
		pEconomicCalendar = make_shared<CEconomicCalendar>();
		pEconomicCalendar->Load(setEconomicCalendar);
		strSymbol = pEconomicCalendar->m_strCountry + pEconomicCalendar->m_strEvent + pEconomicCalendar->m_strTime;
		m_mapEconomicCalendar[strSymbol] = m_vEconomicCalendar.size();
		m_vEconomicCalendar.push_back(pEconomicCalendar);
		setEconomicCalendar.MoveNext();
	}
	setEconomicCalendar.Close();
	m_lLastTotalEconomicCalendar = m_vEconomicCalendar.size();

	return true;
}

bool CDataFinnhubEconomicCalendar::UpdateDB(void) {
	const long lTotalEconomicCalendar = m_vEconomicCalendar.size();
	CEconomicCalendarPtr pEconomicCalendar = nullptr;
	CSetEconomicCalendar setEconomicCalendar;

	if (m_lLastTotalEconomicCalendar < m_vEconomicCalendar.size()) {
		setEconomicCalendar.Open();
		setEconomicCalendar.m_pDatabase->BeginTrans();
		for (long l = m_lLastTotalEconomicCalendar; l < m_vEconomicCalendar.size(); l++) {
			pEconomicCalendar = m_vEconomicCalendar.at(l);
			pEconomicCalendar->Append(setEconomicCalendar);
		}
		setEconomicCalendar.m_pDatabase->CommitTrans();
		setEconomicCalendar.Close();
		m_lLastTotalEconomicCalendar = m_vEconomicCalendar.size();
	}

	return true;
}

bool CDataFinnhubEconomicCalendar::Update(vector<CEconomicCalendarPtr> vEconomicCalendar) {
	CString strSymbol = _T("");

	for (auto& pEconomicCalendar : vEconomicCalendar) {
		strSymbol = pEconomicCalendar->m_strCountry + pEconomicCalendar->m_strEvent + pEconomicCalendar->m_strTime;
		if (m_mapEconomicCalendar.find(strSymbol) == m_mapEconomicCalendar.end()) { // ���¼���
			m_mapEconomicCalendar[strSymbol] = m_vEconomicCalendar.size();
			m_vEconomicCalendar.push_back(pEconomicCalendar);
		}
	}
	return true;
}