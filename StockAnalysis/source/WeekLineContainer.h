#pragma once

#include "VirtualHistoryCandleExtendContainer.h"
#include"WeekLine.h"
#include"DayLine.h"

class CWeekLineContainer : public CVirtualHistoryCandleExtendContainer {
public:
	CWeekLineContainer();
	virtual ~CWeekLineContainer();

	virtual bool LoadData(CString strStockSymbol = _T("")) override;
	virtual bool SaveData(CString strStockSymbol = _T("")) override;

	bool SaveCurrentWeekLine(void);
	bool LoadCurrentWeekLine(void);

	void UpdateData(vector<CWeekLinePtr>& vTempWeekLine);
	bool UpdateData(CVirtualHistoryCandleExtendPtr pHistoryCandleExtend);

	bool StoreVectorData(vector<CWeekLinePtr>& vWeekLine);
};
