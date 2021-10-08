#include"pch.h"

#include"globedef.h"
#include "DayLine.h"

CDayLine::CDayLine() : CVirtualHistoryCandleExtend() {
	Reset();
}

void CDayLine::Reset(void) {
	CVirtualHistoryCandleExtend::Reset();
}

bool CDayLine::IsActive(void) const {
	if ((GetClose() != 0) && (GetLastClose() != 0)) {
		return true;
	}
	else return false;
}