#include "pch.h"

#include "CContainerVirtualStock.h"

CContainerVirtualStock::CContainerVirtualStock() {
	m_lLoadedStock = 0;
}

bool CContainerVirtualStock::Add(CVirtualStockPtr pStock) {
	return false;
}

bool CContainerVirtualStock::Delete(CVirtualStockPtr pStock) {
	return false;
}

void CContainerVirtualStock::Sort() {
}
