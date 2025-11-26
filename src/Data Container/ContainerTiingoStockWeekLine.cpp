#include "pch.h"

#include "ContainerTiingoStockWeekLine.h"

#include "InfoReport.h"

CContainerTiingoStockWeekLine::CContainerTiingoStockWeekLine() {
}

void CContainerTiingoStockWeekLine::UpdateData(CTiingoWeekLinesPtr pvTempWeekLine) {
	Unload(); // 清除已载入的日线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (const auto& pDayLine : *pvTempWeekLine) {
		if (pDayLine->IsActive()) {
			// 清除掉不再交易（停牌或退市后出现的）的股票日线
			Add(pDayLine);
		}
	}
	m_fDataLoaded = true;
}
