#pragma once

#include "TiingoCandleLine.h"

#include "VirtualDataHistoryCandleExtend.h"

class CContainerTiingoStockMonthLine : public CVirtualDataHistoryCandleExtend {
public:
	CContainerTiingoStockMonthLine();
	CContainerTiingoStockMonthLine(const CContainerTiingoStockMonthLine& other) = delete;
	CContainerTiingoStockMonthLine(CContainerTiingoStockMonthLine&& other) noexcept = delete;
	CContainerTiingoStockMonthLine& operator=(const CContainerTiingoStockMonthLine& other) = delete;
	CContainerTiingoStockMonthLine& operator=(CContainerTiingoStockMonthLine&& other) noexcept = delete;
	~CContainerTiingoStockMonthLine() override = default;

	void UpdateData(CTiingoCandleLinesPtr pvTempDayLine);

	CTiingoCandleLinePtr GetData(const size_t lIndex) const { return dynamic_pointer_cast<CTiingoCandleLine>(CVirtualDataHistoryCandleExtend::GetData(lIndex)); }
	CTiingoCandleLinePtr GetDayLine(long lDate) { return dynamic_pointer_cast<CTiingoCandleLine>(CVirtualDataHistoryCandleExtend::GetDayLine(lDate)); }

	bool Add(const CTiingoCandleLinePtr& pData) {
		m_vHistoryData.push_back(pData);
		return true;
	}

	// 特有函数
};

using CContainerTiingoStockWeekLinePtr = shared_ptr<CContainerTiingoStockWeekLine>;
