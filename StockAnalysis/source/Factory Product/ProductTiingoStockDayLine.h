#pragma once

#include"VirtualWebProduct.h"
#include"DayLine.h"

class CProductTiingoStockDayLine : public CVirtualWebProduct {
public:
	DECLARE_DYNCREATE(CProductTiingoStockDayLine)
		CProductTiingoStockDayLine();
	~CProductTiingoStockDayLine() {}

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseTiingoStockDayLine(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductTiingoStockDayLine> CTiingoStockDayLinePtr;