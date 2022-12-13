#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubForexSymbol final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexSymbol)
	CProductFinnhubForexSymbol();
	~CProductFinnhubForexSymbol() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CForexSymbolVectorPtr ParseFinnhubForexSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubForexSymbol> CProductFinnhubForexSymbolPtr;
