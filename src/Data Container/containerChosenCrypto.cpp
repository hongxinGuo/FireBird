#include "pch.h"
module;
#include"SystemData.h"
module FireBird.Container.Chosen.Crypto;

import FireBird.Stock.FinnhubCrypto;
import FireBird.Set.WorldChosenCrypto;

CContainerChosenCrypto::CContainerChosenCrypto() {
	CContainerChosenCrypto::Reset();
}

void CContainerChosenCrypto::Reset() {
	CContainerVirtualStock::Reset();

	m_lChosenCryptoPos = 0;
}

bool CContainerChosenCrypto::LoadDB() {
	CSetWorldChosenCrypto setWorldChosenCrypto;
	CFinnhubCryptoPtr pCrypto = nullptr;

	setWorldChosenCrypto.Open();
	setWorldChosenCrypto.m_pDatabase->BeginTrans();
	while (!setWorldChosenCrypto.IsEOF()) {
		if (gl_dataFinnhubCryptoSymbol.IsSymbol(setWorldChosenCrypto.m_Symbol)) {
			pCrypto = gl_dataFinnhubCryptoSymbol.GetSymbol(setWorldChosenCrypto.m_Symbol);
			m_mapSymbol[setWorldChosenCrypto.m_Symbol] = m_mapSymbol.size();
			m_vStock.push_back(pCrypto);
		}
		else {
			setWorldChosenCrypto.Delete(); // 删除已不存在的代码
		}
		setWorldChosenCrypto.MoveNext();
	}
	setWorldChosenCrypto.m_pDatabase->CommitTrans();
	setWorldChosenCrypto.Close();

	return true;
}
