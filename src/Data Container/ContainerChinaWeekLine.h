#pragma once

#include "VirtualDataHistoryCandleBasic.h"
#include"WeekLine.h"

class CContainerChinaWeekLine : public CVirtualDataHistoryCandleBasic {
public:
	CContainerChinaWeekLine() = default;
	CContainerChinaWeekLine(const CContainerChinaWeekLine& other) = delete;
	CContainerChinaWeekLine(CContainerChinaWeekLine&& other) noexcept = delete;
	CContainerChinaWeekLine& operator=(const CContainerChinaWeekLine& other) = delete;
	CContainerChinaWeekLine& operator=(CContainerChinaWeekLine&& other) noexcept = delete;
	~CContainerChinaWeekLine() override = default;

	bool LoadDB(const string& strStockSymbol) final;
	bool SaveDB(const string& strStockSymbol) final;

	void SaveCurrentWeekLine() const;
	bool LoadCurrentWeekLine();

	void UpdateData(const vector<CWeekLinePtr>& vTempWeekLine);
	void UpdateData(const CVirtualHistoryCandleBasicPtr& pHistoryCandleExtend) const;

	void StoreVectorData(const vector<CWeekLinePtr>& vWeekLine);
};

using CContainerChinaWeekLinePtr = shared_ptr<CContainerChinaWeekLine>;
