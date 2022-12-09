#include "pch.h"

#include "DataFinnhubForexExchange.h"
#include"SetFinnhubForexExchange.h"

CDataFinnhubForexExchange::CDataFinnhubForexExchange() {
	Reset();
}

void CDataFinnhubForexExchange::Reset(void) {
	m_vForexExchange.resize(0);
	m_mapForexExchange.clear();
	m_lLastTotalForexExchange = 0;
}

bool CDataFinnhubForexExchange::Delete(CString strForexExchange) {
	if (!IsForexExchange(strForexExchange)) return false;

	auto it = find(m_vForexExchange.begin(), m_vForexExchange.end(), strForexExchange);
	m_vForexExchange.erase(it);
	m_mapForexExchange.erase(strForexExchange);

	return true;
}

void CDataFinnhubForexExchange::Add(CString strForexExchange) {
	m_mapForexExchange[strForexExchange] = m_vForexExchange.size();
	m_vForexExchange.push_back(strForexExchange);
}

bool CDataFinnhubForexExchange::LoadDB(void) {
	CSetFinnhubForexExchange setForexExchange;
	int i = 0;

	setForexExchange.Open();
	while (!setForexExchange.IsEOF()) {
		m_vForexExchange.push_back(setForexExchange.m_Code);
		m_mapForexExchange[setForexExchange.m_Code] = i++;
		setForexExchange.MoveNext();
	}
	setForexExchange.Close();
	m_lLastTotalForexExchange = m_vForexExchange.size();

	return true;
}

bool CDataFinnhubForexExchange::UpdateDB(void) {
	CSetFinnhubForexExchange setForexExchange;

	if (m_lLastTotalForexExchange < m_vForexExchange.size()) {
		setForexExchange.Open();
		setForexExchange.m_pDatabase->BeginTrans();
		for (long l = m_lLastTotalForexExchange; l < m_vForexExchange.size(); l++) {
			setForexExchange.AddNew();
			setForexExchange.m_Code = m_vForexExchange.at(l);
			setForexExchange.Update();
		}
		setForexExchange.m_pDatabase->CommitTrans();
		setForexExchange.Close();
		m_lLastTotalForexExchange = m_vForexExchange.size();
		return true;
	}
	return false;
}
