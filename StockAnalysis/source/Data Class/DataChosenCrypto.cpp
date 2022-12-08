#include "pch.h"

#include"WorldMarket.h"
#include"SetWorldChosenCrypto.h"
#include "DataChosenCrypto.h"

CDataChosenCrypto::CDataChosenCrypto() {
	Reset();
}

CDataChosenCrypto::~CDataChosenCrypto() {
}

void CDataChosenCrypto::Reset(void) {
	m_vWorldChosenCrypto.resize(0);
	m_mapWorldChosenCrypto.clear();
	m_lChosenCryptoPos = 0;
}

bool CDataChosenCrypto::LoadDB(void) {
	CSetWorldChosenCrypto setWorldChosenCrypto;
	CFinnhubCryptoSymbolPtr pCrypto = nullptr;

	setWorldChosenCrypto.Open();
	setWorldChosenCrypto.m_pDatabase->BeginTrans();
	while (!setWorldChosenCrypto.IsEOF()) {
		if (gl_pWorldMarket->IsFinnhubCryptoSymbol(setWorldChosenCrypto.m_Symbol)) {
			pCrypto = gl_pWorldMarket->GetFinnhubCryptoSymbol(setWorldChosenCrypto.m_Symbol);
			m_mapWorldChosenCrypto[setWorldChosenCrypto.m_Symbol] = m_mapWorldChosenCrypto.size();
			m_vWorldChosenCrypto.push_back(pCrypto);
		}
		setWorldChosenCrypto.MoveNext();
	}
	setWorldChosenCrypto.m_pDatabase->CommitTrans();
	setWorldChosenCrypto.Close();

	return true;
}