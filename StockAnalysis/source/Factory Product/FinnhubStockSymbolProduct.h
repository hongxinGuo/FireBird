#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubStockSymbolProduct : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubStockSymbolProduct)
	CFinnhubStockSymbolProduct();
	~CFinnhubStockSymbolProduct() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubStockSymbolProduct> CFinnhubCompanySymbolProductPtr;