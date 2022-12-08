#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubCryptoSymbol : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCryptoSymbol)
		CProductFinnhubCryptoSymbol();
	~CProductFinnhubCryptoSymbol() {}

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CFinnhubCryptoSymbolVectorPtr ParseFinnhubCryptoSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCryptoSymbol> CFinnhubCryptoSymbolProductPtr;