#pragma once

#include"ProductFinnhub.h"
import FireBird.Stock.Finnhub;

class CProductFinnhubStockSymbol final : public CProductFinnhub {
public:
	CProductFinnhubStockSymbol();
	~CProductFinnhubStockSymbol() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	static bool IsNeedAddExchangeCode(const CString& strStockSymbol, const CString& strExchangeCode);
	CFinnhubStocksPtr ParseFinnhubStockSymbol(const CWebDataPtr& pWebData);
};

using CProductFinnhubCompanySymbolPtr = shared_ptr<CProductFinnhubStockSymbol>;
