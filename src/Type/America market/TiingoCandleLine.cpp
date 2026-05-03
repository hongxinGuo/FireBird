#include "pch.h"

#include "TiingoCandleLine.h"
#include"TiingoStock.h"

namespace {
	CTiingoStock s_stock; // 用于获得各价格的放大比率。
}

CTiingoCandleLine::CTiingoCandleLine() {
	m_ratio = s_stock.GetRatio();
}
