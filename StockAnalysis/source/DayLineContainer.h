#pragma once

#include"DayLine.h"
#include"WeekLine.h"
#include "VirtualHistoryCandleExtendContainer.h"

class CDayLineContainer : public CVirtualHistoryCandleExtendContainer {
public:
	CDayLineContainer();
	virtual ~CDayLineContainer();
public:
	virtual bool SaveData(CString strStockSymbol) override;
	virtual bool LoadData(CString strStockSymbol) override;

	void UpdateData(vector<CDayLinePtr>& vTempDayLine, bool fRevertSave);
	CWeekLinePtr CreateNewWeekLine(long& lCurrentDayLinePos);

public:
	// ÌØÓÐº¯Êý
	bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine);

private:
};
