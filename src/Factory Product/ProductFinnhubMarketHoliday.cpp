#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubMarketHoliday.h"
#include "MarketHoliday.h"

#include "ContainerStockExchange.h"
#include "FinnhubDataSource.h"
#include "SystemMessage.h"
#include "TimeConvert.h"
#include "WorldMarket.h"
#include"cpr/cpr.h"

using namespace std;

CProductFinnhubMarketHoliday::CProductFinnhubMarketHoliday() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/market-holiday?exchange=";
}

void CProductFinnhubMarketHoliday::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}

		const auto pvHoliday = Parse(m_r.text);
		const auto pExchange = gl_dataContainerStockExchange.GetItem(m_index);
		pExchange->SetUpdateMarketHoliday(false);

		gl_pWorldMarket->UpdateMarketHoliday(pvHoliday);
	}
}

void CProductFinnhubMarketHoliday::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0: //
		// do nothing
		break;
	case 401: // no right to access
	case 403:
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		CheckInaccessible();
		break;
	default:
		string sType = typeid(this).name();
		string s = std::format("{} error. http code: {}, error code:{}, message:{}", sType, r.status_code, static_cast<int>(r.error.code), r.error.message);
		gl_systemMessage.PushErrorMessage(s);
		break;
	}
}

void CProductFinnhubMarketHoliday::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductFinnhubMarketHoliday::CreateMessage() {
	const auto strParam = gl_dataContainerStockExchange.GetItemExchangeCode(m_index);

	m_strInquiringExchange = strParam;
	m_inquiryString = m_strInquiryFunction + strParam;
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
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
CMarketHolidaysPtr CProductFinnhubMarketHoliday::Parse(const string& text) {
	auto pvHoliday = make_shared<vector<CMarketHoliday>>();
	pvHoliday->reserve(200);

	string s, sError;
	string sExchange, sTimeZone;
	nlohmannJson js;

	if (text.empty()) return pvHoliday;
	if (!::CreateJsonWithNlohmann(js, text)) return pvHoliday;
	if (::IsVoidJson(text)) return pvHoliday;

	try {
		s = jsonGetString(js, "exchange");
		if (!s.empty()) sExchange = s;
		s = jsonGetString(js, "timezone");
		if (!s.empty()) sTimeZone = s;
		auto js1 = jsonGetChild(js, "data");
		CMarketHoliday holiday;
		for (auto it = js1.begin(); it != js1.end(); ++it) {
			s = jsonGetString(it, "eventName");
			if (!s.empty()) holiday.m_strEventName = s;
			s = jsonGetString(it, "atDate");
			istringstream ss1(s);
			chrono::local_days ld1;
			ss1 >> chrono::parse("%F", ld1);
			holiday.m_date = toFormattedDate(ld1);
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
