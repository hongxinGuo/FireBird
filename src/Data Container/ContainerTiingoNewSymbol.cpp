#include "pch.h"

module FireBird.Container.Tiingo.Symbol;
import FireBird.System.Data;

CContainerTiingoSymbol::CContainerTiingoSymbol() {
	CContainerTiingoSymbol::Reset();
}

CContainerTiingoSymbol::~CContainerTiingoSymbol() {
}

void CContainerTiingoSymbol::Reset() {
	CContainerVirtualStock::Reset();
}
