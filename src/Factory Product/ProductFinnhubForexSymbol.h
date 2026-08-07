#pragma once

#include"ProductFinnhub.h"

class CFinnhubForex;

class CProductFinnhubForexSymbol final : public CProductFinnhub {
public:
	CProductFinnhubForexSymbol();
	~CProductFinnhubForexSymbol() override = default;

	shared_ptr<vector<string>> CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<shared_ptr<CFinnhubForex>>> ParseFinnhubForexSymbol(const CWebDataPtr& pWebData);
};

using CProductFinnhubForexSymbolPtr = shared_ptr<CProductFinnhubForexSymbol>;
