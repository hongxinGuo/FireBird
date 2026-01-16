#include"pch.h"

#include "DayLine.h"

namespace {
	CChinaStock s_stock;
}

CDayLine::CDayLine() {
}

int CDayLine::GetRatio() const {
	return s_stock.GetRatio();
}
