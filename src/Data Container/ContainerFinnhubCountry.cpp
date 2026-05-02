#include "pch.h"
#include <algorithm>
#include<vector>
#include "ContainerFinnhubCountry.h"

CContainerFinnhubCountry::CContainerFinnhubCountry() { Reset(); }

void CContainerFinnhubCountry::Reset() {
	m_vCountry.resize(0);
	m_mapCountry.clear();
	m_llLastTotalCountry = 0;
}

void CContainerFinnhubCountry::Add(const CCountry& pCountry) {
	m_mapCountry[pCountry.m_strCountry] = m_vCountry.size();
	m_vCountry.push_back(pCountry);
}

bool CContainerFinnhubCountry::Delete(const CCountry& country) {
	if (!IsCountry(country.m_strCountry)) return false;

	m_mapCountry.erase(country.m_strCountry);
	auto it = std::ranges::find(m_vCountry, country.m_strCode2, &CCountry::m_strCode2);
	if (it != m_vCountry.end()) m_vCountry.erase(it);

	return true;
}

///////////////////////////////////////////////////////////////////////////
//
// 此种更新方法，是默认新的国家代码附加在最后。
//
//////////////////////////////////////////////////////////////////////////
void CContainerFinnhubCountry::UpdateDB() {
	if (m_llLastTotalCountry < m_vCountry.size()) {
		CSetCountry setCountry;
		setCountry.Open();
		setCountry.m_pDatabase->BeginTrans();
		for (auto l = m_llLastTotalCountry; l < m_vCountry.size(); l++) {
			const CCountry country = m_vCountry.at(l);
			country.Append(setCountry);
		}
		setCountry.m_pDatabase->CommitTrans();
		setCountry.Close();
		m_llLastTotalCountry = m_vCountry.size();
	}
}

bool CContainerFinnhubCountry::LoadDB() {
	CSetCountry setCountry;
	CCountryPtr pCountry = nullptr;

	setCountry.m_strSort = "[Country]";
	setCountry.Open();
	while (!setCountry.IsEOF()) {
		CCountry country;
		country.Load(setCountry);
		m_mapCountry[country.m_strCountry] = m_vCountry.size();
		m_vCountry.push_back(country);
		setCountry.MoveNext();
	}
	setCountry.Close();
	m_llLastTotalCountry = m_vCountry.size();

	return true;
}
