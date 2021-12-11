#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCryptoDayLine : public CWebSourceDataProduct {
public:
	CFinnhubCryptoDayLine();
	~CFinnhubCryptoDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubCryptoDayLine> CFinnhubCryptoDayLinePtr;
