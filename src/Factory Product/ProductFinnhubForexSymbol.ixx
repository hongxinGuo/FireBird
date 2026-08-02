module;

export module FireBirdLib.Product.Finnhub.ForexSymbol;

import FireBirdLib.Product.Finnhub;
import FireBirdLib.WebData;
import FireBirdLib.Stock.FinnhubForex;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubForexSymbol final : public CProductFinnhub {
	public:
		CProductFinnhubForexSymbol();
		~CProductFinnhubForexSymbol() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<shared_ptr<CFinnhubForex>>> ParseFinnhubForexSymbol(const CWebDataPtr& pWebData);
	};

	using CProductFinnhubForexSymbolPtr = shared_ptr<CProductFinnhubForexSymbol>;
}