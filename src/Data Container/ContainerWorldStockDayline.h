#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CContainerWorldStockDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CContainerWorldStockDayLine();
	CContainerWorldStockDayLine(const CContainerWorldStockDayLine& other) = delete;
	CContainerWorldStockDayLine(CContainerWorldStockDayLine&& other) noexcept = delete;
	CContainerWorldStockDayLine& operator=(const CContainerWorldStockDayLine& other) = delete;
	CContainerWorldStockDayLine& operator=(CContainerWorldStockDayLine&& other) noexcept = delete;
	~CContainerWorldStockDayLine() override = default;

	bool SaveDB(const CString& strStockSymbol) override;
	bool LoadDB(const CString& strStockSymbol) override;

	// ÌØÓÐº¯Êý
};

using CContainerWorldStockDayLinePtr = shared_ptr<CContainerWorldStockDayLine>;
