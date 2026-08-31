#pragma once

#include "FinnhubStock.h"
#include"ProductFinnhub.h"

class CFinnhubStock;

class CProductFinnhubStockSymbol final : public CProductFinnhub {
public:
	CProductFinnhubStockSymbol();
	~CProductFinnhubStockSymbol() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;
	shared_ptr<vector<string>> CreateMessage() override;

	static bool IsBadStockSymbol(const string& strStockSymbol, const string& strExchangeCode);
	CFinnhubStocksPtr Parse(const string& text) const;
};

using CProductFinnhubCompanySymbolPtr = shared_ptr<CProductFinnhubStockSymbol>;
