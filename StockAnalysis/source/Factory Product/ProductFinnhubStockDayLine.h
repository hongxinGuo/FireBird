#pragma once

#include"ProductFinnhub.h"

class CProductFinnhubStockDayLine : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubStockDayLine)
		CProductFinnhubStockDayLine();
	~CProductFinnhubStockDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource = nullptr) override final;
	CDayLineVectorPtr ParseFinnhubStockCandle(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubStockDayLine> CFinnhubStockDayLinePtr;