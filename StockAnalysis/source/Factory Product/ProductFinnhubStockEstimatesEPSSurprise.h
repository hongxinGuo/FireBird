#pragma once

#include"ProductFinnhub.h"
#include"EPSSurprise.h"

class CProductFinnhubStockEstimatesEPSSurprise final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockEstimatesEPSSurprise)
	CProductFinnhubStockEstimatesEPSSurprise();
	~CProductFinnhubStockEstimatesEPSSurprise() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CEPSSurpriseVectorPtr ParseFinnhubEPSSurprise(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubStockEstimatesEPSSurprise> CFinnhubStockEstimatesEPSSurprisePtr;
