module;

export module Product.Finnhub.StockSymbol;

import Product.Finnhub;
import Stock.FinnhubStock;

import WebData;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubStockSymbol final : public CProductFinnhub {
	public:
		CProductFinnhubStockSymbol();
		~CProductFinnhubStockSymbol() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		static bool IsBadStockSymbol(const string& strStockSymbol, const string& strExchangeCode);
		shared_ptr<vector<shared_ptr<CFinnhubStock>>> ParseFinnhubStockSymbol(const CWebDataPtr& pWebData);
	};

	using CProductFinnhubCompanySymbolPtr = shared_ptr<CProductFinnhubStockSymbol>;
}