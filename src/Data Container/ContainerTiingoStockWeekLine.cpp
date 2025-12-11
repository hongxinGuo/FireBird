#include "pch.h"

#include "ContainerTiingoStockWeekLine.h"

void CContainerTiingoStockWeekLine::UpdateData(const CTiingoCandleLinesPtr& pvTempWeekLine) {
	Unload(); // 清除已载入的日线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (const auto& pWeekLine : *pvTempWeekLine) {
		if (pWeekLine->IsActive()) { // 清除掉不再交易（停牌或退市后出现的）的股票日线
			Add(pWeekLine);
		}
	}
	m_fDataLoaded = true;
}
