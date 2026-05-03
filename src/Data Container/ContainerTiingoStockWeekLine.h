#pragma once

#include "TiingoCandleLine.h"

#include "VirtualDataHistoryCandle.h"

class CContainerTiingoStockWeekLine : public CVirtualDataHistoryCandle {
public:
	CContainerTiingoStockWeekLine();
	CContainerTiingoStockWeekLine(const CContainerTiingoStockWeekLine& other) = delete;
	CContainerTiingoStockWeekLine(CContainerTiingoStockWeekLine&& other) noexcept = delete;
	CContainerTiingoStockWeekLine& operator=(const CContainerTiingoStockWeekLine& other) = delete;
	CContainerTiingoStockWeekLine& operator=(CContainerTiingoStockWeekLine&& other) noexcept = delete;
	~CContainerTiingoStockWeekLine() override = default;

	void UpdateData(const CTiingoCandleLinesPtr& pvTempDayLine);

	CTiingoCandleLine* GetData(const size_t lIndex) { return static_cast<CTiingoCandleLine*>(CVirtualDataHistoryCandle::GetData(lIndex)); }
	CTiingoCandleLine* GetWeekLine(long lDate) { return static_cast<CTiingoCandleLine*>(CVirtualDataHistoryCandle::GetCandle(lDate)); }

	void Add(CTiingoCandleLine& pData) {
		pData.SetRatio(m_ratio);
		m_vHistoryData.push_back(pData);
	}

	// 特有函数
};

using CContainerTiingoStockWeekLinePtr = shared_ptr<CContainerTiingoStockWeekLine>;
