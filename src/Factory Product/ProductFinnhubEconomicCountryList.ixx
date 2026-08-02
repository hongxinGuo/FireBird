module;

export module FireBirdLib.Product.Finnhub.EconomicCountryList;

import FireBirdLib.Product.Finnhub;
import FireBirdLib.Country;
import FireBirdLib.WebData;

import std;
using std::shared_ptr;
using std::string;
using std::vector;

export {
	class CProductFinnhubEconomicCountryList final : public CProductFinnhub {
	public:
		CProductFinnhubEconomicCountryList();
		~CProductFinnhubEconomicCountryList() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CCountry>> ParseFinnhubCountryList(const CWebDataPtr& pWebData);

		void UpdateSystemStatus() override;
	};

	using CFinnhubEconomicCountryListPtr = shared_ptr<CProductFinnhubEconomicCountryList>;
}