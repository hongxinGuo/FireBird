#pragma once

#include"VirtualHistoryCandleExtend.h"

#include<memory>

class CDayLine;
using CDayLinePtr = shared_ptr<CDayLine>;
using CDayLineVectorPtr = shared_ptr<vector<CDayLinePtr>>;

class CDayLine final : public CVirtualHistoryCandleExtend {
public:
	CDayLine();
	void Reset(void) override; // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

private:
};
