#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubCryptoSymbol final : public CProductFinnhub {
public:
	CProductFinnhubCryptoSymbol();

	~CProductFinnhubCryptoSymbol() override = default;

	CString CreateMessage() final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CFinnhubCryptoSymbolVectorPtr ParseFinnhubCryptoSymbol(CWebDataPtr pWebData);
};

using CFinnhubCryptoSymbolProductPtr = shared_ptr<CProductFinnhubCryptoSymbol>;
