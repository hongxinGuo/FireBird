#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubStockDayLine : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubStockDayLine)
	CFinnhubStockDayLine();
	~CFinnhubStockDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseFinnhubStockCandle(CWebDataPtr pWebData);
};

typedef shared_ptr<CFinnhubStockDayLine> CFinnhubStockDayLinePtr;