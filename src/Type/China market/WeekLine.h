#pragma once

#include"VirtualHistoryCandleExtend.h"

class CWeekLine final : public CVirtualHistoryCandleExtend {
public:
	CWeekLine();
	void Reset(void) override; // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

	bool UpdateWeekLine(CVirtualHistoryCandleExtendPtr pHistoryCandleExtend);

private:
};

using CWeekLinePtr = shared_ptr<CWeekLine>;
