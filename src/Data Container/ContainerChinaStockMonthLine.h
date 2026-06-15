#pragma once

#include "VirtualDataHistoryCandle.h"
#include"MonthLine.h"

class CContainerChinaStockMonthLine : public CVirtualDataHistoryCandle {
public:
	CContainerChinaStockMonthLine();
	~CContainerChinaStockMonthLine() override = default;
	CContainerChinaStockMonthLine(const CContainerChinaStockMonthLine& other) = delete;
	CContainerChinaStockMonthLine(CContainerChinaStockMonthLine&& other) noexcept = delete;
	CContainerChinaStockMonthLine& operator=(const CContainerChinaStockMonthLine& other) = delete;
	CContainerChinaStockMonthLine& operator=(CContainerChinaStockMonthLine&& other) noexcept = delete;

	CMonthLine* GetData(const size_t lIndex) { return static_cast<CMonthLine*>((CVirtualDataHistoryCandle::GetData(lIndex))); }
	CMonthLine* GetDayLine(chrono::local_days lDate) { return static_cast<CMonthLine*>(CVirtualDataHistoryCandle::GetCandle(lDate)); }

	bool Add(CMonthLine& data) {
		data.SetRatio(m_ratio);
		m_vHistoryData.push_back(data);
		return true;
	}
};

using CContainerChinaStockMonthLinePtr = shared_ptr<CContainerChinaStockMonthLine>;
