#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubStockSymbol final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockSymbol)
	CProductFinnhubStockSymbol();
	~CProductFinnhubStockSymbol() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	bool IsNeedAddExchangeCode(CString strStockSymbol, CString strExchangeCode);
	CWorldStockVectorPtr ParseFinnhubStockSymbol(CWebDataPtr pWebData);
};

using CProductFinnhubCompanySymbolPtr = shared_ptr<CProductFinnhubStockSymbol>;
