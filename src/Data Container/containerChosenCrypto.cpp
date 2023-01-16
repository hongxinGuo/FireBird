#include "pch.h"

#include"WorldMarket.h"
#include"SetWorldChosenCrypto.h"
#include "containerChosenCrypto.h"
#include"FinnhubCryptoSymbol.h"

CContianerChosenCrypto::CContianerChosenCrypto() {
	m_lChosenCryptoPos = 0;
	Reset();
}

void CContianerChosenCrypto::Reset(void) {
	m_lChosenCryptoPos = 0;
}

bool CContianerChosenCrypto::LoadDB(void) {
	CSetWorldChosenCrypto setWorldChosenCrypto;
	CFinnhubCryptoSymbolPtr pCrypto = nullptr;

	setWorldChosenCrypto.Open();
	setWorldChosenCrypto.m_pDatabase->BeginTrans();
	while (!setWorldChosenCrypto.IsEOF()) {
		if (gl_pWorldMarket->IsFinnhubCryptoSymbol(setWorldChosenCrypto.m_Symbol)) {
			pCrypto = gl_pWorldMarket->GetFinnhubCryptoSymbol(setWorldChosenCrypto.m_Symbol);
			m_mapSymbol[setWorldChosenCrypto.m_Symbol] = m_mapSymbol.size();
			m_vStock.push_back(pCrypto);
		}
		setWorldChosenCrypto.MoveNext();
	}
	setWorldChosenCrypto.m_pDatabase->CommitTrans();
	setWorldChosenCrypto.Close();

	return true;
}
