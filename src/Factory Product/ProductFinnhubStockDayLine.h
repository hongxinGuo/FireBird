#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubStockDayLine final : public CProductFinnhub {
public:
	CProductFinnhubStockDayLine();
	~CProductFinnhubStockDayLine() override = default;

	CString CreateMessage(void);
	bool ParseAndStoreWebData(CWebDataPtr pWebData);
	CDayLineVectorPtr ParseFinnhubStockCandle(CWebDataPtr pWebData);
};

using CFinnhubStockDayLinePtr = shared_ptr<CProductFinnhubStockDayLine>;
