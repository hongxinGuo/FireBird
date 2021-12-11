#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubForexDayLine : public CWebSourceDataProduct {
public:
	CFinnhubForexDayLine();
	~CFinnhubForexDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubForexDayLine> CFinnhubForexDayLinePtr;
