#include "pch.h"

#include "TiingoDayLine.h"
#include"TiingoStock.h"

static CTiingoStock s_stock; // ���ڻ�ø��۸�ķŴ���ʡ�

CTiingoDayLine::CTiingoDayLine() {
}

int CTiingoDayLine::GetRatio() const {
	return s_stock.GetRatio();
}
