#pragma once

#include "FinnhubForex.h"
#include"ProductFinnhub.h"

class CFinnhubForex;

class CProductFinnhubForexSymbol final : public CProductFinnhub {
public:
	CProductFinnhubForexSymbol();
	~CProductFinnhubForexSymbol() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CForexSymbolsPtr Parse(const string& text);
};

using CProductFinnhubForexSymbolPtr = shared_ptr<CProductFinnhubForexSymbol>;
