#pragma once

#include"ProductWebData.h"
#include"TiingoStock.h"

class CProductTinngoStockSymbol : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductTinngoStockSymbol)
	CProductTinngoStockSymbol();
	~CProductTinngoStockSymbol() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CTiingoStockVectorPtr ParseTiingoStockSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductTinngoStockSymbol> CTiingoStockSymbolsPtr;