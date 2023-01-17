#include "pch.h"

#include"WorldMarket.h"
#include"FinnhubForexSymbol.h"
#include"SetWorldChosenForex.h"
#include "ContainerChosenForex.h"

CDataChosenForex::CDataChosenForex() {
	m_lChosenForexPos = 0;
	CDataChosenForex::Reset();
}

void CDataChosenForex::Reset(void) {
	m_lChosenForexPos = 0;
}

bool CDataChosenForex::LoadDB(void) {
	CSetWorldChosenForex setWorldChosenForex;
	CForexSymbolPtr pForex = nullptr;

	setWorldChosenForex.Open();
	setWorldChosenForex.m_pDatabase->BeginTrans();
	while (!setWorldChosenForex.IsEOF()) {
		if (gl_pWorldMarket->IsForexSymbol(setWorldChosenForex.m_Symbol)) {
			pForex = gl_pWorldMarket->GetForexSymbol(setWorldChosenForex.m_Symbol);
			m_mapSymbol[setWorldChosenForex.m_Symbol] = m_mapSymbol.size();
			m_vStock.push_back(pForex);
		}
		setWorldChosenForex.MoveNext();
	}
	setWorldChosenForex.m_pDatabase->CommitTrans();
	setWorldChosenForex.Close();

	return true;
}
