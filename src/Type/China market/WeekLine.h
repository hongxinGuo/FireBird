#pragma once

#include"VirtualHistoryCandleExtend.h"

class CWeekLine final : public CVirtualHistoryCandleExtend {
public:
	CWeekLine() = default;

	bool UpdateWeekLine(const CVirtualHistoryCandleExtendPtr& pHistoryCandleExtend);
};

using CWeekLinePtr = shared_ptr<CWeekLine>;
