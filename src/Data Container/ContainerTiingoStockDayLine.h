#pragma once

#include "SetTiingoStockDayLine.h"
#include "TiingoDayLine.h"

#include "VirtualDataHistoryCandleExtend.h"

class CContainerTiingoStockDayLine : public CVirtualDataHistoryCandleExtend {
public:
	CContainerTiingoStockDayLine();
	CContainerTiingoStockDayLine(const CContainerTiingoStockDayLine& other) = delete;
	CContainerTiingoStockDayLine(CContainerTiingoStockDayLine&& other) noexcept = delete;
	CContainerTiingoStockDayLine& operator=(const CContainerTiingoStockDayLine& other) = delete;
	CContainerTiingoStockDayLine& operator=(CContainerTiingoStockDayLine&& other) noexcept = delete;
	~CContainerTiingoStockDayLine() override = default;

	bool SaveDB(const string& strStockSymbol) override;
	bool LoadDB(const string& strStockSymbol) override;

	void UpdateDB(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const string& strStockSymbol);
	bool UpdateDB2(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const string& strStockSymbol) const;
	bool LoadBasicDB(CSetTiingoStockDayLine* pSetHistoryCandleBasic);

	void UpdateData(CTiingoDayLinesPtr pvTempDayLine);

	CTiingoDayLinePtr GetData(const size_t lIndex) const { return dynamic_pointer_cast<CTiingoDayLine>(CVirtualDataHistoryCandleExtend::GetData(lIndex)); }
	CTiingoDayLinePtr GetDayLine(long lDate) { return dynamic_pointer_cast<CTiingoDayLine>(CVirtualDataHistoryCandleExtend::GetDayLine(lDate)); }

	bool Add(const CTiingoDayLinePtr& pData) {
		m_vHistoryData.push_back(pData);
		return true;
	}

	// 特有函数
	void CalculateMA(int length) const;
};

using CContainerTiingoStockDayLinePtr = shared_ptr<CContainerTiingoStockDayLine>;
