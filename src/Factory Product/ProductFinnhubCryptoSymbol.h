#pragma once

#include "FinnhubCrypto.h"
#include"ProductFinnhub.h"

class CFinnhubCrypto;

class CProductFinnhubCryptoSymbol final : public CProductFinnhub {
public:
	CProductFinnhubCryptoSymbol();

	~CProductFinnhubCryptoSymbol() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CFinnhubCryptosPtr Parse(const string& text);
};

using CFinnhubCryptoSymbolProductPtr = shared_ptr<CProductFinnhubCryptoSymbol>;
