#pragma once

#include"WebData.h"
#include"ProductFinnhub.h"

class CProductFinnhubForexDayLine final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubForexDayLine)
	CProductFinnhubForexDayLine();
	~CProductFinnhubForexDayLine() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CDayLineVectorPtr ParseFinnhubForexCandle(CWebDataPtr pWebData);
	CDayLineVectorPtr ParseFinnhubForexCandle2(CWebDataPtr pWebData);
};

using CFinnhubForexDayLinePtr = shared_ptr<CProductFinnhubForexDayLine>;
