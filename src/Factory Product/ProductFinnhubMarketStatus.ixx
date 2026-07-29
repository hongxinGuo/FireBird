module;

export module ProductFinnhubMarketStatus;

import ProductFinnhub;
import WebData;
import MarketStatus;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubMarketStatus final : public CProductFinnhub {
	public:
		CProductFinnhubMarketStatus();
		~CProductFinnhubMarketStatus() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CMarketStatus>> ParseFinnhubMarketStatus(const CWebDataPtr& pWebData);
	};

	using CProductFinnhubMarketStatusPtr = shared_ptr<CProductFinnhubMarketStatus>;
}