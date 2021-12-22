#pragma once

#include"ProductWebData.h"

class CProductFinnhubStockEstimatesEPSSurprise : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockEstimatesEPSSurprise)
	CProductFinnhubStockEstimatesEPSSurprise();
	~CProductFinnhubStockEstimatesEPSSurprise() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CEPSSurpriseVectorPtr ParseFinnhubEPSSurprise(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubStockEstimatesEPSSurprise> CFinnhubStockEstimatesEPSSurprisePtr;