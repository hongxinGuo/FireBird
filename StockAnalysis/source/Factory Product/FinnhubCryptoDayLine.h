#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubCryptoDayLine : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubCryptoDayLine)
	CFinnhubCryptoDayLine();
	~CFinnhubCryptoDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseFinnhubCryptoCandle(CWebDataPtr pWebData);
};

typedef shared_ptr<CFinnhubCryptoDayLine> CFinnhubCryptoDayLinePtr;
