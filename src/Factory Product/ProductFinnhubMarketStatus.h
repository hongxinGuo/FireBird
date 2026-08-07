#pragma once

#include"ProductFinnhub.h"

class CMarketStatus;

class CProductFinnhubMarketStatus final : public CProductFinnhub {
public:
	CProductFinnhubMarketStatus();
	~CProductFinnhubMarketStatus() override = default;

	shared_ptr<vector<string>> CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CMarketStatus>> ParseFinnhubMarketStatus(const CWebDataPtr& pWebData);
};

using CProductFinnhubMarketStatusPtr = shared_ptr<CProductFinnhubMarketStatus>;
