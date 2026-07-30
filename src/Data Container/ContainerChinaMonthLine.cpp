module;

module Container.HistoryCandle.ChinaStockMonthLine;

import Stock.ChinaStock;

import std;
using std::chrono::local_days;

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
