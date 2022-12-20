#pragma once

#include"WebData.h"
#include"DayLine.h"

#include"ProductFinnhub.h"

class CProductFinnhubCryptoDayLine final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubCryptoDayLine)
	CProductFinnhubCryptoDayLine();
	~CProductFinnhubCryptoDayLine() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CDayLineVectorPtr ParseFinnhubCryptoCandle(CWebDataPtr pWebData);
	CDayLineVectorPtr ParseFinnhubCryptoCandle2(CWebDataPtr pWebData);
};

using CFinnhubCryptoDayLinePtr = shared_ptr<CProductFinnhubCryptoDayLine>;
