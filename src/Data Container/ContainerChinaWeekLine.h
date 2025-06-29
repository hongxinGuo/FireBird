#pragma once

#include "VirtualDataHistoryCandleExtend.h"
#include"WeekLine.h"

class CContainerChinaWeekLine : public CVirtualDataHistoryCandleExtend {
public:
	CContainerChinaWeekLine();
	~CContainerChinaWeekLine() override = default;

	bool LoadDB(const string& strStockSymbol) final;
	CContainerChinaWeekLine(const CContainerChinaWeekLine& other) = delete;
	CContainerChinaWeekLine(CContainerChinaWeekLine&& other) noexcept = delete;
	CContainerChinaWeekLine& operator=(const CContainerChinaWeekLine& other) = delete;
	CContainerChinaWeekLine& operator=(CContainerChinaWeekLine&& other) noexcept = delete;
	bool SaveDB(const string& strStockSymbol) final;

	void SaveCurrentWeekLine() const;
	bool LoadCurrentWeekLine();

	void UpdateData(const vector<CWeekLinePtr>& vTempWeekLine);
	void UpdateData(const CVirtualHistoryCandleExtendPtr& pHistoryCandleExtend) const;

	void StoreVectorData(const vector<CWeekLinePtr>& vWeekLine);
};

using CContainerChinaWeekLinePtr = shared_ptr<CContainerChinaWeekLine>;
