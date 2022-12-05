#pragma once

#include"VirtualProductWebData.h"
#include"DayLine.h"

class CProductTiingoStockDayLine : public CVirtualProductWebData {
public:
	DECLARE_DYNCREATE(CProductTiingoStockDayLine)
		CProductTiingoStockDayLine();
	~CProductTiingoStockDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseTiingoStockDayLine(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductTiingoStockDayLine> CTiingoStockDayLinePtr;