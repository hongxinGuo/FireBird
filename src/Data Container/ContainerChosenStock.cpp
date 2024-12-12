#include "pch.h"
module;
module FireBird.Container.Chosen.Stock;

import FireBird.Set.WorldChosenStock;
import FireBird.Market.World;
import FireBird.Stock.Finnhub;
import FireBird.System.Data;

CContainerChosenStock::CContainerChosenStock() {
	CContainerChosenStock::Reset();
}

void CContainerChosenStock::Reset() {
	CContainerVirtualStock::Reset();

	m_lChosenStockPos = 0;
}

bool CContainerChosenStock::LoadDB() {
	CSetWorldChosenStock setWorldChosenStock;
	CFinnhubStockPtr pStock = nullptr;

	ASSERT(gl_pWorldMarket != nullptr);
	setWorldChosenStock.Open();
	setWorldChosenStock.m_pDatabase->BeginTrans();
	while (!setWorldChosenStock.IsEOF()) {
		if (gl_dataContainerFinnhubStock.IsSymbol(setWorldChosenStock.m_Symbol)) {
			pStock = gl_dataContainerFinnhubStock.GetStock(setWorldChosenStock.m_Symbol);
			m_mapSymbol[setWorldChosenStock.m_Symbol] = m_mapSymbol.size();
			m_vStock.push_back(pStock);
		}
		else {
			setWorldChosenStock.Delete();
		}
		setWorldChosenStock.MoveNext();
	}
	setWorldChosenStock.m_pDatabase->CommitTrans();
	setWorldChosenStock.Close();

	return true;
}
