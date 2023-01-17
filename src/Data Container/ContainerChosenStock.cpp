#include "pch.h"

#include"WorldMarket.h"
#include"SetWorldChosenStock.h"

#include "ContainerChosenStock.h"

CContainerChosenStock::CContainerChosenStock() {
	m_lChosenStockPos = 0;
	CContainerChosenStock::Reset();
}

void CContainerChosenStock::Reset(void) {
	m_lChosenStockPos = 0;
}

bool CContainerChosenStock::LoadDB(void) {
	CSetWorldChosenStock setWorldChosenStock;
	CWorldStockPtr pStock = nullptr;

	ASSERT(gl_pWorldMarket != nullptr);
	setWorldChosenStock.Open();
	setWorldChosenStock.m_pDatabase->BeginTrans();
	while (!setWorldChosenStock.IsEOF()) {
		if (gl_pWorldMarket->IsStock(setWorldChosenStock.m_Symbol)) {
			pStock = gl_pWorldMarket->GetStock(setWorldChosenStock.m_Symbol);
			m_mapSymbol[setWorldChosenStock.m_Symbol] = m_mapSymbol.size();
			m_vStock.push_back(pStock);
		}
		setWorldChosenStock.MoveNext();
	}
	setWorldChosenStock.m_pDatabase->CommitTrans();
	setWorldChosenStock.Close();

	return true;
}
