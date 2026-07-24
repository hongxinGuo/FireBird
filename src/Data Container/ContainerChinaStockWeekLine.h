#pragma once

#include "VirtualDataHistoryCandle.h"
#include"WeekLine.h"

class CContainerChinaStockWeekLine : public CVirtualDataHistoryCandle {
public:
	CContainerChinaStockWeekLine();
	CContainerChinaStockWeekLine(const CContainerChinaStockWeekLine& other) = delete;
	CContainerChinaStockWeekLine(CContainerChinaStockWeekLine&& other) noexcept = delete;
	CContainerChinaStockWeekLine& operator=(const CContainerChinaStockWeekLine& other) = delete;
	CContainerChinaStockWeekLine& operator=(CContainerChinaStockWeekLine&& other) noexcept = delete;
	~CContainerChinaStockWeekLine() override = default;

	void UpdateData(const vector<CWeekLine>& vTempWeekLine);
	void UpdateData(const CVirtualHistoryCandle* pHistoryCandle);

	void StoreVectorData(const vector<CWeekLine>& vWeekLine);

	CWeekLine* GetData(const size_t lIndex) { return static_cast<CWeekLine*>(CVirtualDataHistoryCandle::GetData(lIndex)); }
	CWeekLine* GetWeekLine(local_days lDate) { return static_cast<CWeekLine*>(CVirtualDataHistoryCandle::GetCandle(lDate)); }
};

using CContainerChinaWeekLinePtr = shared_ptr<CContainerChinaStockWeekLine>;
