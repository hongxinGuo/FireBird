module;

export module ProductFinnhubStockPriceQuete;

import ProductFinnhub;

import FinnhubStock;
import WebData;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubStockPriceQuote final : public CProductFinnhub {
	public:
		CProductFinnhubStockPriceQuote();
		~CProductFinnhubStockPriceQuote() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		bool ParseFinnhubStockQuote(const CWebDataPtr& pWebData, const shared_ptr<CFinnhubStock>& pStock);
	};

	using CFinnhubStockPriceQuotePtr = shared_ptr<CProductFinnhubStockPriceQuote>;
}