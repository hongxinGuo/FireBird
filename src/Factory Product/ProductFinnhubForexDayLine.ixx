module;

export module Product.Finnhub.ForexDayLine;

import Product.Finnhub;
import WebData;
import HistoryCandle.DayLine;

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