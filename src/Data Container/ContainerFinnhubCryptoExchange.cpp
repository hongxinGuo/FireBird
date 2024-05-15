#include "pch.h"

#include "ContainerFinnhubCryptoExchange.h"

#include "InfoReport.h"
#include"SetFinnhubCryptoExchange.h"

CContainerFinnhubCryptoExchange::CContainerFinnhubCryptoExchange() {
	Reset();
}

void CContainerFinnhubCryptoExchange::Reset() {
	m_vCryptoExchange.resize(0);
	m_mapCryptoExchange.clear();
	m_lLastTotalCryptoExchange = 0;
}

bool CContainerFinnhubCryptoExchange::Delete(const string& sCryptoExchange) {
	if (!IsExchange(sCryptoExchange)) return false;

	const auto it = std::ranges::find(m_vCryptoExchange.begin(), m_vCryptoExchange.end(), sCryptoExchange);
	m_vCryptoExchange.erase(it);
	m_mapCryptoExchange.erase(sCryptoExchange);

	return true;
}

void CContainerFinnhubCryptoExchange::Add(const string& sCryptoExchange) {
	m_mapCryptoExchange[sCryptoExchange] = m_vCryptoExchange.size();
	m_vCryptoExchange.push_back(sCryptoExchange);
}

bool CContainerFinnhubCryptoExchange::LoadDB() {
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

bool CContainerFinnhubCryptoExchange::UpdateDB() {
	if (m_lLastTotalCryptoExchange < m_vCryptoExchange.size()) {
		try {
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
		}
		catch (CException* e) {
			ReportInformationAndDeleteException(e);
		}
		return true;
	}
	return false;
}
