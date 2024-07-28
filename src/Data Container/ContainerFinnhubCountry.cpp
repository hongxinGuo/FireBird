#include "pch.h"
#include "ContainerFinnhubCountry.h"

#include "InfoReport.h"

CContainerFinnhubCountry::CContainerFinnhubCountry() { Reset(); }

void CContainerFinnhubCountry::Reset() {
	m_vCountry.resize(0);
	m_mapCountry.clear();
	m_lLastTotalCountry = 0;
}

void CContainerFinnhubCountry::Add(const CCountryPtr& pCountry) {
	m_mapCountry[pCountry->m_strCountry] = m_vCountry.size();
	m_vCountry.push_back(pCountry);
}

bool CContainerFinnhubCountry::Delete(const CCountryPtr& pCountry) {
	if (pCountry == nullptr) return false;
	if (!IsCountry(pCountry->m_strCountry)) return false;

	m_mapCountry.erase(pCountry->m_strCountry);
	const auto it = std::ranges::find(m_vCountry.begin(), m_vCountry.end(), pCountry);
	m_vCountry.erase(it);

	return true;
}

///////////////////////////////////////////////////////////////////////////
//
// 此种更新方法，是默认新的国家代码附加在最后。
//
//////////////////////////////////////////////////////////////////////////
void CContainerFinnhubCountry::UpdateDB() {
	try {
		if (m_lLastTotalCountry < m_vCountry.size()) {
			CSetCountry setCountry;
			setCountry.Open();
			setCountry.m_pDatabase->BeginTrans();
			for (long l = m_lLastTotalCountry; l < m_vCountry.size(); l++) {
				const CCountryPtr pCountry = m_vCountry.at(l);
				pCountry->Append(setCountry);
			}
			setCountry.m_pDatabase->CommitTrans();
			setCountry.Close();
			m_lLastTotalCountry = m_vCountry.size();
		}
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

bool CContainerFinnhubCountry::LoadDB() {
	CSetCountry setCountry;
	CCountryPtr pCountry = nullptr;

	setCountry.m_strSort = _T("[Country]");
	setCountry.Open();
	while (!setCountry.IsEOF()) {
		pCountry = make_shared<CCountry>();
		pCountry->Load(setCountry);
		m_mapCountry[pCountry->m_strCountry] = m_vCountry.size();
		m_vCountry.push_back(pCountry);
		setCountry.MoveNext();
	}
	setCountry.Close();
	m_lLastTotalCountry = m_vCountry.size();

	return true;
}
