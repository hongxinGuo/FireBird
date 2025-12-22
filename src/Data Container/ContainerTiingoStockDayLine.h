#pragma once

#include "SetTiingoStockDayLine.h"
#include "TiingoCandleLine.h"

#include "VirtualDataHistoryCandleExtend.h"

class CContainerTiingoStockDayLine : public CVirtualDataHistoryCandleExtend {
public:
	CContainerTiingoStockDayLine() = default;
	CContainerTiingoStockDayLine(const CContainerTiingoStockDayLine& other) = delete;
	CContainerTiingoStockDayLine(CContainerTiingoStockDayLine&& other) noexcept = delete;
	CContainerTiingoStockDayLine& operator=(const CContainerTiingoStockDayLine& other) = delete;
	CContainerTiingoStockDayLine& operator=(CContainerTiingoStockDayLine&& other) noexcept = delete;
	~CContainerTiingoStockDayLine() override = default;

	bool SaveDB(const string& strStockSymbol) override;
	bool LoadDB(const string& strStockSymbol) override;

	void UpdateDB(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const string& strStockSymbol) const;
	bool UpdateDB2(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const string& strStockSymbol) const;
	bool LoadBasicDB(CSetTiingoStockDayLine* pSetHistoryCandleBasic);

	void UpdateData(const CTiingoCandleLinesPtr& pvTempDayLine);
	void SplitAdjust();

	CTiingoCandleLinePtr GetData(const size_t lIndex) const { return dynamic_pointer_cast<CTiingoCandleLine>(CVirtualDataHistoryCandleExtend::GetData(lIndex)); }
	CTiingoCandleLinePtr GetDayLine(long lDate) { return dynamic_pointer_cast<CTiingoCandleLine>(CVirtualDataHistoryCandleExtend::GetCandle(lDate)); }

	bool Add(const CTiingoCandleLinePtr& pData) {
		m_vHistoryData.push_back(pData);
		return true;
	}

	// 特有函数
};

using CContainerTiingoStockDayLinePtr = shared_ptr<CContainerTiingoStockDayLine>;
