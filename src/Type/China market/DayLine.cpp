#include"pch.h"

#include "DayLine.h"
#include "ChinaStock.h"

namespace ChinaStockDayLine {
	static CChinaStock s_stock;
}

CDayLine::CDayLine() {
	m_ratio = ChinaStockDayLine::s_stock.GetRatio();
}
