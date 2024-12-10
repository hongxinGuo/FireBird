#include"pch.h"
module;
export module FireBird.HistoryCandle.TiingoDayLine;

import FireBird.HistoryCandle.VirtualBasic;

using std::vector;

export {
	class CTiingoDayLine : public CVirtualHistoryCandleBasic {
	public:
		CTiingoDayLine();
		int GetRatio() const override; // TiingoDayLine的比率与TiingoStock相同
	};

	using CTiingoDayLinePtr = shared_ptr<CTiingoDayLine>;
	using CTiingoDayLinesPtr = shared_ptr<vector<CTiingoDayLinePtr>>;
}
