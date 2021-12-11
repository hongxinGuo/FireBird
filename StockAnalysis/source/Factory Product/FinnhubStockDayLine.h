#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubStockDayLine : public CWebSourceDataProduct {
public:
	CFinnhubStockDayLine();
	~CFinnhubStockDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubStockDayLine> CFinnhubStockDayLinePtr;