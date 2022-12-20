#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubForexSymbol final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexSymbol)
	CProductFinnhubForexSymbol();
	~CProductFinnhubForexSymbol() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CForexSymbolVectorPtr ParseFinnhubForexSymbol(CWebDataPtr pWebData);
	CForexSymbolVectorPtr ParseFinnhubForexSymbol2(CWebDataPtr pWebData);
};

using CProductFinnhubForexSymbolPtr = shared_ptr<CProductFinnhubForexSymbol>;
