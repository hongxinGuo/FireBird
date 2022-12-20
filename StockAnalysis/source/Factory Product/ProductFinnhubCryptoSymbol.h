#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubCryptoSymbol final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCryptoSymbol)
	CProductFinnhubCryptoSymbol();

	~CProductFinnhubCryptoSymbol() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CFinnhubCryptoSymbolVectorPtr ParseFinnhubCryptoSymbol(CWebDataPtr pWebData);
};

using CFinnhubCryptoSymbolProductPtr = shared_ptr<CProductFinnhubCryptoSymbol>;
