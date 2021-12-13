#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubStockEstimatesEPSSurprise : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubStockEstimatesEPSSurprise)
	CFinnhubStockEstimatesEPSSurprise();
	~CFinnhubStockEstimatesEPSSurprise() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubStockEstimatesEPSSurprise> CFinnhubStockEstimatesEPSSurprisePtr;