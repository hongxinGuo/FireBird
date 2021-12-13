#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubStockPriceQuote : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubStockPriceQuote)
	CFinnhubStockPriceQuote();
	~CFinnhubStockPriceQuote() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubStockPriceQuote> CFinnhubStockPriceQuotePtr;