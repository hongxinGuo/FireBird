#pragma once

#include "VirtualDataHistoryCandle.h"

class CContainerFinnhubStockDayLine final : public CVirtualDataHistoryCandle {
public:
	CContainerFinnhubStockDayLine();
	CContainerFinnhubStockDayLine(const CContainerFinnhubStockDayLine& other) = delete;
	CContainerFinnhubStockDayLine(CContainerFinnhubStockDayLine&& other) noexcept = delete;
	CContainerFinnhubStockDayLine& operator=(const CContainerFinnhubStockDayLine& other) = delete;
	CContainerFinnhubStockDayLine& operator=(CContainerFinnhubStockDayLine&& other) noexcept = delete;
	~CContainerFinnhubStockDayLine() override = default;

	void SaveDB(const string& strStockSymbol) override;
	void LoadDB(const string& strStockSymbol) override;

	CDayLine* GetData(const size_t lIndex) { return static_cast<CDayLine*>(CVirtualDataHistoryCandle::GetData(lIndex)); }
	CDayLine* GetDayLine(chrono::local_days lDate) { return static_cast<CDayLine*>(CVirtualDataHistoryCandle::GetCandle(lDate)); }

	// 特有函数
};

using CContainerFinnhubStockDayLinePtr = shared_ptr<CContainerFinnhubStockDayLine>;
