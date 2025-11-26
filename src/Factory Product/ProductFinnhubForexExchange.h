#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubForexExchange final : public CProductFinnhub {
public:
	CProductFinnhubForexExchange();
	~CProductFinnhubForexExchange() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<string>> ParseFinnhubForexExchange(const CWebDataPtr& pWebData);

	void UpdateSystemStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CFinnhubForexExchangePtr = shared_ptr<CProductFinnhubForexExchange>;
