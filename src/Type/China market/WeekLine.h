#pragma once

#include"VirtualHistoryCandle.h"

class CWeekLine final : public CVirtualHistoryCandle {
public:
	CWeekLine() = default;

	bool UpdateWeekLine(const CVirtualHistoryCandlePtr& pHistoryCandleExtend);
};

using CWeekLinePtr = shared_ptr<CWeekLine>;
