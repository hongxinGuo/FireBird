#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubMarketHoliday.h"

#include "TimeConvert.h"
#include "WebData.h"
#include "WorldMarket.h"

CProductFinnhubMarketHoliday::CProductFinnhubMarketHoliday() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/market-holiday?exchange=";
}

string CProductFinnhubMarketHoliday::CreateMessage() {
	const auto strParam = gl_dataContainerStockExchange.GetItemExchangeCode(m_lIndex);

	m_strInquiringExchange = strParam;
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductFinnhubMarketHoliday::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvHoliday = ParseFinnhubMarketHoliday(pWebData);
	const auto pExchange = gl_dataContainerStockExchange.GetItem(m_lIndex);
	pExchange->SetUpdateMarketHoliday(false);

	gl_pWorldMarket->UpdateMarketHoliday(pvHoliday);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网址：https://finnhub.io/docs/api/market-holiday
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
CMarketHolidaysPtr CProductFinnhubMarketHoliday::ParseFinnhubMarketHoliday(const CWebDataPtr& pWebData) {
	auto pvHoliday = make_shared<vector<CMarketHolidayPtr>>();
	CMarketHolidayPtr pHoliday = nullptr;
	string s, sError;
	string sExchange, sTimeZone;
	json js;

	if (!pWebData->CreateJson(js)) return pvHoliday;
	if (!IsValidData(pWebData)) return pvHoliday;

	s = jsonGetString(js, "exchange");
	if (!s.empty()) sExchange = s;
	s = jsonGetString(js, "timezone");
	if (!s.empty()) sTimeZone = s;
	auto js1 = jsonGetChild(js, "data");
	try {
		for (auto it = js1.begin(); it != js1.end(); ++it) {
			pHoliday = make_shared<CMarketHoliday>();
			s = jsonGetString(it, "eventName");
			if (!s.empty()) pHoliday->m_strEventName = s;
			s = jsonGetString(it, "atDate");
			pHoliday->m_lDate = XferToYYYYMMDD(s);
			s = jsonGetString(it, "tradingHour");
			pHoliday->m_strTradingHour = s;
			pHoliday->m_strExchange = sExchange;
			pHoliday->m_strTimeZone = sTimeZone;
			pvHoliday->push_back(pHoliday);
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub market holiday ", e.what());
		return pvHoliday;
	}
	return pvHoliday;
}
