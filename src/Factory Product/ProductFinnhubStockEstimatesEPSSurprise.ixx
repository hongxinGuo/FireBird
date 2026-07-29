module;

export module ProductFinnhubStockEstimatesEPSSurprise;

import ProductFinnhub;
import EPSSurprise;

import WebData;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubStockEstimatesEPSSurprise final : public CProductFinnhub {
	public:
		CProductFinnhubStockEstimatesEPSSurprise();
		~CProductFinnhubStockEstimatesEPSSurprise() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CEPSSurprise>> ParseFinnhubEPSSurprise(const CWebDataPtr& pWebData);
	};

	using CFinnhubStockEstimatesEPSSurprisePtr = shared_ptr<CProductFinnhubStockEstimatesEPSSurprise>;
}