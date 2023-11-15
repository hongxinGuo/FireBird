#pragma once

#include"WeekLine.h"
#include "VirtualDataHistoryCandleExtend.h"

class CContainerChinaDayLine : public CVirtualDataHistoryCandleExtend {
public:
	CContainerChinaDayLine();
	~CContainerChinaDayLine() override = default;

	bool SaveDB(const CString& strStockSymbol) final;
	bool LoadDB(const CString& strStockSymbol) final;

	CWeekLinePtr CreateNewWeekLine(long& lCurrentDayLinePos) const;

	// ÌØÓÐº¯Êý
	bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine) const;
};

using CDataChinaDayLinePtr = shared_ptr<CContainerChinaDayLine>;
