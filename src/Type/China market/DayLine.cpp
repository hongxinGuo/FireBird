#include"pch.h"

#include "DayLine.h"

static CChinaStock s_stock;

CDayLine::CDayLine() {
}

int CDayLine::GetRatio() const {
	return s_stock.GetRatio();
}
