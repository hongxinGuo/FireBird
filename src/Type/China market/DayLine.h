#pragma once

#include"VirtualHistoryCandle.h"

#include<memory>

using std::vector;

class CDayLine final : public CVirtualHistoryCandle {
public:
	CDayLine();
	int GetRatio() const override;
};

using CDayLinePtr = shared_ptr<CDayLine>;
using CDayLinesPtr = shared_ptr<vector<CDayLinePtr>>;
