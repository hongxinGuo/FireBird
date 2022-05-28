#pragma once

#include"ProductWebData.h"

class CProductFinnhubStockPriceQuote : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockPriceQuote)
	CProductFinnhubStockPriceQuote();
	~CProductFinnhubStockPriceQuote() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	bool ParseFinnhubStockQuote(CWebDataPtr pWebData, CWorldStockPtr pStock);
};

typedef shared_ptr<CProductFinnhubStockPriceQuote> CFinnhubStockPriceQuotePtr;