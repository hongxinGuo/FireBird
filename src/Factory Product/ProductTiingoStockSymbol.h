#pragma once

#include"VirtualWebProduct.h"
#include"TiingoStock.h"

class CProductTiingoStockSymbol final : public CVirtualWebProduct {
public:
	CProductTiingoStockSymbol();
	~CProductTiingoStockSymbol() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CTiingoStockVectorPtr ParseTiingoStockSymbol(CWebDataPtr pWebData);
};

using CTiingoStockSymbolsPtr = shared_ptr<CProductTiingoStockSymbol>;