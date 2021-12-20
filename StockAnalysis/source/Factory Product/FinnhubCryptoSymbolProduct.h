#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCryptoSymbolProduct : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubCryptoSymbolProduct)
	CFinnhubCryptoSymbolProduct();
	~CFinnhubCryptoSymbolProduct() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CCryptoSymbolVectorPtr ParseFinnhubCryptoSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CFinnhubCryptoSymbolProduct> CFinnhubCryptoSymbolProductPtr;