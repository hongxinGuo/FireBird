#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubCryptoExchange final : public CProductFinnhub {
public:
	CProductFinnhubCryptoExchange();
	~CProductFinnhubCryptoExchange() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	shared_ptr<vector<string>> CreateMessage() override;
	void UpdateSystemStatus() override;
	shared_ptr<vector<string>> Parse(const string& text);
};

using CFinnhubCryptoExchangePtr = shared_ptr<CProductFinnhubCryptoExchange>;
