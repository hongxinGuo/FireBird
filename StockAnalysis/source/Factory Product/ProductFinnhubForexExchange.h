#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubForexExchange : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexExchange)
		CProductFinnhubForexExchange();
	~CProductFinnhubForexExchange() {}

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	shared_ptr<vector<CString>> ParseFinnhubForexExchange(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubForexExchange> CFinnhubForexExchangePtr;