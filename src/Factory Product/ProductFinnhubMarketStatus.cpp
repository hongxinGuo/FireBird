#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"

#include "ProductFinnhubMarketStatus.h"

#include "WebData.h"

CProductFinnhubMarketStatus::CProductFinnhubMarketStatus() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/market-status?exchange=");
}

string CProductFinnhubMarketStatus::CreateMessage() {
	m_strInquiringExchange = gl_dataContainerStockExchange.GetExchangeCode(m_lIndex);
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
// ÍøÖ·£ºhttps://finnhub.io/docs/api/market-status
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
		s = jsonGetString(js, _T("exchange"));
		if (!s.empty()) pMarketStatus->m_strExchange = s;
		s = jsonGetString(js, _T("holiday"));
		if (!s.empty()) pMarketStatus->m_strHoliday = s;
		pMarketStatus->m_bOpen = js.at(_T("isOpen"));
		s = jsonGetString(js, _T("session"));
		if (!s.empty()) pMarketStatus->m_strSession = s;
		s = jsonGetString(js, _T("timezone"));
		if (!s.empty()) pMarketStatus->m_strTimeZone = s;
		pMarketStatus->m_tt = jsonGetLongLong(js, _T("t"));

		pvMarketStatus->push_back(pMarketStatus);
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Market Status "), e.what());
		return pvMarketStatus;
	}
	return pvMarketStatus;
}
