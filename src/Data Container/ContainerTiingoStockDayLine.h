#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CContainerTiingoStockDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CContainerTiingoStockDayLine();
	CContainerTiingoStockDayLine(const CContainerTiingoStockDayLine& other) = delete;
	CContainerTiingoStockDayLine(CContainerTiingoStockDayLine&& other) noexcept = delete;
	CContainerTiingoStockDayLine& operator=(const CContainerTiingoStockDayLine& other) = delete;
	CContainerTiingoStockDayLine& operator=(CContainerTiingoStockDayLine&& other) noexcept = delete;
	~CContainerTiingoStockDayLine() override = default;

	bool SaveDB(const CString& strStockSymbol) override;
	bool LoadDB(const CString& strStockSymbol) override;

	// ÌØÓÐº¯Êý
};

using CContainerTiingoStockDayLinePtr = shared_ptr<CContainerTiingoStockDayLine>;
