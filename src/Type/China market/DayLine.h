#pragma once

#include"VirtualHistoryCandleExtend.h"

#include<memory>
using std::vector;

class CDayLine final : public CVirtualHistoryCandleExtend {
public:
	CDayLine();
	void Reset() override; // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

private:
};

using CDayLinePtr = shared_ptr<CDayLine>;
using CDayLineVectorPtr = shared_ptr<vector<CDayLinePtr>>;
