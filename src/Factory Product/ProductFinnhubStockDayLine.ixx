module;

export module Product.Finnhub.StockDayLine;

import Product.Finnhub;

import HistoryCandle.DayLine;
import WebData;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubStockDayLine final : public CProductFinnhub {
	public:
		CProductFinnhubStockDayLine();
		~CProductFinnhubStockDayLine() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(shared_ptr<CWebData> pWebData) override;
		shared_ptr<vector<CDayLine>> ParseFinnhubStockCandle(const shared_ptr<CWebData>& pWebData);
	};

	using CFinnhubStockDayLinePtr = shared_ptr<CProductFinnhubStockDayLine>;
}