#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubStockDayLine final : public CProductFinnhub {
public:
	CProductFinnhubStockDayLine();
	~CProductFinnhubStockDayLine() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLineVectorPtr ParseFinnhubStockCandle(CWebDataPtr pWebData);
};

using CFinnhubStockDayLinePtr = shared_ptr<CProductFinnhubStockDayLine>;
