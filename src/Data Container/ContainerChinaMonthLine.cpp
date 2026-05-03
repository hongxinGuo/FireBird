#include"pch.h"

#include"ContainerChinaStockMonthLine.h"
#include "ChinaStock.h"

namespace {
	CChinaStock s_stock;
}

CContainerChinaStockMonthLine::CContainerChinaStockMonthLine() {
	m_ratio = s_stock.GetRatio();
}
