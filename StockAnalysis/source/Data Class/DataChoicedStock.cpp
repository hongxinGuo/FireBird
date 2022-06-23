#include "pch.h"

#include"WorldMarket.h"
#include"SetWorldChoicedStock.h"

#include "DataChoicedStock.h"

CDataChoicedStock::CDataChoicedStock() {
	Reset();
}

CDataChoicedStock::~CDataChoicedStock() {
}

void CDataChoicedStock::Reset(void) {
	m_vWorldChoicedStock.resize(0);
	m_mapWorldChoicedStock.clear();
	m_lChoicedStockPos = 0;
}

bool CDataChoicedStock::LoadDB(void) {
	CSetWorldChoicedStock setWorldChoicedStock;
	CWorldStockPtr pStock = nullptr;

	ASSERT(gl_pWorldMarket != nullptr);
	setWorldChoicedStock.Open();
	setWorldChoicedStock.m_pDatabase->BeginTrans();
	while (!setWorldChoicedStock.IsEOF()) {
		if (gl_pWorldMarket->IsStock(setWorldChoicedStock.m_Symbol)) {
			pStock = gl_pWorldMarket->GetStock(setWorldChoicedStock.m_Symbol);
			m_mapWorldChoicedStock[setWorldChoicedStock.m_Symbol] = m_mapWorldChoicedStock.size();
			m_vWorldChoicedStock.push_back(pStock);
		}
		setWorldChoicedStock.MoveNext();
	}
	setWorldChoicedStock.m_pDatabase->CommitTrans();
	setWorldChoicedStock.Close();

	return true;
}