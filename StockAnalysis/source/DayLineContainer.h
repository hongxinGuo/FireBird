#pragma once

#include"DayLine.h"
#include"WeekLine.h"
#include "VirtualHistoryCandleExtendContainer.h"

#include"SetDayLineBasicInfo.h"
#include"SetDayLineExtendInfo.h"

using namespace std;

class CDayLineContainer : public CVirtualHistoryCandleExtendContainer {
public:
	CDayLineContainer();
	virtual ~CDayLineContainer();
public:
	virtual bool SaveData(CString strStockSymbol) override;
	virtual bool LoadData(CString strStockSymbol) override;

	void UpdateData(vector<CDayLinePtr>& vTempDayLine);
	CWeekLinePtr CreateNewWeekLine(long& lCurrentDayLinePos);

public:
	// ���к���
	bool BuildWeekLine(vector<CWeekLinePtr>& vWeekLine);

private:
};
