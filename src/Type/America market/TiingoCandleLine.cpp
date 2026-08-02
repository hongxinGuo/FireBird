module;

module FireBirdLib.HistoryCandle.TiingoCandleLine;

import FireBirdLib.Stock.TiingoStock;

namespace {
	CTiingoStock s_stock; // 用于获得各价格的放大比率。
}

CTiingoCandleLine::CTiingoCandleLine() {
	m_ratio = s_stock.GetRatio();
}
