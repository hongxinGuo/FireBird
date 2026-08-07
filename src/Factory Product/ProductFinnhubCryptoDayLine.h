#pragma once

#include"ProductFinnhub.h"

class CDayLine;

class CProductFinnhubCryptoDayLine final : public CProductFinnhub {
public:
	CProductFinnhubCryptoDayLine();
	~CProductFinnhubCryptoDayLine() override = default;

	shared_ptr<vector<string>> CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CDayLine>> ParseFinnhubCryptoCandle(CWebDataPtr pWebData);
};

using CFinnhubCryptoDayLinePtr = shared_ptr<CProductFinnhubCryptoDayLine>;
