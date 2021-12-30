#include "pch.h"

#include"globedef.h"
#include"FinnhubForexSymbol.h"
#include"SetWorldChoicedForex.h"
#include "DataChoicedForex.h"

CDataChoicedForex::CDataChoicedForex() {
	Reset();
}

CDataChoicedForex::~CDataChoicedForex() {
}

void CDataChoicedForex::Reset(void) {
	m_lChoicedForexPos = 0;
	m_vWorldChoicedForex.resize(0);
	m_mapWorldChoicedForex.clear();
}

bool CDataChoicedForex::LoadDB(void) {
	CSetWorldChoicedForex setWorldChoicedForex;
	CForexSymbolPtr pForex = nullptr;

	setWorldChoicedForex.Open();
	setWorldChoicedForex.m_pDatabase->BeginTrans();
	while (!setWorldChoicedForex.IsEOF()) {
		if (gl_pWorldMarket->IsForexSymbol(setWorldChoicedForex.m_Symbol)) {
			pForex = gl_pWorldMarket->GetForexSymbol(setWorldChoicedForex.m_Symbol);
			m_mapWorldChoicedForex[setWorldChoicedForex.m_Symbol] = m_mapWorldChoicedForex.size();
			m_vWorldChoicedForex.push_back(pForex);
		}
		setWorldChoicedForex.MoveNext();
	}
	setWorldChoicedForex.m_pDatabase->CommitTrans();
	setWorldChoicedForex.Close();

	return true;
}