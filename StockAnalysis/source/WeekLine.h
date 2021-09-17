#pragma once

#include"ChinaStockHistoryCandle.h"
#include"SetCurrentWeekLineInfo.h"
#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"DayLine.h"

class CWeekLine : public CChinaStockHistoryCandle {
public:
	CWeekLine();
	void Reset(void); // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

	bool AppendCurrentWeekData(CCurrentWeekLineInfo* psetCurrentWeekLineInfo);
	bool LoadCurrentWeekData(CCurrentWeekLineInfo* psetCurrentWeekLineInfo);

	bool UpdateWeekLine(CDayLinePtr pDayLine);

private:
};

typedef shared_ptr<CWeekLine> CWeekLinePtr;
