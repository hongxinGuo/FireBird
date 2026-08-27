#pragma once

#include "DayLine.h"
#include"ProductFinnhub.h"

class CDayLine;

class CProductFinnhubCryptoDayLine final : public CProductFinnhub {
public:
	CProductFinnhubCryptoDayLine();
	~CProductFinnhubCryptoDayLine() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CDayLinesPtr Parse(const string& text);
};

using CFinnhubCryptoDayLinePtr = shared_ptr<CProductFinnhubCryptoDayLine>;
