#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubStockPriceQuote final : public CProductFinnhub {
public:
	CProductFinnhubStockPriceQuote();
	~CProductFinnhubStockPriceQuote() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	bool ParseFinnhubStockQuote(const CWebDataPtr& pWebData, const CFinnhubStockPtr& pStock);
};

using CFinnhubStockPriceQuotePtr = shared_ptr<CProductFinnhubStockPriceQuote>;
