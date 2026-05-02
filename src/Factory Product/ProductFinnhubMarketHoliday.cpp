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
	auto pvHoliday = make_shared<vector<CMarketHoliday>>();
	pvHoliday->reserve(200);

	string s, sError;
	string sExchange, sTimeZone;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvHoliday;
	if (!IsValidData(pWebData)) return pvHoliday;

	s = jsonGetString(js, "exchange");
	if (!s.empty()) sExchange = s;
	s = jsonGetString(js, "timezone");
	if (!s.empty()) sTimeZone = s;
	auto js1 = jsonGetChild(js, "data");
	try {
		CMarketHoliday holiday;
		for (auto it = js1.begin(); it != js1.end(); ++it) {
			s = jsonGetString(it, "eventName");
			if (!s.empty()) holiday.m_strEventName = s;
			s = jsonGetString(it, "atDate");
			holiday.m_lDate = XferToYYYYMMDD(s);
			s = jsonGetString(it, "tradingHour");
			holiday.m_strTradingHour = s;
			holiday.m_strExchange = sExchange;
			holiday.m_strTimeZone = sTimeZone;
			pvHoliday->push_back(holiday);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub market holiday ", e.what());
		return pvHoliday;
	}
	return pvHoliday;
}
