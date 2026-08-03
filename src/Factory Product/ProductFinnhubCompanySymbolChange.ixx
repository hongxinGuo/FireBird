module;

export module FireBirdLib.Product.Finnhub.CompanySymbolChange;

import FireBirdLib.Product.Finnhub;
import FireBirdLib.Type.FinnhubCompanySymbolChange;
import FireBirdLib.WebData;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubCompanySymbolChange final : public CProductFinnhub {
	public:
		CProductFinnhubCompanySymbolChange();
		~CProductFinnhubCompanySymbolChange() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CFinnhubCompanySymbolChange>> ParseFinnhubCompanySymbolChange(const CWebDataPtr& pWebData);

		void UpdateSystemStatus() override;
	};

	using CFinnhubCompanySymbolChangePtr = shared_ptr<CProductFinnhubCompanySymbolChange>;
}