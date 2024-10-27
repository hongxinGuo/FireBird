#pragma once

#include "VirtualDataHistoryCandleExtend.h"

class CContainerFinnhubStockDayLine final : public CVirtualDataHistoryCandleExtend {
public:
	CContainerFinnhubStockDayLine();
	CContainerFinnhubStockDayLine(const CContainerFinnhubStockDayLine& other) = delete;
	CContainerFinnhubStockDayLine(CContainerFinnhubStockDayLine&& other) noexcept = delete;
	CContainerFinnhubStockDayLine& operator=(const CContainerFinnhubStockDayLine& other) = delete;
	CContainerFinnhubStockDayLine& operator=(CContainerFinnhubStockDayLine&& other) noexcept = delete;
	~CContainerFinnhubStockDayLine() override = default;

	bool SaveDB(const CString& strStockSymbol) override;
	bool LoadDB(const CString& strStockSymbol) override;

	// ÌØÓÐº¯Êý
};

using CContainerWorldStockDayLinePtr = shared_ptr<CContainerFinnhubStockDayLine>;
