#pragma once

#include"ProductWebData.h"

class CProductFinnhubCryptoExchange : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCryptoExchange)
	CProductFinnhubCryptoExchange();
	~CProductFinnhubCryptoExchange() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	shared_ptr<vector<CString>> ParseFinnhubCryptoExchange(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCryptoExchange> CFinnhubCryptoExchangePtr;