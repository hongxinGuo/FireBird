#pragma once

#include"VirtualProductWebData.h"
#include"TiingoStock.h"

class CProductTinngoStockSymbol : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductTinngoStockSymbol)
		CProductTinngoStockSymbol();
	~CProductTinngoStockSymbol() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource = nullptr) override final;
	CTiingoStockVectorPtr ParseTiingoStockSymbol(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductTinngoStockSymbol> CTiingoStockSymbolsPtr;