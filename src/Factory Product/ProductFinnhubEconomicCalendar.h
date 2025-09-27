#pragma once

#include"ProductFinnhub.h"
#include"EconomicCalendar.h"

class CProductFinnhubEconomicCalendar final : public CProductFinnhub {
public:
	CProductFinnhubEconomicCalendar();
	~CProductFinnhubEconomicCalendar() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CEconomicCalendarsPtr ParseFinnhubEconomicCalendar(const CWebDataPtr& pWebData);

	void UpdateSystemStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CFinnhubEconomicCalendarPtr = shared_ptr<CProductFinnhubEconomicCalendar>;
