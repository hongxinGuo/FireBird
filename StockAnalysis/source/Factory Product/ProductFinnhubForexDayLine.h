#pragma once

#include"ProductWebData.h"

class CProductFinnhubForexDayLine : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexDayLine)
	CProductFinnhubForexDayLine();
	~CProductFinnhubForexDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseFinnhubForexCandle(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubForexDayLine> CFinnhubForexDayLinePtr;
