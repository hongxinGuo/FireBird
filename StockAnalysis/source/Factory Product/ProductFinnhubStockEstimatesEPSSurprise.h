#pragma once

#include"ProductFinnhub.h"
#include"EPSSurprise.h"

class CProductFinnhubStockEstimatesEPSSurprise final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockEstimatesEPSSurprise)
	CProductFinnhubStockEstimatesEPSSurprise();
	~CProductFinnhubStockEstimatesEPSSurprise() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CEPSSurpriseVectorPtr ParseFinnhubEPSSurprise(CWebDataPtr pWebData);
	CEPSSurpriseVectorPtr ParseFinnhubEPSSurprise2(CWebDataPtr pWebData);
};

using CFinnhubStockEstimatesEPSSurprisePtr = shared_ptr<CProductFinnhubStockEstimatesEPSSurprise>;
