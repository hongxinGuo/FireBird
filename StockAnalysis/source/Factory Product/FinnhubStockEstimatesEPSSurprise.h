#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubStockEstimatesEPSSurprise : public CWebSourceDataProduct {
public:
	CFinnhubStockEstimatesEPSSurprise();
	~CFinnhubStockEstimatesEPSSurprise() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubStockEstimatesEPSSurprise> CFinnhubStockEstimatesEPSSurprisePtr;