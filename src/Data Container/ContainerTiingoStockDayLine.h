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

	bool Add(const CTiingoDayLinePtr& pData) {
		m_vHistoryData.push_back(pData);
		return true;
	}

	// ÌØÓÐº¯Êý
	void CalculateMA(int length) const;
};

using CContainerTiingoStockDayLinePtr = shared_ptr<CContainerTiingoStockDayLine>;
