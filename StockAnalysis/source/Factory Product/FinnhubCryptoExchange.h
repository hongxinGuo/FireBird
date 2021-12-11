#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCryptoExchange : public CWebSourceDataProduct {
public:
	CFinnhubCryptoExchange();
	~CFinnhubCryptoExchange() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubCryptoExchange> CFinnhubCryptoExchangePtr;