#include "pch.h"

#include "ContainerTiingoStockWeekLine.h"

namespace {
	CTiingoStock s_stockContainerTiingoStockWeekLine;
}

CContainerTiingoStockWeekLine::CContainerTiingoStockWeekLine() {
	m_ratio = s_stockContainerTiingoStockWeekLine.GetRatio();
}

void CContainerTiingoStockWeekLine::UpdateData(const CTiingoCandleLinesPtr& pvTempWeekLine) {
	Unload(); // 清除已载入的日线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (auto& weekLine : *pvTempWeekLine) {
		if (weekLine.IsActive()) { // 清除掉不再交易（停牌或退市后出现的）的股票日线
			Add(weekLine);
		}
	}
	m_fDataLoaded = true;
}
