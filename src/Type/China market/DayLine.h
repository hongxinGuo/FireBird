#pragma once

#include"VirtualHistoryCandleExtend.h"

#include<memory>

class CDayLine;
using CDayLinePtr = shared_ptr<CDayLine>;
using CDayLineVectorPtr = shared_ptr<vector<CDayLinePtr>>;

class CDayLine final : public CVirtualHistoryCandleExtend {
public:
	CDayLine();
	void Reset(void) override; // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

private:
};
