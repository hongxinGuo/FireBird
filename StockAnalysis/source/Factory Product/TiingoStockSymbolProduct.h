#pragma once

#include"WebSourceDataProduct.h"

class CTinngoStockSymbolProduct : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CTinngoStockSymbolProduct)
	CTinngoStockSymbolProduct();
	~CTinngoStockSymbolProduct() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CTiingoStockVectorPtr ParseTiingoStockSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CTinngoStockSymbolProduct> CTiingoStockSymbolsPtr;