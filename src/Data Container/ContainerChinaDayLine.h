#pragma once

import FireBird.HistoryCandle.WeekLine;
#include "VirtualContainerHistoryCandleExtend.h"

class CContainerChinaDayLine : public CVirtualContainerHistoryCandleExtend {
public:
	CContainerChinaDayLine();
	CContainerChinaDayLine(const CContainerChinaDayLine& other) = delete;
	CContainerChinaDayLine(CContainerChinaDayLine&& other) noexcept = delete;
	CContainerChinaDayLine& operator=(const CContainerChinaDayLine& other) = delete;
	CContainerChinaDayLine& operator=(CContainerChinaDayLine&& other) noexcept = delete;
	~CContainerChinaDayLine() override = default;

	bool SaveDB(const CString& strStockSymbol) final;
	bool LoadDB(const CString& strStockSymbol) final;

	CWeekLinePtr CreateNewWeekLine(long& lCurrentDayLinePos) const;

	// ÌØÓÐº¯Êý
	bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine) const;
};

using CDataChinaDayLinePtr = shared_ptr<CContainerChinaDayLine>;
