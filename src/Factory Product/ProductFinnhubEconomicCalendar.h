#pragma once

#include"ProductFinnhub.h"
#include"EconomicCalendar.h"

class CProductFinnhubEconomicCalendar final : public CProductFinnhub {
public:
	CProductFinnhubEconomicCalendar();
	~CProductFinnhubEconomicCalendar() override = default;

	CString CreateMessage() final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CEconomicCalendarVectorPtr ParseFinnhubEconomicCalendar(CWebDataPtr pWebData);
};

using CFinnhubEconomicCalendarPtr = shared_ptr<CProductFinnhubEconomicCalendar>;
