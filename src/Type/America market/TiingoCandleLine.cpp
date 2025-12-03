#include "pch.h"

#include "TiingoCandleLine.h"
#include"TiingoStock.h"

static CTiingoStock s_stock; // 用于获得各价格的放大比率。

CTiingoCandleLine::CTiingoCandleLine() {
}

int CTiingoCandleLine::GetRatio() const {
	return s_stock.GetRatio();
}
