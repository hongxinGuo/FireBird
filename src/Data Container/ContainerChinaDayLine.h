#pragma once

#include"WeekLine.h"
#include "VirtualDataHistoryCandleBasic.h"

class CContainerChinaDayLine : public CVirtualDataHistoryCandleBasic {
public:
	CContainerChinaDayLine() = default;
	CContainerChinaDayLine(const CContainerChinaDayLine& other) = delete;
	CContainerChinaDayLine(CContainerChinaDayLine&& other) noexcept = delete;
	CContainerChinaDayLine& operator=(const CContainerChinaDayLine& other) = delete;
	CContainerChinaDayLine& operator=(CContainerChinaDayLine&& other) noexcept = delete;
	~CContainerChinaDayLine() override = default;

	bool SaveDB(const string& strStockSymbol) final;
	bool LoadDB(const string& strStockSymbol) final;

	CWeekLinePtr CreateNewWeekLine(long& lCurrentDayLinePos) const;

	// 特有函数
	bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine) const;
};

using CContainerChinaDayLinePtr = shared_ptr<CContainerChinaDayLine>;
