#pragma once

#include"ProductFinnhub.h"

class CWebData;

class CProductFinnhubCryptoExchange final : public CProductFinnhub {
public:
	CProductFinnhubCryptoExchange();
	~CProductFinnhubCryptoExchange() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(shared_ptr<CWebData> pWebData) override;

	shared_ptr<vector<string>> ParseFinnhubCryptoExchange(const shared_ptr<CWebData>& pWebData);
	void UpdateSystemStatus() override;
};

using CFinnhubCryptoExchangePtr = shared_ptr<CProductFinnhubCryptoExchange>;
