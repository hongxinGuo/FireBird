module;

export module FireBirdLib.Product.Finnhub.CompanyProfile;

import FireBirdLib.Product.Finnhub;
import FireBirdLib.WebData;
import FireBirdLib.Stock.FinnhubStock;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubCompanyProfile final : public CProductFinnhub {
	public:
		CProductFinnhubCompanyProfile();
		~CProductFinnhubCompanyProfile() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		bool ParseFinnhubStockProfile(CWebDataPtr pWebData, shared_ptr<CFinnhubStock> pStock) const;
	};

	using CFinnhubCompanyProfilePtr = shared_ptr<CProductFinnhubCompanyProfile>;
}