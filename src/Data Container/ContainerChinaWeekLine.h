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

	void UpdateData(const vector<CWeekLine>& vTempWeekLine);
	void UpdateData(const CVirtualHistoryCandle* pHistoryCandle);

	void StoreVectorData(const vector<CWeekLine>& vWeekLine);

	CWeekLine* GetData(const size_t lIndex) { return static_cast<CWeekLine*>(CVirtualDataHistoryCandle::GetData(lIndex)); }
	CWeekLine* GetWeekLine(chrono::local_days lDate) { return static_cast<CWeekLine*>(CVirtualDataHistoryCandle::GetCandle(lDate)); }
};

using CContainerChinaWeekLinePtr = shared_ptr<CContainerChinaWeekLine>;
