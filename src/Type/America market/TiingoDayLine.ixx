#include"pch.h"
module;
#include<memory>
export module FireBird.HistoryCandle.TiingoDayLine;

import FireBird.HistoryCandle.VirtualBasic;

using std::vector;

export {
	class CTiingoDayLine : public CVirtualHistoryCandleBasic {
	public:
		CTiingoDayLine();
		int GetRatio() const override; // TiingoDayLine�ı�����TiingoStock��ͬ
	};

	using CTiingoDayLinePtr = shared_ptr<CTiingoDayLine>;
	using CTiingoDayLinesPtr = shared_ptr<vector<CTiingoDayLinePtr>>;
}
