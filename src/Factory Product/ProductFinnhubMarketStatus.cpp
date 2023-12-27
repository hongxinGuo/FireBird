#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"

#include "ProductFinnhubMarketStatus.h"

CProductFinnhubMarketStatus::CProductFinnhubMarketStatus() {
	m_strClassName = _T("Finnhub market status");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/market-status?exchange=");
	m_lIndex = -1;
}

CString CProductFinnhubMarketStatus::CreateMessage() {
	const auto strParam = gl_dataContainerFinnhubStockExchange.GetExchangeCode(m_lIndex);

	m_strInquiringExchange = strParam;
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

bool CProductFinnhubMarketStatus::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvMarketStatus = ParseFinnhubMarketStatus(pWebData);
	const auto pExchange = gl_dataContainerFinnhubStockExchange.GetExchange(m_lIndex);
	pExchange->SetMarketStatusUpdated(true);

	if (!pvMarketStatus->empty()) {
		gl_pWorldMarket->UpdateMarketStatus(pvMarketStatus);
	}

	return true;
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
CMarketStatusVectorPtr CProductFinnhubMarketStatus::ParseFinnhubMarketStatus(const CWebDataPtr& pWebData) {
	auto pvMarketStatus = make_shared<vector<CMarketStatusPtr>>();
	CMarketStatusPtr pMarketStatus = nullptr;
	string s, sError;

	if (!IsValidData(pWebData)) return pvMarketStatus;

	const auto pjs = pWebData->GetJSon();
	try {
		pMarketStatus = make_shared<CMarketStatus>();
		s = jsonGetString(pjs, _T("exchange"));
		if (!s.empty()) pMarketStatus->m_strExchange = s.c_str();
		s = jsonGetString(pjs, _T("holiday"));
		if (!s.empty()) pMarketStatus->m_strHoliday = s.c_str();
		pMarketStatus->m_bOpen = pjs->at(_T("isOpen"));
		s = jsonGetString(pjs, _T("session"));
		if (!s.empty()) pMarketStatus->m_strSession = s.c_str();
		s = jsonGetString(pjs, _T("timezone"));
		if (!s.empty()) pMarketStatus->m_strTimeZone = s.c_str();
		pMarketStatus->m_tt = jsonGetLongLong(pjs, _T("t"));

		pvMarketStatus->push_back(pMarketStatus);
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Market Status "), e.what());
		return pvMarketStatus;
	}
	return pvMarketStatus;
}
