#include "pch.h"

#include"WorldMarket.h"
#include"SetWorldChosenStock.h"

#include "ContainerChosenStock.h"

#include "CharSetTransfer.h"

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
		if (gl_dataContainerFinnhubStock.IsSymbol(T2Utf8(setWorldChosenStock.m_Symbol))) {
			pStock = gl_dataContainerFinnhubStock.GetItem(T2Utf8(setWorldChosenStock.m_Symbol));
			m_mapSymbol[T2Utf8(setWorldChosenStock.m_Symbol)] = m_mapSymbol.size();
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
