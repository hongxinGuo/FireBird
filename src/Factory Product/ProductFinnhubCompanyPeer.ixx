module;

export module FireBirdLib.Product.Finnhub.CompanyPeer;
export import FireBirdLib.Product.Finnhub;

import FireBirdLib.WebData;
import FireBirdLib.Accessory.NlohmannJsonDeclaration;


import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductFinnhubCompanyPeer final : public CProductFinnhub {
	public:
		CProductFinnhubCompanyPeer();
		~CProductFinnhubCompanyPeer() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		nlohmannJson ParseFinnhubStockPeer(const CWebDataPtr& pWebData);
	};

	using CFinnhubCompanyPeerPtr = shared_ptr<CProductFinnhubCompanyPeer>;
}