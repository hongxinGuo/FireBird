module;

export module FireBirdLib.HistoryCandle.TiingoCandleLine;

import FireBirdLib.HistoryCandle.Virtual;

import std;
using std::vector;
using std::shared_ptr;

export {
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
}
