#include"pch.h"
#include<memory>
export module FireBird.HistoryCandle.DayLine;

import FireBird.HistoryCandle.VirtualExtend;

using std::vector;

export {
	class CDayLine final : public CVirtualHistoryCandleExtend {
	public:
		CDayLine();
		int GetRatio() const override { return 1000; };
	};

	using CDayLinePtr = shared_ptr<CDayLine>;
	using CDayLinesPtr = shared_ptr<vector<CDayLinePtr>>;
}
