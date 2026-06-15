#pragma once

#include "TiingoCandleLine.h"

#include "VirtualDataHistoryCandle.h"

class CContainerTiingoStockMonthLine : public CVirtualDataHistoryCandle {
public:
	CContainerTiingoStockMonthLine();
	CContainerTiingoStockMonthLine(const CContainerTiingoStockMonthLine& other) = delete;
	CContainerTiingoStockMonthLine(CContainerTiingoStockMonthLine&& other) noexcept = delete;
	CContainerTiingoStockMonthLine& operator=(const CContainerTiingoStockMonthLine& other) = delete;
	CContainerTiingoStockMonthLine& operator=(CContainerTiingoStockMonthLine&& other) noexcept = delete;
	~CContainerTiingoStockMonthLine() override = default;

	void UpdateData(const CTiingoCandleLinesPtr& pvTempDayLine);

	CTiingoCandleLine* GetData(const size_t lIndex) { return static_cast<CTiingoCandleLine*>(CVirtualDataHistoryCandle::GetData(lIndex)); }
	CTiingoCandleLine* GetDayLine(chrono::local_days lDate) { return static_cast<CTiingoCandleLine*>(CVirtualDataHistoryCandle::GetCandle(lDate)); }

	void Add(CTiingoCandleLine& data) {
		data.SetRatio(m_ratio);
		m_vHistoryData.push_back(data);
	}

	// 特有函数
};

using CContainerTiingoStockMonthLinePtr = shared_ptr<CContainerTiingoStockMonthLine>;
using CContainerTiingoStockMonthLinesPtr = shared_ptr<vector<CContainerTiingoStockMonthLinePtr>>;
