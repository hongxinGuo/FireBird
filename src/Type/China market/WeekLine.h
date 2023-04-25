#pragma once

#include"VirtualHistoryCandleExtend.h"

class CWeekLine final : public CVirtualHistoryCandleExtend {
public:
	CWeekLine();
	void Reset() override; // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

	bool UpdateWeekLine(const CVirtualHistoryCandleExtendPtr& pHistoryCandleExtend);
};

using CWeekLinePtr = shared_ptr<CWeekLine>;
