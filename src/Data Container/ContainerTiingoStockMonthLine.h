#pragma once

#include "TiingoCandleLine.h"

#include "VirtualDataHistoryCandleBasic.h"

class CContainerTiingoStockMonthLine : public CVirtualDataHistoryCandleBasic {
public:
	CContainerTiingoStockMonthLine() = default;
	CContainerTiingoStockMonthLine(const CContainerTiingoStockMonthLine& other) = delete;
	CContainerTiingoStockMonthLine(CContainerTiingoStockMonthLine&& other) noexcept = delete;
	CContainerTiingoStockMonthLine& operator=(const CContainerTiingoStockMonthLine& other) = delete;
	CContainerTiingoStockMonthLine& operator=(CContainerTiingoStockMonthLine&& other) noexcept = delete;
	~CContainerTiingoStockMonthLine() override = default;

	void UpdateData(const CTiingoCandleLinesPtr& pvTempDayLine);

	CTiingoCandleLinePtr GetData(const size_t lIndex) const { return dynamic_pointer_cast<CTiingoCandleLine>(CVirtualDataHistoryCandleBasic::GetData(lIndex)); }
	CTiingoCandleLinePtr GetDayLine(long lDate) { return dynamic_pointer_cast<CTiingoCandleLine>(CVirtualDataHistoryCandleBasic::GetCandle(lDate)); }

	bool Add(const CTiingoCandleLinePtr& pData) {
		m_vHistoryData.push_back(pData);
		return true;
	}

	// 特有函数
};

using CContainerTiingoStockMonthLinePtr = shared_ptr<CContainerTiingoStockMonthLine>;
using CContainerTiingoStockMonthLinesPtr = shared_ptr<vector<CContainerTiingoStockMonthLinePtr>>;
