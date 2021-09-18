#pragma once

#include"VirtualHistoryCandleExtend.h"
#include"SetCurrentWeekLine.h"
#include"SetWeekLineBasicInfo.h"
#include"SetWeekLineExtendInfo.h"
#include"DayLine.h"

class CWeekLine : public CVirtualHistoryCandleExtend {
public:
	CWeekLine();
	void Reset(void); // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

	bool AppendCurrentWeekData(CSetCurrentWeekLine* psetCurrentWeekLineInfo);
	bool LoadCurrentWeekData(CSetCurrentWeekLine* psetCurrentWeekLineInfo);

	bool UpdateWeekLine(CDayLinePtr pDayLine);

private:
};

typedef shared_ptr<CWeekLine> CWeekLinePtr;
