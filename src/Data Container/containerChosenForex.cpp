#include "pch.h"

#include"FinnhubForex.h"
#include"SetWorldChosenForex.h"
#include "ContainerChosenForex.h"

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
		if (gl_dataFinnhubForexSymbol.IsSymbol(setWorldChosenForex.m_Symbol.GetString())) {
			pForex = gl_dataFinnhubForexSymbol.GetSymbol(setWorldChosenForex.m_Symbol.GetString());
			m_mapSymbol[setWorldChosenForex.m_Symbol.GetString()] = m_mapSymbol.size();
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
