module;

export module FireBirdLib.Product.Finnhub.CompanyProfileConcise;

import FireBirdLib.Product.Finnhub;
import FireBirdLib.WebData;
import FireBirdLib.Stock.FinnhubStock;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubCompanyProfileConcise final : public CProductFinnhub {
	public:
		CProductFinnhubCompanyProfileConcise();
		~CProductFinnhubCompanyProfileConcise() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		bool ParseFinnhubStockProfileConcise(const CWebDataPtr& pWebData, const shared_ptr<CFinnhubStock>& pStock) const;
	};

	using CFinnhubCompanyProfileConcisePtr = shared_ptr<CProductFinnhubCompanyProfileConcise>;
}