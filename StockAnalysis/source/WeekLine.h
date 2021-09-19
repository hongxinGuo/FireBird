#pragma once

#include"VirtualHistoryCandleExtend.h"

class CWeekLine : public CVirtualHistoryCandleExtend {
public:
	CWeekLine();
	void Reset(void); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

	bool UpdateWeekLine(CDayLinePtr pDayLine);

private:
};

typedef shared_ptr<CWeekLine> CWeekLinePtr;
