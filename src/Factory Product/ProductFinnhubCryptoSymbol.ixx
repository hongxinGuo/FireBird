module;

export module ProductFinnhubCryptoSymbol;

import ProductFinnhub;
import WebData;
import FinnhubCrypto;

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