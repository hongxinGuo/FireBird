#pragma once

#include"WebData.h"
#include"ProductFinnhub.h"

class CProductFinnhubStockDayLine final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockDayLine)
	CProductFinnhubStockDayLine();
	~CProductFinnhubStockDayLine() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseFinnhubStockCandle(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubStockDayLine> CFinnhubStockDayLinePtr;
