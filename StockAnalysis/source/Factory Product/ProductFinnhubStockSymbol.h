#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubStockSymbol : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockSymbol)
		CProductFinnhubStockSymbol();
	~CProductFinnhubStockSymbol() {}

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	bool IsNeedAddExchangeCode(CString strStockSymbol, CString strExchangeCode);
	CWorldStockVectorPtr ParseFinnhubStockSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubStockSymbol> CProductFinnhubCompanySymbolPtr;