#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubStockSymbol final : public CProductFinnhub {
public:
	CProductFinnhubStockSymbol();
	~CProductFinnhubStockSymbol() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	static bool IsBadStockSymbol(const string& strStockSymbol, const string& strExchangeCode);
	CFinnhubStocksPtr ParseFinnhubStockSymbol(const CWebDataPtr& pWebData);
};

using CProductFinnhubCompanySymbolPtr = shared_ptr<CProductFinnhubStockSymbol>;
