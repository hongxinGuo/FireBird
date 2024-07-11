#pragma once

#include "MarketStatus.h"
#include"ProductFinnhub.h"

class CProductFinnhubMarketStatus final : public CProductFinnhub {
public:
	CProductFinnhubMarketStatus();
	~CProductFinnhubMarketStatus() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CMarketStatussPtr ParseFinnhubMarketStatus(const CWebDataPtr& pWebData);
};

using CProductFinnhubMarketStatusPtr = shared_ptr<CProductFinnhubMarketStatus>;
