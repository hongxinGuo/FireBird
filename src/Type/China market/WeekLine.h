#pragma once

#include"VirtualHistoryCandleExtend.h"

class CWeekLine final : public CVirtualHistoryCandleExtend {
public:
	CWeekLine();
	void Reset() override; // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

	bool UpdateWeekLine(const CVirtualHistoryCandleExtendPtr& pHistoryCandleExtend);
};

using CWeekLinePtr = shared_ptr<CWeekLine>;
