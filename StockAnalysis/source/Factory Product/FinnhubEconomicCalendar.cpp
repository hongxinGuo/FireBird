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

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	((CWorldMarket*)m_pMarket)->ParseFinnhubEconomicCalendar(pWebData, vEconomicCalendar);
	((CWorldMarket*)m_pMarket)->UpdateEconomicCalendar(vEconomicCalendar);
	((CWorldMarket*)m_pMarket)->SetFinnhubEconomicCalendarUpdated(true);

	return true;
}