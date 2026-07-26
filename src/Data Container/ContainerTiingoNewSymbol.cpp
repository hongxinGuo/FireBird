#include "pch.h"

#include "ContainerTiingoSymbol.h"
#include"TiingoStock.h"

CContainerTiingoSymbol::CContainerTiingoSymbol() {
	CContainerTiingoSymbol::Reset();
}

void CContainerTiingoSymbol::Reset() {
	CContainerVirtualStock::Reset();
}

CTiingoStockPtr CContainerTiingoSymbol::GetStock(size_t lIndex) {
	return dynamic_pointer_cast<CTiingoStock>(Get(lIndex));
}

CTiingoStockPtr CContainerTiingoSymbol::GetStock(const string& strStockCode) {
	return dynamic_pointer_cast<CTiingoStock>(Get(strStockCode));
}
