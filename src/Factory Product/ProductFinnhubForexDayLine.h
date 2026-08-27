#pragma once

#include"ProductFinnhub.h"

class CDayLine;

class CProductFinnhubForexDayLine final : public CProductFinnhub {
public:
	CProductFinnhubForexDayLine();
	~CProductFinnhubForexDayLine() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CDayLinesPtr Parse(const string& text);
};

using CFinnhubForexDayLinePtr = shared_ptr<CProductFinnhubForexDayLine>;
