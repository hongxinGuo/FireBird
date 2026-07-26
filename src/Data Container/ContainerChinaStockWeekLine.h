#pragma once

#include "VirtualDataHistoryCandle.h"

class CWeekLine;

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

	CWeekLine* GetData(size_t lIndex);
	CWeekLine* GetWeekLine(local_days lDate);
};

using CContainerChinaWeekLinePtr = shared_ptr<CContainerChinaStockWeekLine>;
