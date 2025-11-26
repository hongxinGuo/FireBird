#pragma once

#include"VirtualHistoryCandleExtend.h"

#include<memory>

using std::vector;

class CDayLine final : public CVirtualHistoryCandleExtend {
public:
	CDayLine();
	int GetRatio() const override;
};

using CDayLinePtr = shared_ptr<CDayLine>;
using CDayLinesPtr = shared_ptr<vector<CDayLinePtr>>;
