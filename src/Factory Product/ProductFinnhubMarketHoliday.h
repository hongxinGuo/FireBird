#pragma once

#include "MarketHoliday.h"
#include"ProductFinnhub.h"

class CProductFinnhubMarketHoliday final : public CProductFinnhub {
public:
	CProductFinnhubMarketHoliday();
	~CProductFinnhubMarketHoliday() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CMarketHolidaysPtr Parse(const string& text);
};

using CProductFinnhubMarketHolidayPtr = shared_ptr<CProductFinnhubMarketHoliday>;
