#include "pch.h"

#include "TiingoCandleLine.h"
#include"TiingoStock.h"

namespace TiingoCandleLine {
	CTiingoStock s_stock; // 用于获得各价格的放大比率。
}

CTiingoCandleLine::CTiingoCandleLine() {
	m_ratio = TiingoCandleLine::s_stock.GetRatio();
}
