#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubForexSymbol final : public CProductFinnhub {
public:
	CProductFinnhubForexSymbol();
	~CProductFinnhubForexSymbol() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CForexSymbolsPtr ParseFinnhubForexSymbol(const CWebDataPtr& pWebData);
};

using CProductFinnhubForexSymbolPtr = shared_ptr<CProductFinnhubForexSymbol>;
