#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubStockPriceQuote : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockPriceQuote)
		CProductFinnhubStockPriceQuote();
	~CProductFinnhubStockPriceQuote() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	bool ParseFinnhubStockQuote(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

typedef shared_ptr<CProductFinnhubStockPriceQuote> CFinnhubStockPriceQuotePtr;