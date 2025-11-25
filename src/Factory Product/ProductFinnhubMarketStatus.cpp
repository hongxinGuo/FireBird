#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

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
	auto pvMarketStatus = make_shared<vector<CMarketStatusPtr>>();
	CMarketStatusPtr pMarketStatus = nullptr;
	string sError;
	json js;

	if (!pWebData->CreateJson(js)) return pvMarketStatus;
	if (!IsValidData(pWebData)) return pvMarketStatus;

	try {
		string s;
		pMarketStatus = make_shared<CMarketStatus>();
		s = jsonGetString(js, "exchange");
		if (!s.empty()) pMarketStatus->m_strExchange = s;
		s = jsonGetString(js, "holiday");
		if (!s.empty()) pMarketStatus->m_strHoliday = s;
		pMarketStatus->m_bOpen = js.at("isOpen");
		s = jsonGetString(js, "session");
		if (!s.empty()) pMarketStatus->m_strSession = s;
		s = jsonGetString(js, "timezone");
		if (!s.empty()) pMarketStatus->m_strTimeZone = s;
		pMarketStatus->m_tt = jsonGetLongLong(js, "t");

		pvMarketStatus->push_back(pMarketStatus);
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Market Status ", e.what());
		return pvMarketStatus;
	}
	return pvMarketStatus;
}
