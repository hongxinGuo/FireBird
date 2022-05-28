#pragma once

#include"ProductWebData.h"

class CProductFinnhubCryptoDayLine : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubCryptoDayLine)
	CProductFinnhubCryptoDayLine();
	~CProductFinnhubCryptoDayLine() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CDayLineVectorPtr ParseFinnhubCryptoCandle(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubCryptoDayLine> CFinnhubCryptoDayLinePtr;
