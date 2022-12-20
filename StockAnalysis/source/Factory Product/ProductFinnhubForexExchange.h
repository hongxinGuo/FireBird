#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubForexExchange final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexExchange)
	CProductFinnhubForexExchange();
	~CProductFinnhubForexExchange() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	shared_ptr<vector<CString>> ParseFinnhubForexExchange(CWebDataPtr pWebData);
	shared_ptr<vector<CString>> ParseFinnhubForexExchange2(CWebDataPtr pWebData);
};

using CFinnhubForexExchangePtr = shared_ptr<CProductFinnhubForexExchange>;
