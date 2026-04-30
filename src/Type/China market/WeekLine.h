#pragma once

#include"VirtualHistoryCandleBasic.h"

class CWeekLine final : public CVirtualHistoryCandleBasic {
public:
	CWeekLine() = default;

	bool UpdateWeekLine(const CVirtualHistoryCandleBasicPtr& pHistoryCandleExtend);
};

using CWeekLinePtr = shared_ptr<CWeekLine>;
