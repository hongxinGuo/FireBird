#pragma once

#include"WebData.h"
#include"ProductFinnhub.h"

class CProductFinnhubStockDayLine final : public CProductFinnhub {
public:
	CProductFinnhubStockDayLine();
	~CProductFinnhubStockDayLine() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CDayLineVectorPtr ParseFinnhubStockCandle(CWebDataPtr pWebData);
};

using CFinnhubStockDayLinePtr = shared_ptr<CProductFinnhubStockDayLine>;
