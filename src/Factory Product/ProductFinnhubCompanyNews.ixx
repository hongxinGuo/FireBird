/////////////////////////////////////////////////////////////////////////////////////////
//
// finnhub company news. available in North America only.
//
//
////////////////////////////////////////////////////////////////////////////////////////
module;

export module FireBirdLib.Product.Finnhub.CompanyNews;

export import FireBirdLib.Product.Finnhub;
import FireBirdLib.Type.FinnhubCompanyNews;
import FireBirdLib.WebData;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubCompanyNews final : public CProductFinnhub {
	public:
		CProductFinnhubCompanyNews();
		~CProductFinnhubCompanyNews() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CFinnhubCompanyNews>> ParseFinnhubCompanyNews(const CWebDataPtr& pWebData);
	};

	using CProductFinnhubCompanyNewsPtr = shared_ptr<CProductFinnhubCompanyNews>;
}