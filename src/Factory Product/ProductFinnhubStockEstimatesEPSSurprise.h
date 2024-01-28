#pragma once

#include"ProductFinnhub.h"
#include"EPSSurprise.h"

class CProductFinnhubStockEstimatesEPSSurprise final : public CProductFinnhub {
public:
	CProductFinnhubStockEstimatesEPSSurprise();
	~CProductFinnhubStockEstimatesEPSSurprise() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CEPSSurpriseVectorPtr ParseFinnhubEPSSurprise(const CWebDataPtr& pWebData);
};

using CFinnhubStockEstimatesEPSSurprisePtr = shared_ptr<CProductFinnhubStockEstimatesEPSSurprise>;
