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
	m_lLastTotalForexExchange = 0;
}

bool CContainerFinnhubForexExchange::Delete(const CString& strForexExchange) {
	if (!IsExchange(strForexExchange)) return false;

	const auto it = std::ranges::find(m_vForexExchange.begin(), m_vForexExchange.end(), strForexExchange);
	m_vForexExchange.erase(it);
	m_mapForexExchange.erase(strForexExchange);

	return true;
}

void CContainerFinnhubForexExchange::Add(const CString& strForexExchange) {
	m_mapForexExchange[strForexExchange] = m_vForexExchange.size();
	m_vForexExchange.push_back(strForexExchange);
}

bool CContainerFinnhubForexExchange::LoadDB() {
	CSetFinnhubForexExchange setForexExchange;
	int i = 0;

	setForexExchange.Open();
	while (!setForexExchange.IsEOF()) {
		m_vForexExchange.push_back(setForexExchange.m_Code);
		m_mapForexExchange[setForexExchange.m_Code] = i++;
		setForexExchange.MoveNext();
	}
	setForexExchange.Close();
	m_lLastTotalForexExchange = static_cast<long>(m_vForexExchange.size());

	return true;
}

bool CContainerFinnhubForexExchange::UpdateDB() {
	if (m_lLastTotalForexExchange < m_vForexExchange.size()) {
		try {
			CSetFinnhubForexExchange setForexExchange;
			setForexExchange.Open();
			setForexExchange.m_pDatabase->BeginTrans();
			for (long l = m_lLastTotalForexExchange; l < m_vForexExchange.size(); l++) {
				setForexExchange.AddNew();
				setForexExchange.m_Code = m_vForexExchange.at(l);
				setForexExchange.Update();
			}
			setForexExchange.m_pDatabase->CommitTrans();
			setForexExchange.Close();
			m_lLastTotalForexExchange = static_cast<long>(m_vForexExchange.size());
		}
		catch (CException* e) {
			ReportInformationAndDeleteException(e);
		}
		return true;
	}
	return false;
}
