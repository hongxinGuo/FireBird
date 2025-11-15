#pragma once

#include "TiingoWeekLine.h"

#include "VirtualDataHistoryCandleExtend.h"

class CContainerTiingoStockWeekLine : public CVirtualDataHistoryCandleExtend {
public:
	CContainerTiingoStockWeekLine();
	CContainerTiingoStockWeekLine(const CContainerTiingoStockWeekLine& other) = delete;
	CContainerTiingoStockWeekLine(CContainerTiingoStockWeekLine&& other) noexcept = delete;
	CContainerTiingoStockWeekLine& operator=(const CContainerTiingoStockWeekLine& other) = delete;
	CContainerTiingoStockWeekLine& operator=(CContainerTiingoStockWeekLine&& other) noexcept = delete;
	~CContainerTiingoStockWeekLine() override = default;

	void UpdateData(CTiingoWeekLinesPtr pvTempDayLine);

	CTiingoWeekLinePtr GetData(const size_t lIndex) const { return dynamic_pointer_cast<CTiingoWeekLine>(CVirtualDataHistoryCandleExtend::GetData(lIndex)); }
	CTiingoWeekLinePtr GetDayLine(long lDate) { return dynamic_pointer_cast<CTiingoWeekLine>(CVirtualDataHistoryCandleExtend::GetDayLine(lDate)); }

	bool Add(const CTiingoWeekLinePtr& pData) {
		m_vHistoryData.push_back(pData);
		return true;
	}

	// ÌØÓÐº¯Êý
};

using CContainerTiingoStockWeekLinePtr = shared_ptr<CContainerTiingoStockWeekLine>;
