#pragma once

#include"pch.h"

#include"Accessory.h"

#include"VirtualHistoryCandleExtend.h"

#include<gsl/gsl>
using namespace gsl;

using namespace std;
#include<memory>

class CDayLine;
typedef shared_ptr<CDayLine> CDayLinePtr;

class CDayLine : public CVirtualHistoryCandleExtend {
public:
	CDayLine();
	void Reset(void);       // ��Щʵ������Ҫ�������ַ��������ڲ�״̬����Ϊϵͳ��һֱ���У�ÿ�춼��Ҫ����״̬��

	bool IsActive(void) const;

private:
};
