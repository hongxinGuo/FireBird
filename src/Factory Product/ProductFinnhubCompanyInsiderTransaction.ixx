module;

export module FireBirdLib.Product.Finnhub.CompanyInsiderTransaction;

export import FireBirdLib.Product.Finnhub;
import FireBirdLib.WebData;
import FireBirdLib.Type.InsiderTransaction;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubCompanyInsiderTransaction final : public CProductFinnhub {
	public:
		CProductFinnhubCompanyInsiderTransaction();
		~CProductFinnhubCompanyInsiderTransaction() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		void UpdateSystemStatus() override;
		shared_ptr<vector<CInsiderTransaction>> ParseFinnhubStockInsiderTransaction(const CWebDataPtr& pWebData);
	};

	using CFinnhubCompanyInsiderTransactionPtr = shared_ptr<CProductFinnhubCompanyInsiderTransaction>;
}