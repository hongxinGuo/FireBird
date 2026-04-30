#pragma once

#include"VirtualHistoryCandleBasic.h"

#include<memory>

using std::vector;

class CDayLine final : public CVirtualHistoryCandleBasic {
public:
	CDayLine();
	int GetRatio() const override;
};

using CDayLinePtr = shared_ptr<CDayLine>;
using CDayLinesPtr = shared_ptr<vector<CDayLinePtr>>;
