#include "pch.h"

#include "TiingoDayLine.h"
#include"TiingoStock.h"

static CTiingoStock s_stock; // 用于获得各价格的放大比率。

CTiingoDayLine::CTiingoDayLine() {
}

int CTiingoDayLine::GetRatio() const {
	return s_stock.GetRatio();
}
