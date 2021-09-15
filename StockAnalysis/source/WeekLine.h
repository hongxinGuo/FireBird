#pragma once

#include"ChinaStockHistoryCandle.h"
#include"SetCurrentWeekLineInfo.h"
#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"DayLine.h"

class CWeekLine : public CChinaStockHistoryCandle {
public:
	CWeekLine();
	void Reset(void); // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

	bool AppendCurrentWeekData(CCurrentWeekLineInfo* psetCurrentWeekLineInfo);
	bool SaveExtendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);
	bool AppendExtendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);
	bool LoadCurrentWeekData(CCurrentWeekLineInfo* psetCurrentWeekLineInfo);
	bool LoadExtendData(CSetWeekLineExtendInfo* psetWeekLineExtendInfo);

	bool UpdateWeekLine(CDayLinePtr pDayLine);

private:
};

typedef shared_ptr<CWeekLine> CWeekLinePtr;
