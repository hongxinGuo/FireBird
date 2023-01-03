#pragma once

#include"WebData.h"
#include"ProductFinnhub.h"

class CProductFinnhubForexDayLine final : public CProductFinnhub {
public:
	CProductFinnhubForexDayLine();
	~CProductFinnhubForexDayLine() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CDayLineVectorPtr ParseFinnhubForexCandle(CWebDataPtr pWebData);
};

using CFinnhubForexDayLinePtr = shared_ptr<CProductFinnhubForexDayLine>;
