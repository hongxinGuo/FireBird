#pragma once

#include"VirtualWebProduct.h"
#include"TiingoStock.h"

class CProductTiingoStockSymbol final : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductTiingoStockSymbol)
	CProductTiingoStockSymbol();
	~CProductTiingoStockSymbol() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CTiingoStockVectorPtr ParseTiingoStockSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductTiingoStockSymbol> CTiingoStockSymbolsPtr;
