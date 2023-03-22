#pragma once

#include"WorldMarket.h"
#include"ProductFinnhub.h"

class CProductFinnhubForexSymbol final : public CProductFinnhub {
public:
	CProductFinnhubForexSymbol();
	~CProductFinnhubForexSymbol() override = default;

	CString CreateMessage() final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CForexSymbolVectorPtr ParseFinnhubForexSymbol(CWebDataPtr pWebData);
};

using CProductFinnhubForexSymbolPtr = shared_ptr<CProductFinnhubForexSymbol>;
