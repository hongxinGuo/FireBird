#include "pch.h"

#include"globedef.h"
#include"SetWorldChoicedCrypto.h"
#include "DataChoicedCrypto.h"

CDataChoicedCrypto::CDataChoicedCrypto() {
	Reset();
}

CDataChoicedCrypto::~CDataChoicedCrypto() {
}

void CDataChoicedCrypto::Reset(void) {
	m_vWorldChoicedCrypto.resize(0);
	m_mapWorldChoicedCrypto.clear();
	m_lChoicedCryptoPos = 0;
}

bool CDataChoicedCrypto::LoadDB(void) {
	CSetWorldChoicedCrypto setWorldChoicedCrypto;
	CFinnhubCryptoSymbolPtr pCrypto = nullptr;

	setWorldChoicedCrypto.Open();
	setWorldChoicedCrypto.m_pDatabase->BeginTrans();
	while (!setWorldChoicedCrypto.IsEOF()) {
		if (gl_pWorldMarket->IsFinnhubCryptoSymbol(setWorldChoicedCrypto.m_Symbol)) {
			pCrypto = gl_pWorldMarket->GetCryptoSymbol(setWorldChoicedCrypto.m_Symbol);
			m_mapWorldChoicedCrypto[setWorldChoicedCrypto.m_Symbol] = m_mapWorldChoicedCrypto.size();
			m_vWorldChoicedCrypto.push_back(pCrypto);
		}
		setWorldChoicedCrypto.MoveNext();
	}
	setWorldChoicedCrypto.m_pDatabase->CommitTrans();
	setWorldChoicedCrypto.Close();

	return true;
}