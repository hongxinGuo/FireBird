#include "pch.h"

#include"SetWorldChosenCrypto.h"
#include "containerChosenCrypto.h"
#include"FinnhubCryptoSymbol.h"

CContainerChosenCrypto::CContainerChosenCrypto() {
	CContainerChosenCrypto::Reset();
}

void CContainerChosenCrypto::Reset() {
	CContainerVirtualStock::Reset();

	m_lChosenCryptoPos = 0;
}

bool CContainerChosenCrypto::LoadDB() {
	CSetWorldChosenCrypto setWorldChosenCrypto;
	CFinnhubCryptoSymbolPtr pCrypto = nullptr;

	setWorldChosenCrypto.Open();
	setWorldChosenCrypto.m_pDatabase->BeginTrans();
	while (!setWorldChosenCrypto.IsEOF()) {
		if (gl_dataFinnhubCryptoSymbol.IsSymbol(setWorldChosenCrypto.m_Symbol)) {
			pCrypto = gl_dataFinnhubCryptoSymbol.GetSymbol(setWorldChosenCrypto.m_Symbol);
			m_mapSymbol[setWorldChosenCrypto.m_Symbol] = m_mapSymbol.size();
			m_vStock.push_back(pCrypto);
		}
		setWorldChosenCrypto.MoveNext();
	}
	setWorldChosenCrypto.m_pDatabase->CommitTrans();
	setWorldChosenCrypto.Close();

	return true;
}
