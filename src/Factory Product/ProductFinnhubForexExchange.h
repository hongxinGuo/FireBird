#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubForexExchange final : public CProductFinnhub {
public:
	CProductFinnhubForexExchange();
	~CProductFinnhubForexExchange() override = default;

	CString CreateMessage() final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CString>> ParseFinnhubForexExchange(CWebDataPtr pWebData);
};

using CFinnhubForexExchangePtr = shared_ptr<CProductFinnhubForexExchange>;
