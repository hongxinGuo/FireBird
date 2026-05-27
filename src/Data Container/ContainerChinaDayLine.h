#pragma once

#include"WeekLine.h"
#include "VirtualDataHistoryCandle.h"

class CContainerChinaDayLine : public CVirtualDataHistoryCandle {
public:
	CContainerChinaDayLine();
	CContainerChinaDayLine(const CContainerChinaDayLine& other) = delete;
	CContainerChinaDayLine(CContainerChinaDayLine&& other) noexcept = delete;
	CContainerChinaDayLine& operator=(const CContainerChinaDayLine& other) = delete;
	CContainerChinaDayLine& operator=(CContainerChinaDayLine&& other) noexcept = delete;
	~CContainerChinaDayLine() override = default;

	bool SaveDB(const string& strStockSymbol) final;
	bool LoadDB(const string& strStockSymbol) final;

	bool UpdateDB(const string& strStockSymbol);

	CDayLine* GetData(const size_t lIndex) { return static_cast<CDayLine*>(CVirtualDataHistoryCandle::GetData(lIndex)); }
	CDayLine* GetDayLine(long lDate) { return static_cast<CDayLine*>(CVirtualDataHistoryCandle::GetCandle(lDate)); }

	CWeekLine CreateNewWeekLine(long& lCurrentDayLinePos);

	// 特有函数
	bool BuildWeekLine(vector<CWeekLine>& vWeekLine);
};

using CContainerChinaDayLinePtr = shared_ptr<CContainerChinaDayLine>;
