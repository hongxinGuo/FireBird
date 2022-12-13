#pragma once

#include"VirtualWebProduct.h"
#include"TiingoStock.h"

class CProductTinngoStockSymbol : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductTinngoStockSymbol)
		CProductTinngoStockSymbol();
	~CProductTinngoStockSymbol() {}

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CTiingoStockVectorPtr ParseTiingoStockSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductTinngoStockSymbol> CTiingoStockSymbolsPtr;