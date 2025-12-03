#include "pch.h"

#include "ContainerTiingoStockMonthLine.h"

CContainerTiingoStockMonthLine::CContainerTiingoStockMonthLine() {
}

void CContainerTiingoStockMonthLine::UpdateData(CTiingoCandleLinesPtr pvTempMonthLine) {
	Unload(); // 清除已载入的日线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (const auto& pDayLine : *pvTempMonthLine) {
		if (pDayLine->IsActive()) {
			// 清除掉不再交易（停牌或退市后出现的）的股票日线
			Add(pDayLine);
		}
	}
	m_fDataLoaded = true;
}
