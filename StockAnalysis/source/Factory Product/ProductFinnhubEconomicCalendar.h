#pragma once

#include"ProductWebData.h"
#include"EconomicCalendar.h"

class CProductFinnhubEconomicCalendar : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubEconomicCalendar)
	CProductFinnhubEconomicCalendar();
	~CProductFinnhubEconomicCalendar() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) override final;
	CEconomicCalendarVectorPtr ParseFinnhubEconomicCalendar(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubEconomicCalendar> CFinnhubEconomicCalendarPtr;