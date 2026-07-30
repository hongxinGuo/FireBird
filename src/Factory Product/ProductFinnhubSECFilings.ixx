module;

export module Product.Finnhub.SECFilings;

import Product.Finnhub;
import WebData;
import SECFiling;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubSECFilings final : public CProductFinnhub {
	public:
		CProductFinnhubSECFilings();
		~CProductFinnhubSECFilings() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CSECFiling>> ParseFinnhubStockSECFilings(const CWebDataPtr& pWebData);
	};

	using CFinnhubSECFilings = shared_ptr<CProductFinnhubSECFilings>;
}