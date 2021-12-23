#include "pch.h"

#include "DataFinnhubCryptoExchange.h"
#include"SetFinnhubCryptoExchange.h"

CDataFinnhubCryptoExchange::CDataFinnhubCryptoExchange() {
	Reset();
}

CDataFinnhubCryptoExchange::~CDataFinnhubCryptoExchange() {
}

void CDataFinnhubCryptoExchange::Reset(void) {
	m_vCryptoExchange.resize(0);
	m_mapCryptoExchange.clear();
	m_lLastTotalCryptoExchange = 0;
}

bool CDataFinnhubCryptoExchange::Delete(CString strCryptoExchange) {
	if (!IsCryptoExchange(strCryptoExchange)) return false;

	auto it = find(m_vCryptoExchange.begin(), m_vCryptoExchange.end(), strCryptoExchange);
	m_vCryptoExchange.erase(it);
	m_mapCryptoExchange.erase(strCryptoExchange);

	return true;
}

void CDataFinnhubCryptoExchange::Add(CString strCryptoExchange) {
	m_mapCryptoExchange[strCryptoExchange] = m_vCryptoExchange.size();
	m_vCryptoExchange.push_back(strCryptoExchange);
}

bool CDataFinnhubCryptoExchange::LoadDB(void) {
	CSetFinnhubCryptoExchange setCryptoExchange;
	int i = 0;

	setCryptoExchange.Open();
	while (!setCryptoExchange.IsEOF()) {
		m_vCryptoExchange.push_back(setCryptoExchange.m_Code);
		m_mapCryptoExchange[setCryptoExchange.m_Code] = i++;
		setCryptoExchange.MoveNext();
	}
	setCryptoExchange.Close();
	m_lLastTotalCryptoExchange = m_vCryptoExchange.size();

	return true;
}

bool CDataFinnhubCryptoExchange::UpdateDB(void) {
	CSetFinnhubCryptoExchange setCryptoExchange;

	if (m_lLastTotalCryptoExchange < m_vCryptoExchange.size()) {
		setCryptoExchange.Open();
		setCryptoExchange.m_pDatabase->BeginTrans();
		for (long l = m_lLastTotalCryptoExchange; l < m_vCryptoExchange.size(); l++) {
			setCryptoExchange.AddNew();
			setCryptoExchange.m_Code = m_vCryptoExchange.at(l);
			setCryptoExchange.Update();
		}
		setCryptoExchange.m_pDatabase->CommitTrans();
		setCryptoExchange.Close();
		m_lLastTotalCryptoExchange = m_vCryptoExchange.size();
		return true;
	}
	return false;
}