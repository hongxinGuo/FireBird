#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubForexDayLine final : public CProductFinnhub {
public:
	CProductFinnhubForexDayLine();
	~CProductFinnhubForexDayLine() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLinesPtr ParseFinnhubForexCandle(CWebDataPtr pWebData);
};

using CFinnhubForexDayLinePtr = shared_ptr<CProductFinnhubForexDayLine>;
