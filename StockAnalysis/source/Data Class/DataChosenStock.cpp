#include "pch.h"

#include"WorldMarket.h"
#include"SetWorldChosenStock.h"

#include "DataChosenStock.h"

CDataChosenStock::CDataChosenStock() {
	Reset();
}

CDataChosenStock::~CDataChosenStock() {
}

void CDataChosenStock::Reset(void) {
	m_vWorldChosenStock.resize(0);
	m_mapWorldChosenStock.clear();
	m_lChosenStockPos = 0;
}

bool CDataChosenStock::LoadDB(void) {
	CSetWorldChosenStock setWorldChosenStock;
	CWorldStockPtr pStock = nullptr;

	ASSERT(gl_pWorldMarket != nullptr);
	setWorldChosenStock.Open();
	setWorldChosenStock.m_pDatabase->BeginTrans();
	while (!setWorldChosenStock.IsEOF()) {
		if (gl_pWorldMarket->IsStock(setWorldChosenStock.m_Symbol)) {
			pStock = gl_pWorldMarket->GetStock(setWorldChosenStock.m_Symbol);
			m_mapWorldChosenStock[setWorldChosenStock.m_Symbol] = m_mapWorldChosenStock.size();
			m_vWorldChosenStock.push_back(pStock);
		}
		setWorldChosenStock.MoveNext();
	}
	setWorldChosenStock.m_pDatabase->CommitTrans();
	setWorldChosenStock.Close();

	return true;
}