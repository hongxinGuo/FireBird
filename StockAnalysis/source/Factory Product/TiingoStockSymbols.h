#pragma once

#include"WebSourceDataProduct.h"

class CTiingoStockSymbols : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CTiingoStockSymbols)
	CTiingoStockSymbols();
	~CTiingoStockSymbols() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CTiingoStockSymbols> CTiingoStockSymbolsPtr;