#include "pch.h"
module;
module FireBird.HistoryCandle.TiingoDayLine; 
import FireBird.Stock.TiingoStock;

CTiingoDayLine::CTiingoDayLine() {
}

int CTiingoDayLine::GetRatio() const {
	CTiingoStock stock;
	return stock.GetRatio();
}
