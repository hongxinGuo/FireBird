#pragma once

#include"VirtualHistoryCandleExtend.h"

class CWeekLine : public CVirtualHistoryCandleExtend {
public:
	CWeekLine();
	void Reset(void); // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

	bool UpdateWeekLine(CDayLinePtr pDayLine);

private:
};

typedef shared_ptr<CWeekLine> CWeekLinePtr;
