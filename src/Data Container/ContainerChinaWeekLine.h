#pragma once

#include "VirtualDataHistoryCandleExtend.h"
#include"WeekLine.h"

class CContainerChinaWeekLine : public CVirtualDataHistoryCandleExtend {
public:
	CContainerChinaWeekLine();
	~CContainerChinaWeekLine() override = default;

	bool LoadDB(const CString& strStockSymbol) final;
	bool SaveDB(const CString& strStockSymbol) final;

	bool SaveCurrentWeekLine(void);
	bool LoadCurrentWeekLine(void);

	void UpdateData(vector<CWeekLinePtr>& vTempWeekLine);
	bool UpdateData(CVirtualHistoryCandleExtendPtr pHistoryCandleExtend);

	bool StoreVectorData(vector<CWeekLinePtr>& vWeekLine);
};

using CContainerChinaWeekLinePtr = shared_ptr<CContainerChinaWeekLine>;
