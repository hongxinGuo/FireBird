#pragma once

#include"WebData.h"
#include"DayLine.h"

#include"ProductFinnhub.h"

class CProductFinnhubCryptoDayLine final : public CProductFinnhub {
public:
	CProductFinnhubCryptoDayLine();
	~CProductFinnhubCryptoDayLine() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CDayLineVectorPtr ParseFinnhubCryptoCandle(CWebDataPtr pWebData);
};

using CFinnhubCryptoDayLinePtr = shared_ptr<CProductFinnhubCryptoDayLine>;