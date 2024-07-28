#include "pch.h"
#include "ContainerTiingoCryptoSymbol.h"

#include "InfoReport.h"

CContainerTiingoCryptoSymbol::CContainerTiingoCryptoSymbol() {
	Reset();
}

void CContainerTiingoCryptoSymbol::Reset() {
	m_vTiingoCrypto.resize(0);
	m_mapTiingoCrypto.clear();
	m_lLastTotalTiingoCrypto = 0;
}

bool CContainerTiingoCryptoSymbol::Delete(const CTiingoCryptoSymbolPtr& pCryptoSymbol) {
	if (pCryptoSymbol == nullptr) return false;
	if (!IsSymbol(pCryptoSymbol->m_strTicker)) return false;

	m_vTiingoCrypto.erase(m_vTiingoCrypto.begin() + m_mapTiingoCrypto.at(pCryptoSymbol->m_strTicker));
	m_mapTiingoCrypto.erase(pCryptoSymbol->m_strTicker);

	return true;
}

void CContainerTiingoCryptoSymbol::Add(const CTiingoCryptoSymbolPtr& pCryptoSymbol) {
	ASSERT(!m_mapTiingoCrypto.contains(pCryptoSymbol->m_strTicker));
	m_mapTiingoCrypto[pCryptoSymbol->m_strTicker] = m_mapTiingoCrypto.size();
	m_vTiingoCrypto.push_back(pCryptoSymbol);
}

bool CContainerTiingoCryptoSymbol::LoadDB() {
	CSetTiingoCrypto setCryptoSymbol;

	setCryptoSymbol.m_strSort = _T("[Ticker]");
	setCryptoSymbol.Open();
	setCryptoSymbol.m_pDatabase->BeginTrans();
	while (!setCryptoSymbol.IsEOF()) {
		if (!IsSymbol(setCryptoSymbol.m_Ticker)) {
			const auto pSymbol = make_shared<CTiingoCryptoSymbol>();
			pSymbol->Load(setCryptoSymbol);
			Add(pSymbol);
		}
		else { setCryptoSymbol.Delete(); }
		setCryptoSymbol.MoveNext();
	}
	setCryptoSymbol.m_pDatabase->CommitTrans();
	setCryptoSymbol.Close();
	m_lLastTotalTiingoCrypto = m_vTiingoCrypto.size();

	return true;
}

bool CContainerTiingoCryptoSymbol::UpdateDB() {
	const long lTotalTiingoCryptoSymbol = m_vTiingoCrypto.size();

	if (m_lLastTotalTiingoCrypto < lTotalTiingoCryptoSymbol) {
		try {
			CSetTiingoCrypto setCryptoSymbol;
			setCryptoSymbol.Open();
			setCryptoSymbol.m_pDatabase->BeginTrans();
			for (long l = m_lLastTotalTiingoCrypto; l < lTotalTiingoCryptoSymbol; l++) {
				const CTiingoCryptoSymbolPtr pSymbol = m_vTiingoCrypto.at(l);
				pSymbol->Append(setCryptoSymbol);
			}
			setCryptoSymbol.m_pDatabase->CommitTrans();
			setCryptoSymbol.Close();
			m_lLastTotalTiingoCrypto = lTotalTiingoCryptoSymbol;
		}
		catch (CException* e) {
			ReportInformationAndDeleteException(e);
		}
	}

	return true;
}
