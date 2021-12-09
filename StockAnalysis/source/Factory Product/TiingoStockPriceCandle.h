#pragma once

#include"WebSourceDataProduct.h"

class CTiingoStockPriceCandle : public CWebSourceDataProduct {
public:
	CTiingoStockPriceCandle();
	~CTiingoStockPriceCandle() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CTiingoStockPriceCandle> CTiingoStockPriceCandlePtr;