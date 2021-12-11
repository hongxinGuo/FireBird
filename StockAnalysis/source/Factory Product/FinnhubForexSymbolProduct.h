#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubForexSymbolProduct : public CWebSourceDataProduct {
public:
	CFinnhubForexSymbolProduct();
	~CFinnhubForexSymbolProduct() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubForexSymbolProduct> CFinnhubForexSymbolProductPtr;