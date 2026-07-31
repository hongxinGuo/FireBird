module;

module HistoryCandle.DayLine;
import FireBirdLib.Stock.ChinaStock;

namespace {
	CChinaStock s_stock;
}

CDayLine::CDayLine() {
	m_ratio = s_stock.GetRatio();
}
