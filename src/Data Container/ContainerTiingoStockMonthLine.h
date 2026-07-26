#pragma once

#include "VirtualDataHistoryCandle.h"

class CTiingoCandleLine;

class CContainerTiingoStockMonthLine : public CVirtualDataHistoryCandle {
public:
	CContainerTiingoStockMonthLine();
	CContainerTiingoStockMonthLine(const CContainerTiingoStockMonthLine& other) = delete;
	CContainerTiingoStockMonthLine(CContainerTiingoStockMonthLine&& other) noexcept = delete;
	CContainerTiingoStockMonthLine& operator=(const CContainerTiingoStockMonthLine& other) = delete;
	CContainerTiingoStockMonthLine& operator=(CContainerTiingoStockMonthLine&& other) noexcept = delete;
	~CContainerTiingoStockMonthLine() override = default;

	void UpdateData(const shared_ptr<vector<CTiingoCandleLine>>& pvTempDayLine);

	CTiingoCandleLine* GetData(size_t lIndex);
	CTiingoCandleLine* GetDayLine(local_days lDate);

	void Add(CTiingoCandleLine& data);

	// 特有函数
};

using CContainerTiingoStockMonthLinePtr = shared_ptr<CContainerTiingoStockMonthLine>;
using CContainerTiingoStockMonthLinesPtr = shared_ptr<vector<CContainerTiingoStockMonthLinePtr>>;
