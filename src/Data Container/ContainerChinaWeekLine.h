#pragma once

#include "VirtualDataHistoryCandle.h"
#include"WeekLine.h"

class CContainerChinaWeekLine : public CVirtualDataHistoryCandle {
public:
	CContainerChinaWeekLine();
	CContainerChinaWeekLine(const CContainerChinaWeekLine& other) = delete;
	CContainerChinaWeekLine(CContainerChinaWeekLine&& other) noexcept = delete;
	CContainerChinaWeekLine& operator=(const CContainerChinaWeekLine& other) = delete;
	CContainerChinaWeekLine& operator=(CContainerChinaWeekLine&& other) noexcept = delete;
	~CContainerChinaWeekLine() override = default;

	bool LoadDB(const string& strStockSymbol) final;
	bool SaveDB(const string& strStockSymbol) final;

	void SaveCurrentWeekLine() const;
	bool LoadCurrentWeekLine();

	void UpdateData(const vector<CWeekLine>& vTempWeekLine);
	void UpdateData(const CVirtualHistoryCandle* pHistoryCandle);

	void StoreVectorData(const vector<CWeekLine>& vWeekLine);

	CWeekLine* GetData(const size_t lIndex) { return static_cast<CWeekLine*>(CVirtualDataHistoryCandle::GetData(lIndex)); }
	CWeekLine* GetWeekLine(long lDate) { return static_cast<CWeekLine*>(CVirtualDataHistoryCandle::GetCandle(lDate)); }
};

using CContainerChinaWeekLinePtr = shared_ptr<CContainerChinaWeekLine>;
