#include "pch.h"

#include"SetWorldChosenCrypto.h"
#include "containerChosenCrypto.h"
#include"FinnhubCrypto.h"

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
		if (gl_dataFinnhubCryptoSymbol.IsSymbol(setWorldChosenCrypto.m_Symbol.GetString())) {
			pCrypto = gl_dataFinnhubCryptoSymbol.GetItem(setWorldChosenCrypto.m_Symbol.GetString());
			m_mapSymbol[setWorldChosenCrypto.m_Symbol.GetString()] = m_mapSymbol.size();
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
