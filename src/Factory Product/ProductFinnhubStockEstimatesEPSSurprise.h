#pragma once

#include"ProductFinnhub.h"

class CEPSSurprise;

class CProductFinnhubStockEstimatesEPSSurprise final : public CProductFinnhub {
public:
	CProductFinnhubStockEstimatesEPSSurprise();
	~CProductFinnhubStockEstimatesEPSSurprise() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CEPSSurprise>> ParseFinnhubEPSSurprise(const CWebDataPtr& pWebData);
};

using CFinnhubStockEstimatesEPSSurprisePtr = shared_ptr<CProductFinnhubStockEstimatesEPSSurprise>;
