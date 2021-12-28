#include "pch.h"
#include "DataTiingoCryptoSymbol.h"

CDataTiingoCryptoSymbol::CDataTiingoCryptoSymbol() {
	Reset();
}

CDataTiingoCryptoSymbol::~CDataTiingoCryptoSymbol() {
}

void CDataTiingoCryptoSymbol::Reset(void) {
	m_vTiingoCrypto.resize(0);
	m_mapTiingoCrypto.clear();
	m_lLastTotalTiingoCrypto = 0;
}

bool CDataTiingoCryptoSymbol::Delete(CTiingoCryptoSymbolPtr pCryptoSymbol) {
	if (pCryptoSymbol == nullptr) return false;
	if (!IsTiingoCryptoSymbol(pCryptoSymbol->m_strTicker)) return false;

	m_mapTiingoCrypto.erase(pCryptoSymbol->m_strTicker);
	auto it = find(m_vTiingoCrypto.begin(), m_vTiingoCrypto.end(), pCryptoSymbol);
	m_vTiingoCrypto.erase(it);

	return true;
}

void CDataTiingoCryptoSymbol::Add(CTiingoCryptoSymbolPtr pCryptoSymbol) {
	m_mapTiingoCrypto[pCryptoSymbol->m_strTicker] = m_mapTiingoCrypto.size();
	m_vTiingoCrypto.push_back(pCryptoSymbol);
}

bool CDataTiingoCryptoSymbol::LoadDB(void) {
	CSetTiingoCrypto setCryptoSymbol;
	CTiingoCryptoSymbolPtr pSymbol = nullptr;
	int i = 0;

	setCryptoSymbol.Open();
	while (!setCryptoSymbol.IsEOF()) {
		pSymbol = make_shared<CTiingoCryptoSymbol>();
		pSymbol->Load(setCryptoSymbol);
		m_vTiingoCrypto.push_back(pSymbol);
		m_mapTiingoCrypto[pSymbol->m_strTicker] = i++;
		setCryptoSymbol.MoveNext();
	}
	setCryptoSymbol.Close();
	m_lLastTotalTiingoCrypto = m_vTiingoCrypto.size();

	return true;
}

bool CDataTiingoCryptoSymbol::UpdateDB(void) {
	const long lTotalTiingoCryptoSymbol = m_vTiingoCrypto.size();
	CTiingoCryptoSymbolPtr pSymbol = nullptr;
	CSetTiingoCrypto setCryptoSymbol;
	bool fUpdateSymbol = false;

	if (m_lLastTotalTiingoCrypto < lTotalTiingoCryptoSymbol) {
		setCryptoSymbol.Open();
		setCryptoSymbol.m_pDatabase->BeginTrans();
		for (long l = m_lLastTotalTiingoCrypto; l < lTotalTiingoCryptoSymbol; l++) {
			pSymbol = m_vTiingoCrypto.at(l);
			pSymbol->Append(setCryptoSymbol);
		}
		setCryptoSymbol.m_pDatabase->CommitTrans();
		setCryptoSymbol.Close();
		m_lLastTotalTiingoCrypto = lTotalTiingoCryptoSymbol;
	}

	return true;
}