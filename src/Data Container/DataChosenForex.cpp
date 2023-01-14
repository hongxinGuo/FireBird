#include "pch.h"

#include"WorldMarket.h"
#include"FinnhubForexSymbol.h"
#include"SetWorldChosenForex.h"
#include "DataChosenForex.h"

CDataChosenForex::CDataChosenForex() {
	m_lChosenForexPos = 0;
	Reset();
}

void CDataChosenForex::Reset(void) {
	m_lChosenForexPos = 0;
	m_vWorldChosenForex.resize(0);
	m_mapWorldChosenForex.clear();
}

bool CDataChosenForex::LoadDB(void) {
	CSetWorldChosenForex setWorldChosenForex;
	CForexSymbolPtr pForex = nullptr;

	setWorldChosenForex.Open();
	setWorldChosenForex.m_pDatabase->BeginTrans();
	while (!setWorldChosenForex.IsEOF()) {
		if (gl_pWorldMarket->IsForexSymbol(setWorldChosenForex.m_Symbol)) {
			pForex = gl_pWorldMarket->GetForexSymbol(setWorldChosenForex.m_Symbol);
			m_mapWorldChosenForex[setWorldChosenForex.m_Symbol] = m_mapWorldChosenForex.size();
			m_vWorldChosenForex.push_back(pForex);
		}
		setWorldChosenForex.MoveNext();
	}
	setWorldChosenForex.m_pDatabase->CommitTrans();
	setWorldChosenForex.Close();

	return true;
}
