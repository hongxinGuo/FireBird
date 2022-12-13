#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubCryptoSymbol final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCryptoSymbol)
	CProductFinnhubCryptoSymbol();

	~CProductFinnhubCryptoSymbol() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CFinnhubCryptoSymbolVectorPtr ParseFinnhubCryptoSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCryptoSymbol> CFinnhubCryptoSymbolProductPtr;
