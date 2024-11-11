#include "pch.h"

#include "TiingoDayLine.h"
#include"TiingoStock.h"

CTiingoDayLine::CTiingoDayLine() {
}

int CTiingoDayLine::GetRatio() const {
	CTiingoStock stock;
	return stock.GetRatio();
}
