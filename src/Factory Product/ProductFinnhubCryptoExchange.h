#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCryptoExchange final : public CProductFinnhub {
public:
	CProductFinnhubCryptoExchange();
	~CProductFinnhubCryptoExchange() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;

	shared_ptr<vector<string>> ParseFinnhubCryptoExchange(const CWebDataPtr& pWebData);
	void UpdateSystemStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CFinnhubCryptoExchangePtr = shared_ptr<CProductFinnhubCryptoExchange>;
