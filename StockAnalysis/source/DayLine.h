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
	void Reset(void);       // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

	bool IsActive(void) const;

private:
};
