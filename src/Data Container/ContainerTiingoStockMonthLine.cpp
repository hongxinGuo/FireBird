#include "pch.h"

#include "ContainerTiingoStockMonthLine.h"

namespace {
	CTiingoStock s_stock;
}

CContainerTiingoStockMonthLine::CContainerTiingoStockMonthLine() {
	m_ratio = s_stock.GetRatio();
}

void CContainerTiingoStockMonthLine::UpdateData(const CTiingoCandleLinesPtr& pvTempMonthLine) {
	Unload(); // 清除已载入的日线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (auto& dayLine : *pvTempMonthLine) {
		if (dayLine.IsActive()) {// 清除掉不再交易（停牌或退市后出现的）的股票日线
			Add(dayLine);
		}
	}
	m_fDataLoaded = true;
}
