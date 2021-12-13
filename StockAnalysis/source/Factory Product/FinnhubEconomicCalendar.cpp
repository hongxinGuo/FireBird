#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "FinnhubEconomicCalendar.h"

IMPLEMENT_DYNCREATE(CFinnhubEconomicCalendar, CWebSourceDataProduct)

CFinnhubEconomicCalendar::CFinnhubEconomicCalendar() {
	m_strClassName = _T("Finnhub economic calendar");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/calendar/economic?");
	m_lIndex = -1;
}

CString CFinnhubEconomicCalendar::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CFinnhubEconomicCalendar::ProcessWebData(CWebDataPtr pWebData) {
	vector<CEconomicCalendarPtr> vEconomicCalendar;
	gl_pWorldMarket->ParseFinnhubEconomicCalendar(pWebData, vEconomicCalendar);
	gl_pWorldMarket->UpdateEconomicCalendar(vEconomicCalendar);
	gl_pWorldMarket->SetFinnhubEconomicCalendarUpdated(true);

	return true;
}