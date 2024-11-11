#pragma once

#include"VirtualHistoryCandleBasic.h"

#include<memory>
using std::vector;

class CTiingoDayLine : public CVirtualHistoryCandleBasic {
public:
	CTiingoDayLine();
	int GetRatio() const override; // TiingoDayLine�ı�����TiingoStock��ͬ
};

using CTiingoDayLinePtr = shared_ptr<CTiingoDayLine>;
using CTiingoDayLinesPtr = shared_ptr<vector<CTiingoDayLinePtr>>;
