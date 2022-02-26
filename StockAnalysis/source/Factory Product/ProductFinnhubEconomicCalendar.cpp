#include "pch.h"

#include"globedef.h"
#include"accessory.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubEconomicCalendar.h"

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

IMPLEMENT_DYNCREATE(CProductFinnhubEconomicCalendar, CProductWebSourceData)

CProductFinnhubEconomicCalendar::CProductFinnhubEconomicCalendar() {
	m_strClassName = _T("Finnhub economic calendar");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/calendar/economic?");
	m_lIndex = -1;
}

CString CProductFinnhubEconomicCalendar::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CProductFinnhubEconomicCalendar::ProcessWebData(CWebDataPtr pWebData) {
	CEconomicCalendarVectorPtr pvEconomicCalendar;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pvEconomicCalendar = ParseFinnhubEconomicCalendar(pWebData);
	((CWorldMarket*)m_pMarket)->UpdateEconomicCalendar(*pvEconomicCalendar);
	((CWorldMarket*)m_pMarket)->SetFinnhubEconomicCalendarUpdated(true);

	return true;
}

CEconomicCalendarVectorPtr CProductFinnhubEconomicCalendar::ParseFinnhubEconomicCalendar(CWebDataPtr pWebData) {
	CEconomicCalendarVectorPtr pvEconomicCalendar = make_shared<vector<CEconomicCalendarPtr>>();
	CEconomicCalendarPtr pEconomicCalendar = nullptr;
	ptree pt1, pt2;
	string s;
	shared_ptr<ptree> ppt;

	if (pWebData->IsJSonContentType() && pWebData->IsSucceedCreatePTree()) {
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
				pEconomicCalendar->m_dEstimate = pt2.get<double>(_T("estimate"));
				pEconomicCalendar->m_dActual = pt2.get<double>(_T("actual"));
				pEconomicCalendar->m_dPrev = pt2.get<double>(_T("prev"));
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
	}
	return pvEconomicCalendar;
}