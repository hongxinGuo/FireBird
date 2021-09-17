#pragma once

#include"ChinaStockHistoryCandle.h"
#include"SetCurrentWeekLine.h"
#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"DayLine.h"

class CWeekLine : public CChinaStockHistoryCandle {
public:
	CWeekLine();
	void Reset(void); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

	bool AppendCurrentWeekData(CSetCurrentWeekLine* psetCurrentWeekLineInfo);
	bool LoadCurrentWeekData(CSetCurrentWeekLine* psetCurrentWeekLineInfo);

	bool UpdateWeekLine(CDayLinePtr pDayLine);

private:
};

typedef shared_ptr<CWeekLine> CWeekLinePtr;
