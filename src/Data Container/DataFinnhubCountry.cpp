#include "pch.h"
#include "DataFinnhubCountry.h"

#include<memory>
using std::make_shared;

CDataFinnhubCountry::CDataFinnhubCountry() { Reset(); }

void CDataFinnhubCountry::Reset(void) {
	m_vCountry.resize(0);
	m_mapCountry.clear();
	m_lLastTotalCountry = 0;
}

void CDataFinnhubCountry::Add(CCountryPtr pCountry) {
	m_mapCountry[pCountry->m_strCountry] = m_vCountry.size();
	m_vCountry.push_back(pCountry);
}

bool CDataFinnhubCountry::Delete(CCountryPtr pCountry) {
	if (pCountry == nullptr) return false;
	if (!IsCountry(pCountry->m_strCountry)) return false;

	m_mapCountry.erase(pCountry->m_strCountry);
	auto it = find(m_vCountry.begin(), m_vCountry.end(), pCountry);
	m_vCountry.erase(it);

	return true;
}

///////////////////////////////////////////////////////////////////////////
//
// 此种更新方法，是默认新的国家代码附加在最后。
//
//////////////////////////////////////////////////////////////////////////
bool CDataFinnhubCountry::UpdateDB(void) {
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
	return true;
}

bool CDataFinnhubCountry::LoadDB(void) {
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
