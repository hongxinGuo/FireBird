#include "pch.h"

#include"jsonParse.h"
#include"WorldMarket.h"

#include "ProductFinnhubEconomicCalendar.h"

IMPLEMENT_DYNCREATE(CProductFinnhubEconomicCalendar, CProductFinnhub)

CProductFinnhubEconomicCalendar::CProductFinnhubEconomicCalendar() {
	m_strClassName = _T("Finnhub economic calendar");
	m_strInquiry = _T("https://finnhub.io/api/v1/calendar/economic?");
	m_lIndex = -1;
}

CString CProductFinnhubEconomicCalendar::CreateMessage(void) {
	m_strTotalInquiryMessage = m_strInquiry;
	ASSERT(m_strInquiringExchange.Compare(_T("ALL")) == 0);
	m_strInquiringExchange = _T("ALL"); // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubEconomicCalendar::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pvEconomicCalendar = ParseFinnhubEconomicCalendar(pWebData);
	dynamic_cast<CWorldMarket*>(m_pMarket)->UpdateEconomicCalendar(*pvEconomicCalendar);

	return true;
}

CEconomicCalendarVectorPtr CProductFinnhubEconomicCalendar::ParseFinnhubEconomicCalendar(CWebDataPtr pWebData) {
	auto pvEconomicCalendar = make_shared<vector<CEconomicCalendarPtr>>();
	CEconomicCalendarPtr pEconomicCalendar = nullptr;
	string s;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvEconomicCalendar;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvEconomicCalendar;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvEconomicCalendar;
	}
	const auto ppt = pWebData->GetPTree();
	try {
		ptree pt1 = ppt->get_child(_T("economicCalendar"));
		for (ptree::iterator it = pt1.begin(); it != pt1.end(); ++it) {
			pEconomicCalendar = make_shared<CEconomicCalendar>();
			ptree pt2 = it->second;
			s = pt2.get<string>(_T("country"));
			if (!s.empty()) pEconomicCalendar->m_strCountry = s.c_str();
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
