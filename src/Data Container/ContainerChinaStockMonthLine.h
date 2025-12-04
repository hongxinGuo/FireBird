#pragma once

#include "VirtualDataHistoryCandleExtend.h"
#include"MonthLine.h"

class CContainerChinaStockMonthLine : public CVirtualDataHistoryCandleExtend {
public:
	CContainerChinaStockMonthLine() = default;
	~CContainerChinaStockMonthLine() override = default;
	CContainerChinaStockMonthLine(const CContainerChinaStockMonthLine& other) = delete;
	CContainerChinaStockMonthLine(CContainerChinaStockMonthLine&& other) noexcept = delete;
	CContainerChinaStockMonthLine& operator=(const CContainerChinaStockMonthLine& other) = delete;
	CContainerChinaStockMonthLine& operator=(CContainerChinaStockMonthLine&& other) noexcept = delete;

	CMonthLinePtr GetData(const size_t lIndex) const { return dynamic_pointer_cast<CMonthLine>(CVirtualDataHistoryCandleExtend::GetData(lIndex)); }
	CMonthLinePtr GetDayLine(long lDate) { return dynamic_pointer_cast<CMonthLine>(CVirtualDataHistoryCandleExtend::GetDayLine(lDate)); }

	bool Add(const CMonthLinePtr& pData) {
		m_vHistoryData.push_back(pData);
		return true;
	}
};

using CContainerChinaStockMonthLinePtr = shared_ptr<CContainerChinaStockMonthLine>;
