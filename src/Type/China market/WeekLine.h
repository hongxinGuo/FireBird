#pragma once

#include"VirtualHistoryCandle.h"

class CWeekLine final : public CVirtualHistoryCandle {
public:
	CWeekLine();

	bool UpdateWeekLine(const CVirtualHistoryCandle* pDayLine);
};

using CWeekLinePtr = shared_ptr<CWeekLine>;
