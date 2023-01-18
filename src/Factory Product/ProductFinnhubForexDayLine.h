#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubForexDayLine final : public CProductFinnhub {
public:
	CProductFinnhubForexDayLine();
	~CProductFinnhubForexDayLine() override = default;

	CString CreateMessage(void);
	bool ParseAndStoreWebData(CWebDataPtr pWebData);
	CDayLineVectorPtr ParseFinnhubForexCandle(CWebDataPtr pWebData);
};

using CFinnhubForexDayLinePtr = shared_ptr<CProductFinnhubForexDayLine>;
