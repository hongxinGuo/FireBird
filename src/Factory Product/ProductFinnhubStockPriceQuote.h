#pragma once

#include"ProductFinnhub.h"

class CFinnhubStock;

class CProductFinnhubStockPriceQuote final : public CProductFinnhub {
public:
	CProductFinnhubStockPriceQuote();
	~CProductFinnhubStockPriceQuote() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	bool ParseFinnhubStockQuote(const CWebDataPtr& pWebData, const shared_ptr<CFinnhubStock>& pStock);
};

using CFinnhubStockPriceQuotePtr = shared_ptr<CProductFinnhubStockPriceQuote>;
