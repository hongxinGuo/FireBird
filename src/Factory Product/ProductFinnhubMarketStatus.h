#pragma once

#include "MarketStatus.h"
#include"ProductFinnhub.h"

class CProductFinnhubMarketStatus final : public CProductFinnhub {
public:
	CProductFinnhubMarketStatus();
	~CProductFinnhubMarketStatus() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CMarketStatusVectorPtr ParseFinnhubMarketStatus(const CWebDataPtr& pWebData);
};

using CProductFinnhubMarketStatusPtr = shared_ptr<CProductFinnhubMarketStatus>;
