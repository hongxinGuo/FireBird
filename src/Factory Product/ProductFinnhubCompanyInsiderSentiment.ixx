module;

export module FireBirdLib.Product.Finnhub.CompanyInsiderSentiment;

export import FireBirdLib.Product.Finnhub;
import FireBirdLib.WebData;
import FireBirdLib.Type.InsiderSentiment;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubCompanyInsiderSentiment final : public CProductFinnhub {
	public:
		CProductFinnhubCompanyInsiderSentiment();
		~CProductFinnhubCompanyInsiderSentiment() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		void UpdateSystemStatus() override;

		shared_ptr<vector<CInsiderSentiment>> ParseFinnhubStockInsiderSentiment(const CWebDataPtr& pWebData);
	};

	using CProductFinnhubCompanyInsiderSentimentPtr = shared_ptr<CProductFinnhubCompanyInsiderSentiment>;
}