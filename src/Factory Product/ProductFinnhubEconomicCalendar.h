#pragma once

#include "EconomicCalendar.h"
#include"ProductFinnhub.h"

class CWebData;
class CEconomicCalendar;

class CProductFinnhubEconomicCalendar final : public CProductFinnhub {
public:
	CProductFinnhubEconomicCalendar();
	~CProductFinnhubEconomicCalendar() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CEconomicCalendarsPtr Parse(const string& text);

};

using CFinnhubEconomicCalendarPtr = shared_ptr<CProductFinnhubEconomicCalendar>;
