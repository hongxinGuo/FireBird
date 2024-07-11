#pragma once

#include"DayLine.h"

#include"ProductFinnhub.h"

class CProductFinnhubCryptoDayLine final : public CProductFinnhub {
public:
	CProductFinnhubCryptoDayLine();
	~CProductFinnhubCryptoDayLine() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CDayLinesPtr ParseFinnhubCryptoCandle(CWebDataPtr pWebData);
};

using CFinnhubCryptoDayLinePtr = shared_ptr<CProductFinnhubCryptoDayLine>;
