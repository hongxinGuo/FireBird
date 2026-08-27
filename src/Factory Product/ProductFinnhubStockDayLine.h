#pragma once

#include"ProductFinnhub.h"

#include"DayLine.h"

class CProductFinnhubStockDayLine final : public CProductFinnhub {
public:
	CProductFinnhubStockDayLine();
	~CProductFinnhubStockDayLine() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CDayLinesPtr Parse(const string& text);
};

using CFinnhubStockDayLinePtr = shared_ptr<CProductFinnhubStockDayLine>;
