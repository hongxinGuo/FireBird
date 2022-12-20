#pragma once

#include"ProductFinnhub.h"
#include"EconomicCalendar.h"

class CProductFinnhubEconomicCalendar final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubEconomicCalendar)
	CProductFinnhubEconomicCalendar();
	~CProductFinnhubEconomicCalendar() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) final;
	CEconomicCalendarVectorPtr ParseFinnhubEconomicCalendar(CWebDataPtr pWebData);
};

using CFinnhubEconomicCalendarPtr = shared_ptr<CProductFinnhubEconomicCalendar>;
