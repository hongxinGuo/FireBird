#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"FinnhubStock.h"
#include"ChinaMarket.h"
#include "ProductFinnhubStockEstimatesEPSSurprise.h"

#include "TimeConvert.h"
#include "WebData.h"

CProductFinnhubStockEstimatesEPSSurprise::CProductFinnhubStockEstimatesEPSSurprise() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/earnings?symbol=";
}

string CProductFinnhubStockEstimatesEPSSurprise::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	const auto strParam = pStock->GetSymbol();

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductFinnhubStockEstimatesEPSSurprise::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	const auto pvEPSSurprise = ParseFinnhubEPSSurprise(pWebData);
	if (!pvEPSSurprise->empty()) { pStock->UpdateEPSSurprise(pvEPSSurprise); }
	else {
		pStock->SetLastEPSSurpriseUpdateDate(19700101); // 将日期设置为更早。
		pStock->SetUpdateProfileDB(true);
	}
	pStock->SetUpdateEPSSurprise(false);
	pStock->m_fUpdateEPSSurpriseDB = true;
}

CEPSSurprisesPtr CProductFinnhubStockEstimatesEPSSurprise::ParseFinnhubEPSSurprise(const CWebDataPtr& pWebData) {
	auto pvEPSSurprise = make_shared<vector<CEPSSurprise>>();
	CEPSSurprise pEPSSurprise;
	string sError;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvEPSSurprise;
	if (!IsValidData(pWebData)) return pvEPSSurprise;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			string s = jsonGetString(it, "symbol");
			pEPSSurprise.m_strSymbol = s;
			s = jsonGetString(it, "period");
			pEPSSurprise.m_lDate = XferToYYYYMMDD(s);
			pEPSSurprise.m_dEstimate = jsonGetDouble(it, "estimate");
			pEPSSurprise.m_dActual = jsonGetDouble(it, "actual");
			pvEPSSurprise->push_back(pEPSSurprise);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub EPS Surprise ", e.what());
		return pvEPSSurprise;
	}
	std::ranges::sort(pvEPSSurprise->begin(), pvEPSSurprise->end(),
	                  [](const CEPSSurprise& p1, const CEPSSurprise& p2) { return (p1.m_lDate < p2.m_lDate); }); // 以日期早晚顺序排列。
	return pvEPSSurprise;
}
