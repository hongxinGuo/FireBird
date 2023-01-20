#pragma once

#include"ProductFinnhub.h"
#include"EPSSurprise.h"

class CProductFinnhubStockEstimatesEPSSurprise final : public CProductFinnhub {
public:
	CProductFinnhubStockEstimatesEPSSurprise();
	~CProductFinnhubStockEstimatesEPSSurprise() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CEPSSurpriseVectorPtr ParseFinnhubEPSSurprise(CWebDataPtr pWebData);
};

using CFinnhubStockEstimatesEPSSurprisePtr = shared_ptr<CProductFinnhubStockEstimatesEPSSurprise>;
