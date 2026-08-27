#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"WorldMarket.h"
#include "MarketStatus.h"

#include "ProductFinnhubMarketStatus.h"

#include "ContainerStockExchange.h"
#include "FinnhubDataSource.h"
#include"cpr/cpr.h"

using std::make_shared;

CProductFinnhubMarketStatus::CProductFinnhubMarketStatus() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/market-status?exchange=";
}

void CProductFinnhubMarketStatus::InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		cpr::Response r = cpr::Get(cpr::Url{ inquireString });
		m_statusCode = r.status_code;
		m_elapsed = r.elapsed;

		if (m_statusCode != 200) {
			WebStatusCheck(r);
			return;
		}
		const auto pvMarketStatus = Parse(r.text);
		const auto pExchange = gl_dataContainerStockExchange.GetItem(m_index);
		pExchange->SetUpdateMarketStatus(false);

		if (!pvMarketStatus->empty()) {
			gl_pWorldMarket->UpdateMarketStatus(pvMarketStatus);
		}
	}
}

void CProductFinnhubMarketStatus::WebStatusCheck(cpr::Response& r) {
}

void CProductFinnhubMarketStatus::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductFinnhubMarketStatus::CreateMessage() {
	m_strInquiringExchange = gl_dataContainerStockExchange.GetItemExchangeCode(m_index);
	m_inquiryString = m_strInquiryFunction + m_strInquiringExchange;

	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网址：https://finnhub.io/docs/api/market-status
//
// {
// "exchange": "US",
// "holiday": null,
// "isOpen" : false,
// "session" : "pre-market",
// "timeZone" : "America/New York",
// "t" : 1697018041
// }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
CMarketStatusesPtr CProductFinnhubMarketStatus::Parse(const string& text) {
	auto pvMarketStatus = make_shared<vector<CMarketStatus>>();
	pvMarketStatus->reserve(200);

	string sError;
	nlohmannJson js;

	if (text.empty()) return pvMarketStatus;
	if (!::CreateJsonWithNlohmann(js, text)) return pvMarketStatus;
	if (::IsVoidJson(text)) return pvMarketStatus;

	try {
		CMarketStatus marketStatus;
		string s = jsonGetString(js, "exchange");
		if (!s.empty()) marketStatus.m_strExchange = s;
		s = jsonGetString(js, "holiday");
		if (!s.empty()) marketStatus.m_strHoliday = s;
		marketStatus.m_bOpen = js.at("isOpen");
		s = jsonGetString(js, "session");
		if (!s.empty()) marketStatus.m_strSession = s;
		s = jsonGetString(js, "timezone");
		if (!s.empty()) marketStatus.m_strTimeZone = s;
		marketStatus.m_tt = jsonGetLongLong(js, "t");

		pvMarketStatus->push_back(marketStatus);
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Market Status ", e.what());
		return pvMarketStatus;
	}
	return pvMarketStatus;
}
