#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubStockPriceQuote final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockPriceQuote)
	CProductFinnhubStockPriceQuote();
	~CProductFinnhubStockPriceQuote() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	bool ParseFinnhubStockQuote(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

using CFinnhubStockPriceQuotePtr = shared_ptr<CProductFinnhubStockPriceQuote>;
