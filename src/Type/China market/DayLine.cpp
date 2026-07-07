#include"pch.h"

#include "DayLine.h"
#include "ChinaStock.h"

namespace {
	CChinaStock s_stock;
}

CDayLine::CDayLine() {
	m_ratio = s_stock.GetRatio();
}
