#include "pch.h"
module;
module FireBird.Container.Chosen.Forex;

import FireBird.Set.WorldChosenForex;
import FireBird.Stock.FinnhubForex;
import FireBird.System.Data;

CContainerChosenForex::CContainerChosenForex() {
	CContainerChosenForex::Reset();
}

void CContainerChosenForex::Reset() {
	CContainerVirtualStock::Reset();

	m_lChosenForexPos = 0;
}

bool CContainerChosenForex::LoadDB() {
	CSetWorldChosenForex setWorldChosenForex;
	CForexSymbolPtr pForex = nullptr;

	setWorldChosenForex.Open();
	setWorldChosenForex.m_pDatabase->BeginTrans();
	while (!setWorldChosenForex.IsEOF()) {
		if (gl_dataFinnhubForexSymbol.IsSymbol(setWorldChosenForex.m_Symbol)) {
			pForex = gl_dataFinnhubForexSymbol.GetSymbol(setWorldChosenForex.m_Symbol);
			m_mapSymbol[setWorldChosenForex.m_Symbol] = m_mapSymbol.size();
			m_vStock.push_back(pForex);
		}
		else {
			setWorldChosenForex.Delete();
		}
		setWorldChosenForex.MoveNext();
	}
	setWorldChosenForex.m_pDatabase->CommitTrans();
	setWorldChosenForex.Close();

	return true;
}
