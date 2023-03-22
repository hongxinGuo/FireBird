#pragma once

#include"VirtualHistoryCandleExtend.h"

#include<memory>
using std::vector;

class CDayLine final : public CVirtualHistoryCandleExtend {
public:
	CDayLine();
	void Reset() override; // 这些实现类需要采用这种方法重置内部状态，因为系统会一直运行，每天都需要重置状态。

private:
};

using CDayLinePtr = shared_ptr<CDayLine>;
using CDayLineVectorPtr = shared_ptr<vector<CDayLinePtr>>;
