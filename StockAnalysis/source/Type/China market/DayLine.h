#pragma once

#include"VirtualHistoryCandleExtend.h"

#include<gsl/gsl>
using namespace gsl;

#include<memory>

class CDayLine;
typedef shared_ptr<CDayLine> CDayLinePtr;
typedef shared_ptr<vector<CDayLinePtr>> CDayLineVectorPtr;

class CDayLine final : public CVirtualHistoryCandleExtend {
public:
	CDayLine();
	void Reset(void); // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

private:
};
