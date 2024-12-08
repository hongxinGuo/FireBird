#include "pch.h"

#include"TiingoStock.h"
module FireBird.HistoryCandle.TiingoDayLine; 

CTiingoDayLine::CTiingoDayLine() {
}

int CTiingoDayLine::GetRatio() const {
	CTiingoStock stock;
	return stock.GetRatio();
}
