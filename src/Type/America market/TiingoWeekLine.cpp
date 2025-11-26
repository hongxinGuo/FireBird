#include "pch.h"

#include "TiingoWeekLine.h"
#include"TiingoStock.h"

static CTiingoStock s_stock; // 用于获得各价格的放大比率。

CTiingoWeekLine::CTiingoWeekLine() {
}

int CTiingoWeekLine::GetRatio() const {
	return s_stock.GetRatio();
}
