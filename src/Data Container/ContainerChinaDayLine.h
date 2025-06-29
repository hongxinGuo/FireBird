#pragma once

#include"WeekLine.h"
#include "VirtualDataHistoryCandleExtend.h"

class CContainerChinaDayLine : public CVirtualDataHistoryCandleExtend {
public:
	CContainerChinaDayLine();
	CContainerChinaDayLine(const CContainerChinaDayLine& other) = delete;
	CContainerChinaDayLine(CContainerChinaDayLine&& other) noexcept = delete;
	CContainerChinaDayLine& operator=(const CContainerChinaDayLine& other) = delete;
	CContainerChinaDayLine& operator=(CContainerChinaDayLine&& other) noexcept = delete;
	~CContainerChinaDayLine() override = default;

	bool SaveDB(const string& strStockSymbol) final;
	bool LoadDB(const string& strStockSymbol) final;

	CWeekLinePtr CreateNewWeekLine(long& lCurrentDayLinePos) const;

	// ÌØÓÐº¯Êý
	bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine) const;
};

using CDataChinaDayLinePtr = shared_ptr<CContainerChinaDayLine>;
