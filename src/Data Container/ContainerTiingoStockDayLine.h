#pragma once

#include "SetTiingoStockDayLine.h"
#include "TiingoCandleLine.h"

#include "VirtualDataHistoryCandle.h"

class CContainerTiingoStockDayLine : public CVirtualDataHistoryCandle {
public:
	CContainerTiingoStockDayLine();
	CContainerTiingoStockDayLine(const CContainerTiingoStockDayLine& other) = delete;
	CContainerTiingoStockDayLine(CContainerTiingoStockDayLine&& other) noexcept = delete;
	CContainerTiingoStockDayLine& operator=(const CContainerTiingoStockDayLine& other) = delete;
	CContainerTiingoStockDayLine& operator=(CContainerTiingoStockDayLine&& other) noexcept = delete;
	~CContainerTiingoStockDayLine() override = default;

	bool SaveDB(const string& strStockSymbol) override;
	bool LoadDB(const string& strStockSymbol) override;
	bool LoadDB2(const string& strStockSymbol);

	void UpdateDB(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const string& strStockSymbol);
	void UpdateDB2(const string& strStockSymbol);

	void UpdateData(const CTiingoCandleLinesPtr& pvTempDayLine);
	void SplitAdjust();

	CTiingoCandleLine* GetData(const size_t lIndex) { return static_cast<CTiingoCandleLine*>(CVirtualDataHistoryCandle::GetData(lIndex)); }
	CTiingoCandleLine* GetDayLine(long lDate) { return static_cast<CTiingoCandleLine*>(CVirtualDataHistoryCandle::GetCandle(lDate)); }

	void Add(CTiingoCandleLine& data) {
		data.SetRatio(m_ratio);
		m_vHistoryData.push_back(data);
	}

	// 特有函数
};

using CContainerTiingoStockDayLinePtr = shared_ptr<CContainerTiingoStockDayLine>;
