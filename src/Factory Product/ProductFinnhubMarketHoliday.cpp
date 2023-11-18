#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include "ProductFinnhubMarketHoliday.h"

CProductFinnhubMarketHoliday::CProductFinnhubMarketHoliday() {
	m_strClassName = _T("Finnhub market holiday");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/market-holiday?exchange=");
	m_lIndex = -1;
}

CString CProductFinnhubMarketHoliday::CreateMessage() {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto strParam = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStockExchangeCode(m_lIndex);

	m_strInquiringExchange = strParam;
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

bool CProductFinnhubMarketHoliday::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto pvHoliday = ParseFinnhubMarketHoliday(pWebData);
	const auto pExchange = gl_pWorldMarket->GetStockExchange(m_lIndex);
	pExchange->SetMarketHolidayUpdated(true);

	gl_pWorldMarket->UpdateMarketHoliday(pvHoliday);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ÍøÖ·£ºhttps://finnhub.io/docs/api/market-holiday
//
// {
//	"data": [
//		{
//			"eventName": "Christmas",
//			"atDate": "2023-12-25",
//			"tradingHour" : ""
//		},
//		{
//			"eventName": "Independence Day",
//			"atDate": "2023-07-04",
//			"tradingHour" : "09:30-13:00"
//		}
//	]
//	"exchange": "US",
//	"timezone": "America/New_York"
// }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
CMarketHolidayVectorPtr CProductFinnhubMarketHoliday::ParseFinnhubMarketHoliday(const CWebDataPtr& pWebData) {
	auto pvHoliday = make_shared<vector<CMarketHolidayPtr>>();
	CMarketHolidayPtr pHoliday = nullptr;
	string s, sError;
	long year, month, day;
	CString sExchange, sTimeZone;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvHoliday;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return pvHoliday;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return pvHoliday;
	}
	const auto pjs = pWebData->GetJSon();
	s = jsonGetString(pjs, _T("exchange"));
	if (!s.empty()) sExchange = s.c_str();
	s = jsonGetString(pjs, _T("timezone"));
	if (!s.c_str()) sTimeZone = s.c_str();
	auto js1 = jsonGetChild(pjs, (_T("data")));
	try {
		for (auto it = js1.begin(); it != js1.end(); ++it) {
			pHoliday = make_shared<CMarketHoliday>();
			s = jsonGetString(it, _T("eventName"));
			if (!s.empty()) pHoliday->m_strEventName = s.c_str();
			s = jsonGetString(it, _T("atDate"));
			sscanf_s(s.c_str(), _T("%4d-%02d-%02d"), &year, &month, &day);
			pHoliday->m_lDate = XferYearMonthDayToYYYYMMDD(year, month, day);
			s = jsonGetString(it, _T("tradingHour"));
			pHoliday->m_strTradingHour = s.c_str();
			pHoliday->m_strExchange = sExchange;
			pHoliday->m_strTimeZone = sTimeZone;
			pvHoliday->push_back(pHoliday);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub market holiday "), e.what());
		return pvHoliday;
	}
	return pvHoliday;
}
