#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubForexDayLine : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubForexDayLine)
	CFinnhubForexDayLine();
	~CFinnhubForexDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseFinnhubForexCandle(CWebDataPtr pWebData);
};

typedef shared_ptr<CFinnhubForexDayLine> CFinnhubForexDayLinePtr;
