module;

export module ProductFinnhubCompanyProfile;

import ProductFinnhub;
import WebData;
import FinnhubStock;

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