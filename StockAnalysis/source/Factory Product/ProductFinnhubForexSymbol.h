#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubForexSymbol : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexSymbol)
		CProductFinnhubForexSymbol();
	~CProductFinnhubForexSymbol() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CForexSymbolVectorPtr ParseFinnhubForexSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubForexSymbol> CProductFinnhubForexSymbolPtr;