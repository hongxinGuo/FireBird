#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCryptoExchange final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCryptoExchange)
	CProductFinnhubCryptoExchange();
	~CProductFinnhubCryptoExchange() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	shared_ptr<vector<CString>> ParseFinnhubCryptoExchange(CWebDataPtr pWebData);
	shared_ptr<vector<CString>> ParseFinnhubCryptoExchange2(CWebDataPtr pWebData);
};

using CFinnhubCryptoExchangePtr = shared_ptr<CProductFinnhubCryptoExchange>;
