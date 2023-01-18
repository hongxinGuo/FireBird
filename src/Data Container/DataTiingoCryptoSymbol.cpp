#include "pch.h"
#include "DataTiingoCryptoSymbol.h"

#include<memory>
using std::make_shared;

CDataTiingoCryptoSymbol::CDataTiingoCryptoSymbol() { Reset(); }

void CDataTiingoCryptoSymbol::Reset(void) {
	m_vTiingoCrypto.resize(0);
	m_mapTiingoCrypto.clear();
	m_lLastTotalTiingoCrypto = 0;
}

bool CDataTiingoCryptoSymbol::Delete(CTiingoCryptoSymbolPtr pCryptoSymbol) {
	if (pCryptoSymbol == nullptr) return false;
	if (!IsTiingoCryptoSymbol(pCryptoSymbol->m_strTicker)) return false;

	m_vTiingoCrypto.erase(m_vTiingoCrypto.begin() + m_mapTiingoCrypto.at(pCryptoSymbol->m_strTicker));
	m_mapTiingoCrypto.erase(pCryptoSymbol->m_strTicker);

	return true;
}

void CDataTiingoCryptoSymbol::Add(CTiingoCryptoSymbolPtr pCryptoSymbol) {
	ASSERT(!m_mapTiingoCrypto.contains(pCryptoSymbol->m_strTicker));
	m_mapTiingoCrypto[pCryptoSymbol->m_strTicker] = m_mapTiingoCrypto.size();
	m_vTiingoCrypto.push_back(pCryptoSymbol);
}

bool CDataTiingoCryptoSymbol::LoadDB(void) {
	CSetTiingoCrypto setCryptoSymbol;

	setCryptoSymbol.m_strSort = _T("[Ticker]");
	setCryptoSymbol.Open();
	setCryptoSymbol.m_pDatabase->BeginTrans();
	while (!setCryptoSymbol.IsEOF()) {
		if (!IsTiingoCryptoSymbol(setCryptoSymbol.m_Ticker)) {
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

bool CDataTiingoCryptoSymbol::UpdateDB(void) {
	const long lTotalTiingoCryptoSymbol = m_vTiingoCrypto.size();

	if (m_lLastTotalTiingoCrypto < lTotalTiingoCryptoSymbol) {
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

	return true;
}
