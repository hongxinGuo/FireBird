#pragma once

#include"ProductWebData.h"
#include"DayLine.h"

class CProductTiingoStockDayLine : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductTiingoStockDayLine)
	CProductTiingoStockDayLine();
	~CProductTiingoStockDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseTiingoStockDayLine(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductTiingoStockDayLine> CTiingoStockDayLinePtr;