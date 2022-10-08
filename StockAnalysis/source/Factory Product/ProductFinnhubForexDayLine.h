#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubForexDayLine : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexDayLine)
		CProductFinnhubForexDayLine();
	~CProductFinnhubForexDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseFinnhubForexCandle(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubForexDayLine> CFinnhubForexDayLinePtr;
