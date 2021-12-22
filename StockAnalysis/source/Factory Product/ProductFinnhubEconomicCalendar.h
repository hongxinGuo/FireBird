#pragma once

#include"ProductWebData.h"

class CProductFinnhubEconomicCalendar : public CProductWebSourceData {
public:
	DECLARE_DYNCREATE(CProductFinnhubEconomicCalendar)
	CProductFinnhubEconomicCalendar();
	~CProductFinnhubEconomicCalendar() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
	CEconomicCalendarVectorPtr ParseFinnhubEconomicCalendar(CWebDataPtr pWebData);
};

typedef shared_ptr<CProductFinnhubEconomicCalendar> CFinnhubEconomicCalendarPtr;