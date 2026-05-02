#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"WorldMarket.h"

#include "ProductFinnhubMarketStatus.h"

#include "WebData.h"

CProductFinnhubMarketStatus::CProductFinnhubMarketStatus() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/market-status?exchange=";
}

string CProductFinnhubMarketStatus::CreateMessage() {
	m_strInquiringExchange = gl_dataContainerStockExchange.GetItemExchangeCode(m_lIndex);
	m_strInquiry = m_strInquiryFunction + m_strInquiringExchange;

	return m_strInquiry;
}

void CProductFinnhubMarketStatus::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvMarketStatus = ParseFinnhubMarketStatus(pWebData);
	const auto pExchange = gl_dataContainerStockExchange.GetItem(m_lIndex);
	pExchange->SetUpdateMarketStatus(false);

	if (!pvMarketStatus->empty()) {
		gl_pWorldMarket->UpdateMarketStatus(pvMarketStatus);
	}
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
CMarketStatusesPtr CProductFinnhubMarketStatus::ParseFinnhubMarketStatus(const CWebDataPtr& pWebData) {
	auto pvMarketStatus = make_shared<vector<CMarketStatus>>();
	pvMarketStatus->reserve(200);

	string sError;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvMarketStatus;
	if (!IsValidData(pWebData)) return pvMarketStatus;

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
