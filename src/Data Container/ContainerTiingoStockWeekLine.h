#pragma once

#include "TiingoCandleLine.h"

#include "VirtualDataHistoryCandleBasic.h"

class CContainerTiingoStockWeekLine : public CVirtualDataHistoryCandleBasic {
public:
	CContainerTiingoStockWeekLine() = default;
	CContainerTiingoStockWeekLine(const CContainerTiingoStockWeekLine& other) = delete;
	CContainerTiingoStockWeekLine(CContainerTiingoStockWeekLine&& other) noexcept = delete;
	CContainerTiingoStockWeekLine& operator=(const CContainerTiingoStockWeekLine& other) = delete;
	CContainerTiingoStockWeekLine& operator=(CContainerTiingoStockWeekLine&& other) noexcept = delete;
	~CContainerTiingoStockWeekLine() override = default;

	void UpdateData(const CTiingoCandleLinesPtr& pvTempDayLine);

	CTiingoCandleLinePtr GetData(const size_t lIndex) const { return dynamic_pointer_cast<CTiingoCandleLine>(CVirtualDataHistoryCandleBasic::GetData(lIndex)); }
	CTiingoCandleLinePtr GetWeekLine(long lDate) { return dynamic_pointer_cast<CTiingoCandleLine>(CVirtualDataHistoryCandleBasic::GetCandle(lDate)); }

	bool Add(const CTiingoCandleLinePtr& pData) {
		m_vHistoryData.push_back(pData);
		return true;
	}

	// 特有函数
};

using CContainerTiingoStockWeekLinePtr = shared_ptr<CContainerTiingoStockWeekLine>;
