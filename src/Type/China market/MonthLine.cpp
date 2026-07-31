module;

module HistoryCandle.MonthLine;

import FireBirdLib.Stock.ChinaStock;

namespace {
	CChinaStock s_stockMonthLine;
}

CMonthLine::CMonthLine() {
	m_ratio = s_stockMonthLine.GetRatio();
}
