#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCryptoSymbolProduct : public CWebSourceDataProduct {
public:
	CFinnhubCryptoSymbolProduct();
	~CFinnhubCryptoSymbolProduct() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubCryptoSymbolProduct> CFinnhubCryptoSymbolProductPtr;