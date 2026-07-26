#pragma once

#include"ProductFinnhub.h"

class CFinnhubCrypto;

class CProductFinnhubCryptoSymbol final : public CProductFinnhub {
public:
	CProductFinnhubCryptoSymbol();

	~CProductFinnhubCryptoSymbol() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<shared_ptr<CFinnhubCrypto>>> ParseFinnhubCryptoSymbol(const CWebDataPtr& pWebData);
};

using CFinnhubCryptoSymbolProductPtr = shared_ptr<CProductFinnhubCryptoSymbol>;
