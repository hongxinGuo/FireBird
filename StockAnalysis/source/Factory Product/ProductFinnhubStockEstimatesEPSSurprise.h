#pragma once

#include"ProductFinnhub.h"
#include"EPSSurprise.h"

class CProductFinnhubStockEstimatesEPSSurprise : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockEstimatesEPSSurprise)
		CProductFinnhubStockEstimatesEPSSurprise();
	~CProductFinnhubStockEstimatesEPSSurprise() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource = nullptr) override final;
	CEPSSurpriseVectorPtr ParseFinnhubEPSSurprise(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubStockEstimatesEPSSurprise> CFinnhubStockEstimatesEPSSurprisePtr;