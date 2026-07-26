#include"pch.h"

#include"ContainerChinaStockMonthLine.h"
#include "ChinaStock.h"
#include"MonthLine.h"

namespace {
	CChinaStock s_stock;
}

CContainerChinaStockMonthLine::CContainerChinaStockMonthLine() {
	m_ratio = s_stock.GetRatio();
}

CMonthLine* CContainerChinaStockMonthLine::GetData(size_t lIndex) {
	return static_cast<CMonthLine*>((CVirtualDataHistoryCandle::GetData(lIndex)));
}

CMonthLine* CContainerChinaStockMonthLine::GetDayLine(local_days lDate) {
	return static_cast<CMonthLine*>(CVirtualDataHistoryCandle::GetCandle(lDate));
}

bool CContainerChinaStockMonthLine::Add(CMonthLine& data) {
	data.SetRatio(m_ratio);
	m_vHistoryData.push_back(data);
	return true;
}
