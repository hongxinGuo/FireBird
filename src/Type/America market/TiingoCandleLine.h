#pragma once

#include"VirtualHistoryCandle.h"

#include<memory>
using std::vector;

class CTiingoCandleLine : public CVirtualHistoryCandle {
public:
	CTiingoCandleLine() = default;
	~CTiingoCandleLine() override = default;
	int GetRatio() const override; // TiingoDayLine的比率与TiingoStock相同
};

using CTiingoCandleLinePtr = shared_ptr<CTiingoCandleLine>;
using CTiingoCandleLinesPtr = shared_ptr<vector<CTiingoCandleLinePtr>>;
