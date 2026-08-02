module;

export module FireBirdLib.Product.Finnhub.ForexDayLine;

import FireBirdLib.Product.Finnhub;
import FireBirdLib.WebData;
import FireBirdLib.HistoryCandle.DayLine;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubForexDayLine final : public CProductFinnhub {
	public:
		CProductFinnhubForexDayLine();
		~CProductFinnhubForexDayLine() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CDayLine>> ParseFinnhubForexCandle(const CWebDataPtr& pWebData);
	};

	using CFinnhubForexDayLinePtr = shared_ptr<CProductFinnhubForexDayLine>;
}