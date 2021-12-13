#pragma once

#include"WebSourceDataProduct.h"

class CFinnhubEconomicCalendar : public CWebSourceDataProduct {
public:
	DECLARE_DYNCREATE(CFinnhubEconomicCalendar)
	CFinnhubEconomicCalendar();
	~CFinnhubEconomicCalendar() {}

	virtual CString CreatMessage(void) override final;
	virtual bool ProcessWebData(CWebDataPtr pWebData) override final;
};

typedef shared_ptr<CFinnhubEconomicCalendar> CFinnhubEconomicCalendarPtr;