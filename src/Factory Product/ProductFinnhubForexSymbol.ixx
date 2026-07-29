module;

export module ProductFinnhubForexSymbol;

import ProductFinnhub;
import WebData;
import FinnhubForex;

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