#pragma once

#include"ProductFinnhub.h"

class CWebData;
class CEconomicCalendar;

class CProductFinnhubEconomicCalendar final : public CProductFinnhub {
public:
	CProductFinnhubEconomicCalendar();
	~CProductFinnhubEconomicCalendar() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(shared_ptr<CWebData> pWebData) override;
	shared_ptr<vector<CEconomicCalendar>> ParseFinnhubEconomicCalendar(const shared_ptr<CWebData>& pWebData);

	void UpdateSystemStatus() override;
};

using CFinnhubEconomicCalendarPtr = shared_ptr<CProductFinnhubEconomicCalendar>;
