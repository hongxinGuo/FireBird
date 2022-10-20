#include "pch.h"

#include"jsonParse.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubEconomicCalendar.h"

IMPLEMENT_DYNCREATE(CProductFinnhubEconomicCalendar, CProductFinnhub)

CProductFinnhubEconomicCalendar::CProductFinnhubEconomicCalendar() {
	m_strClassName = _T("Finnhub economic calendar");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/calendar/economic?");
	m_lIndex = -1;
}

CString CProductFinnhubEconomicCalendar::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CProductFinnhubEconomicCalendar::ParseAndStoreWebData(CWebDataPtr pWebData) {
	CEconomicCalendarVectorPtr pvEconomicCalendar;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pvEconomicCalendar = ParseFinnhubEconomicCalendar(pWebData);
	((CWorldMarket*)m_pMarket)->UpdateEconomicCalendar(*pvEconomicCalendar);

	return true;
}

CEconomicCalendarVectorPtr CProductFinnhubEconomicCalendar::ParseFinnhubEconomicCalendar(CWebDataPtr pWebData) {
	CEconomicCalendarVectorPtr pvEconomicCalendar = make_shared<vector<CEconomicCalendarPtr>>();
	CEconomicCalendarPtr pEconomicCalendar = nullptr;
	ptree pt1, pt2;
	string s;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvEconomicCalendar;
	if (pWebData->IsVoidJSon()) { m_iReceivedDataStatus = __VOID_DATA__; return pvEconomicCalendar; }
	if (pWebData->NoRightToAccess()) { m_iReceivedDataStatus = __NO_ACCESS_RIGHT__; return pvEconomicCalendar; }
	ppt = pWebData->GetPTree();
	try {
		pt1 = ppt->get_child(_T("economicCalendar"));
		for (ptree::iterator it = pt1.begin(); it != pt1.end(); ++it) {
			pEconomicCalendar = make_shared<CEconomicCalendar>();
			pt2 = it->second;
			s = pt2.get<string>(_T("country"));
			if (s.size() > 0) pEconomicCalendar->m_strCountry = s.c_str();
			s = pt2.get<string>(_T("event"));
			pEconomicCalendar->m_strEvent = s.c_str();
			s = pt2.get<string>(_T("impact"));
			pEconomicCalendar->m_strImpact = s.c_str();
			pEconomicCalendar->m_dEstimate = ptreeGetDouble(pt2, _T("estimate"));
			pEconomicCalendar->m_dActual = ptreeGetDouble(pt2, _T("actual"));
			pEconomicCalendar->m_dPrev = ptreeGetDouble(pt2, _T("prev"));
			s = pt2.get<string>(_T("time"));
			pEconomicCalendar->m_strTime = s.c_str();
			s = pt2.get<string>(_T("unit"));
			pEconomicCalendar->m_strUnit = s.c_str();
			pvEconomicCalendar->push_back(pEconomicCalendar);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Economic Calendar "), e);
	}
	return pvEconomicCalendar;
}