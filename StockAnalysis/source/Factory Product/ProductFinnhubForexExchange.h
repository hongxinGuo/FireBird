#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubForexExchange final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexExchange)
	CProductFinnhubForexExchange();
	~CProductFinnhubForexExchange() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	shared_ptr<vector<CString>> ParseFinnhubForexExchange(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubForexExchange> CFinnhubForexExchangePtr;
