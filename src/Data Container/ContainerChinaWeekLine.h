#pragma once

#include "VirtualDataHistoryCandleExtend.h"
#include"WeekLine.h"

class CContainerChinaWeekLine : public CVirtualDataHistoryCandleExtend {
public:
	CContainerChinaWeekLine();
	~CContainerChinaWeekLine() override = default;

	bool LoadDB(const CString& strStockSymbol) final;
	bool SaveDB(const CString& strStockSymbol) final;

	void SaveCurrentWeekLine() const;
	bool LoadCurrentWeekLine();

	void UpdateData(const vector<CWeekLinePtr>& vTempWeekLine);
	void UpdateData(const CVirtualHistoryCandleExtendPtr& pHistoryCandleExtend) const;

	void StoreVectorData(const vector<CWeekLinePtr>& vWeekLine);
};

using CContainerChinaWeekLinePtr = shared_ptr<CContainerChinaWeekLine>;
