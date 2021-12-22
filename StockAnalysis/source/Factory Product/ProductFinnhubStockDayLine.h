#pragma once

#include"ProductWebData.h"

class CProductFinnhubStockDayLine : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockDayLine)
	CProductFinnhubStockDayLine();
	~CProductFinnhubStockDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseFinnhubStockCandle(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubStockDayLine> CFinnhubStockDayLinePtr;