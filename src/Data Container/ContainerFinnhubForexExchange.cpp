#include "pch.h"

#include "ContainerFinnhubForexExchange.h"

#include "InfoReport.h"
#include"SetFinnhubForexExchange.h"

CContainerFinnhubForexExchange::CContainerFinnhubForexExchange() {
	Reset();
}

void CContainerFinnhubForexExchange::Reset() {
	m_vForexExchange.resize(0);
	m_mapForexExchange.clear();
	m_llLastTotalForexExchange = 0;
}

bool CContainerFinnhubForexExchange::Delete(const string& strForexExchange) {
	if (!IsExchange(strForexExchange)) return false;

	const auto it = std::ranges::find(m_vForexExchange.begin(), m_vForexExchange.end(), strForexExchange);
	m_vForexExchange.erase(it);
	m_mapForexExchange.erase(strForexExchange);

	return true;
}

void CContainerFinnhubForexExchange::Add(const string& strForexExchange) {
	m_mapForexExchange[strForexExchange] = m_vForexExchange.size();
	m_vForexExchange.push_back(strForexExchange);
}

bool CContainerFinnhubForexExchange::LoadDB() {
	CSetFinnhubForexExchange setForexExchange;
	int i = 0;

	setForexExchange.Open();
	while (!setForexExchange.IsEOF()) {
		m_vForexExchange.push_back(setForexExchange.m_Code.GetString());
		m_mapForexExchange[setForexExchange.m_Code.GetString()] = i++;
		setForexExchange.MoveNext();
	}
	setForexExchange.Close();
	m_llLastTotalForexExchange = m_vForexExchange.size();

	return true;
}

bool CContainerFinnhubForexExchange::UpdateDB() {
	if (m_llLastTotalForexExchange < m_vForexExchange.size()) {
		try {
			CSetFinnhubForexExchange setForexExchange;
			setForexExchange.Open();
			setForexExchange.m_pDatabase->BeginTrans();
			for (auto l = m_llLastTotalForexExchange; l < m_vForexExchange.size(); l++) {
				setForexExchange.AddNew();
				setForexExchange.m_Code = m_vForexExchange.at(l).c_str();
				setForexExchange.Update();
			}
			setForexExchange.m_pDatabase->CommitTrans();
			setForexExchange.Close();
			m_llLastTotalForexExchange = m_vForexExchange.size();
		} catch (CException& e) {
			ReportInformation(e);
		}
		return true;
	}
	return false;
}
