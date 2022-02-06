#pragma once

#include"DayLine.h"
#include"WeekLine.h"
#include "VirtualDataHistoryCandleExtend.h"

class CDataChinaDayLine : public CVirtualDataHistoryCandleExtend {
public:
	CDataChinaDayLine();
	virtual ~CDataChinaDayLine();
public:
	virtual bool SaveDB(CString strStockSymbol) override final;
	virtual bool LoadDB(CString strStockSymbol) override final;

	void UpdateData(vector<CDayLinePtr>& vTempDayLine, bool fRevertSave);
	CWeekLinePtr CreateNewWeekLine(long& lCurrentDayLinePos);

public:
	// ÌØÓÐº¯Êý
	bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine);

private:
};
