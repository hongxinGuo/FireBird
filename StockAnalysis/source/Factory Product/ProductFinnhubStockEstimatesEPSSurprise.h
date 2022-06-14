#pragma once

#include"ProductWebData.h"
#include"EPSSurprise.h"

class CProductFinnhubStockEstimatesEPSSurprise : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockEstimatesEPSSurprise)
	CProductFinnhubStockEstimatesEPSSurprise();
	~CProductFinnhubStockEstimatesEPSSurprise() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CEPSSurpriseVectorPtr ParseFinnhubEPSSurprise(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubStockEstimatesEPSSurprise> CFinnhubStockEstimatesEPSSurprisePtr;