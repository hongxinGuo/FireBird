#pragma once

#include "VirtualDataHistoryCandleExtend.h"
#include"WeekLine.h"

class CDataChinaWeekLine : public CVirtualDataHistoryCandleExtend {
public:
	CDataChinaWeekLine();
	~CDataChinaWeekLine() override = default;

	virtual bool LoadDB(CString strStockSymbol = _T("")) override final;
	virtual bool SaveDB(CString strStockSymbol = _T("")) override final;

	bool SaveCurrentWeekLine(void);
	bool LoadCurrentWeekLine(void);

	void UpdateData(vector<CWeekLinePtr>& vTempWeekLine);
	bool UpdateData(CVirtualHistoryCandleExtendPtr pHistoryCandleExtend);

	bool StoreVectorData(vector<CWeekLinePtr>& vWeekLine);
};

typedef shared_ptr<CDataChinaWeekLine> CDataChinaWeekLinePtr;
