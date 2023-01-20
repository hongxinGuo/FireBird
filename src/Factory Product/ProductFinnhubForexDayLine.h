#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubForexDayLine final : public CProductFinnhub {
public:
	CProductFinnhubForexDayLine();
	~CProductFinnhubForexDayLine() override = default;

	CString CreateMessage(void) override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLineVectorPtr ParseFinnhubForexCandle(CWebDataPtr pWebData);
};

using CFinnhubForexDayLinePtr = shared_ptr<CProductFinnhubForexDayLine>;
