#pragma once

#include"WebSourceDataProduct.h"

class CTiingoStockDayLine : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CTiingoStockDayLine)
	CTiingoStockDayLine();
	~CTiingoStockDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseTiingoStockDayLine(CWebDataPtr pWebData);
};

typedef shared_ptr<CTiingoStockDayLine> CTiingoStockDayLinePtr;