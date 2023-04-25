#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubCryptoSymbol final : public CProductFinnhub {
public:
	CProductFinnhubCryptoSymbol();

	~CProductFinnhubCryptoSymbol() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CFinnhubCryptoSymbolVectorPtr ParseFinnhubCryptoSymbol(const CWebDataPtr& pWebData);
};

using CFinnhubCryptoSymbolProductPtr = shared_ptr<CProductFinnhubCryptoSymbol>;
