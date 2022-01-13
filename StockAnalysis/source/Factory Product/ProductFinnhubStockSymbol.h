#pragma once

#include"ProductWebData.h"

class CProductFinnhubStockSymbol : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockSymbol)
	CProductFinnhubStockSymbol();
	~CProductFinnhubStockSymbol() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	bool IsNeedAddExchangeCode(CString strStockSymbol, CString strExchangeCode);
	CWorldStockVectorPtr ParseFinnhubStockSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubStockSymbol> CProductFinnhubCompanySymbolPtr;