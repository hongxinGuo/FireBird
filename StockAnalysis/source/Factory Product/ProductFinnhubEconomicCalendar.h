#pragma once

#include"ProductFinnhub.h"
#include"EconomicCalendar.h"

class CProductFinnhubEconomicCalendar final : public CProductFinnhub {
public:
	DECLARE_DYNCREATE(CProductFinnhubEconomicCalendar)
	CProductFinnhubEconomicCalendar();
	~CProductFinnhubEconomicCalendar() override = default;

	virtual CString CreateMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CEconomicCalendarVectorPtr ParseFinnhubEconomicCalendar(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubEconomicCalendar> CFinnhubEconomicCalendarPtr;
