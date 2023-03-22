#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubStockSymbol final : public CProductFinnhub {
public:
	CProductFinnhubStockSymbol();
	~CProductFinnhubStockSymbol() override = default;

	CString CreateMessage() final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	bool IsNeedAddExchangeCode(CString strStockSymbol, CString strExchangeCode);
	CWorldStockVectorPtr ParseFinnhubStockSymbol(CWebDataPtr pWebData);
};

using CProductFinnhubCompanySymbolPtr = shared_ptr<CProductFinnhubStockSymbol>;
