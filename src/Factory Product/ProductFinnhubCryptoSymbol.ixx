module;

export module FireBirdLib.Product.Finnhub.CryptoSymbol;

import FireBirdLib.Product.Finnhub;
import FireBirdLib.WebData;
import FireBirdLib.Stock.FinnhubCrypto;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubCryptoSymbol final : public CProductFinnhub {
	public:
		CProductFinnhubCryptoSymbol();

		~CProductFinnhubCryptoSymbol() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<shared_ptr<CFinnhubCrypto>>> ParseFinnhubCryptoSymbol(const CWebDataPtr& pWebData);
	};

	using CFinnhubCryptoSymbolProductPtr = shared_ptr<CProductFinnhubCryptoSymbol>;
}