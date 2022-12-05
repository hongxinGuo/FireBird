#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCryptoExchange : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCryptoExchange)
		CProductFinnhubCryptoExchange();
	~CProductFinnhubCryptoExchange() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	shared_ptr<vector<CString>> ParseFinnhubCryptoExchange(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCryptoExchange> CFinnhubCryptoExchangePtr;