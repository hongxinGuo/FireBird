#include "pch.h"

#include "DataFinnhubCryptoExchange.h"
#include"SetFinnhubCryptoExchange.h"

CDataFinnhubCryptoExchange::CDataFinnhubCryptoExchange() {
	Reset();
}

void CDataFinnhubCryptoExchange::Reset(void) {
	m_vCryptoExchange.resize(0);
	m_mapCryptoExchange.clear();
	m_lLastTotalCryptoExchange = 0;
}

bool CDataFinnhubCryptoExchange::Delete(const string& sCryptoExchange) {
	if (!IsCryptoExchange(sCryptoExchange)) return false;

	auto it = find(m_vCryptoExchange.begin(), m_vCryptoExchange.end(), sCryptoExchange);
	m_vCryptoExchange.erase(it);
	m_mapCryptoExchange.erase(sCryptoExchange);

	return true;
}

void CDataFinnhubCryptoExchange::Add(const string& sCryptoExchange) {
	m_mapCryptoExchange[sCryptoExchange] = m_vCryptoExchange.size();
	m_vCryptoExchange.push_back(sCryptoExchange);
}

bool CDataFinnhubCryptoExchange::LoadDB(void) {
	CSetFinnhubCryptoExchange setCryptoExchange;
	int i = 0;

	setCryptoExchange.Open();
	while (!setCryptoExchange.IsEOF()) {
		m_vCryptoExchange.push_back(setCryptoExchange.m_Code.GetBuffer());
		m_mapCryptoExchange[setCryptoExchange.m_Code.GetBuffer()] = i++;
		setCryptoExchange.MoveNext();
	}
	setCryptoExchange.Close();
	m_lLastTotalCryptoExchange = static_cast<long>(m_vCryptoExchange.size());

	return true;
}

bool CDataFinnhubCryptoExchange::UpdateDB(void) {
	if (m_lLastTotalCryptoExchange < m_vCryptoExchange.size()) {
		CSetFinnhubCryptoExchange setCryptoExchange;
		setCryptoExchange.Open();
		setCryptoExchange.m_pDatabase->BeginTrans();
		for (long l = m_lLastTotalCryptoExchange; l < m_vCryptoExchange.size(); l++) {
			setCryptoExchange.AddNew();
			setCryptoExchange.m_Code = m_vCryptoExchange.at(l).c_str();
			setCryptoExchange.Update();
		}
		setCryptoExchange.m_pDatabase->CommitTrans();
		setCryptoExchange.Close();
		m_lLastTotalCryptoExchange = static_cast<long>(m_vCryptoExchange.size());
		return true;
	}
	return false;
}
