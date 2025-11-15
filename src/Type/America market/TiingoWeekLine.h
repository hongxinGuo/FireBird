#pragma once

#include"VirtualHistoryCandleExtend.h"

#include<memory>
using std::vector;

class CTiingoWeekLine : public CVirtualHistoryCandleExtend {
public:
	CTiingoWeekLine();
	int GetRatio() const override; // TiingoDayLine的比率与TiingoStock相同
};

using CTiingoWeekLinePtr = shared_ptr<CTiingoWeekLine>;
using CTiingoWeekLinesPtr = shared_ptr<vector<CTiingoWeekLinePtr>>;
