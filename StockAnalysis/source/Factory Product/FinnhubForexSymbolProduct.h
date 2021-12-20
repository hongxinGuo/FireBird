#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubForexSymbolProduct : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubForexSymbolProduct)
	CFinnhubForexSymbolProduct();
	~CFinnhubForexSymbolProduct() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CForexSymbolVectorPtr ParseFinnhubForexSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CFinnhubForexSymbolProduct> CFinnhubForexSymbolProductPtr;