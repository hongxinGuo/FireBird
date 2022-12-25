#pragma once

#include "VirtualDataHistoryCandleExtend.h"
#include"WeekLine.h"

class CDataChinaWeekLine : public CVirtualDataHistoryCandleExtend {
public:
	CDataChinaWeekLine();
	~CDataChinaWeekLine() override = default;

	bool LoadDB(const CString& strStockSymbol = _T("")) final;
	bool SaveDB(const CString& strStockSymbol = _T("")) final;

	bool SaveCurrentWeekLine(void);
	bool LoadCurrentWeekLine(void);

	void UpdateData(vector<CWeekLinePtr>& vTempWeekLine);
	bool UpdateData(CVirtualHistoryCandleExtendPtr pHistoryCandleExtend);

	bool StoreVectorData(vector<CWeekLinePtr>& vWeekLine);
};

using CDataChinaWeekLinePtr = shared_ptr<CDataChinaWeekLine>;
