#pragma once

#include "VirtualDataHistoryCandle.h"

class CContainerFinnhubStockDayLine final : public CVirtualDataHistoryCandle {
public:
	CContainerFinnhubStockDayLine() = default;
	CContainerFinnhubStockDayLine(const CContainerFinnhubStockDayLine& other) = delete;
	CContainerFinnhubStockDayLine(CContainerFinnhubStockDayLine&& other) noexcept = delete;
	CContainerFinnhubStockDayLine& operator=(const CContainerFinnhubStockDayLine& other) = delete;
	CContainerFinnhubStockDayLine& operator=(CContainerFinnhubStockDayLine&& other) noexcept = delete;
	~CContainerFinnhubStockDayLine() override = default;

	bool SaveDB(const string& strStockSymbol) override;
	bool LoadDB(const string& strStockSymbol) override;

	// 特有函数
};

using CContainerFinnhubStockDayLinePtr = shared_ptr<CContainerFinnhubStockDayLine>;
