#pragma once

#include"DayLine.h"

#include"ProductFinnhub.h"

class CProductFinnhubCryptoDayLine final : public CProductFinnhub {
public:
	CProductFinnhubCryptoDayLine();
	~CProductFinnhubCryptoDayLine() override = default;

	CString CreateMessage(void);
	bool ParseAndStoreWebData(CWebDataPtr pWebData);
	CDayLineVectorPtr ParseFinnhubCryptoCandle(CWebDataPtr pWebData);
};

using CFinnhubCryptoDayLinePtr = shared_ptr<CProductFinnhubCryptoDayLine>;
