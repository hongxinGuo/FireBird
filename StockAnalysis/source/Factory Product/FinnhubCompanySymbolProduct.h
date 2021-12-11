#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCompanySymbolProduct : public CWebSourceDataProduct {
public:
	CFinnhubCompanySymbolProduct();
	~CFinnhubCompanySymbolProduct() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubCompanySymbolProduct> CFinnhubCompanySymbolProductPtr;