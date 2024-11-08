#include "pch.h"
#include "ContainerTiingoSymbol.h"

CContainerTiingoSymbol::CContainerTiingoSymbol() {
	CContainerTiingoSymbol::Reset();
}

CContainerTiingoSymbol::~CContainerTiingoSymbol() {
}

void CContainerTiingoSymbol::Reset() {
	CContainerVirtualStock::Reset();
}
