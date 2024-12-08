#include "pch.h"

import FireBird.HistoryCandle.TiingoDayLine;
#include"TiingoStock.h"

CTiingoDayLine::CTiingoDayLine() {
}

int CTiingoDayLine::GetRatio() const {
	CTiingoStock stock;
	return stock.GetRatio();
}
