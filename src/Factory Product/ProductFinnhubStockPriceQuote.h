#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubStockPriceQuote final : public CProductFinnhub {
public:
	CProductFinnhubStockPriceQuote();
	~CProductFinnhubStockPriceQuote() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	bool ParseFinnhubStockQuote(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

using CFinnhubStockPriceQuotePtr = shared_ptr<CProductFinnhubStockPriceQuote>;
