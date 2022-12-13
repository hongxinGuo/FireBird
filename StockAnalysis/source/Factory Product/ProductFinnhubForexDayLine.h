#pragma once

#include"WebData.h"
#include"ProductFinnhub.h"

class CProductFinnhubForexDayLine final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexDayLine)
	CProductFinnhubForexDayLine();
	~CProductFinnhubForexDayLine() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseFinnhubForexCandle(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubForexDayLine> CFinnhubForexDayLinePtr;
