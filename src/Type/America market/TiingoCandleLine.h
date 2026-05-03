#pragma once

#include"VirtualHistoryCandle.h"

#include<memory>
using std::vector;

class CTiingoCandleLine : public CVirtualHistoryCandle {
public:
	CTiingoCandleLine();
	CTiingoCandleLine(const CTiingoCandleLine& other) = default;
	CTiingoCandleLine& operator=(const CTiingoCandleLine& other) = default;
	CTiingoCandleLine(CTiingoCandleLine&& other) = default;
	CTiingoCandleLine& operator=(CTiingoCandleLine&& other) = default;
	~CTiingoCandleLine() override = default;
};

using CTiingoCandleLinePtr = shared_ptr<CTiingoCandleLine>;
using CTiingoCandleLinesPtr = shared_ptr<vector<CTiingoCandleLine>>;
