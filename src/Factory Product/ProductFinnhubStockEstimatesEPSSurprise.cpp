#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"

#include "ProductFinnhubStockEstimatesEPSSurprise.h"

#include "TimeConvert.h"

CProductFinnhubStockEstimatesEPSSurprise::CProductFinnhubStockEstimatesEPSSurprise() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/earnings?symbol=");
}

CString CProductFinnhubStockEstimatesEPSSurprise::CreateMessage() {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);
	const auto strParam = pStock->GetSymbol();

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductFinnhubStockEstimatesEPSSurprise::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);
	const auto pvEPSSurprise = ParseFinnhubEPSSurprise(pWebData);
	if (!pvEPSSurprise->empty()) { pStock->UpdateEPSSurprise(*pvEPSSurprise); }
	else {
		pStock->SetLastEPSSurpriseUpdateDate(19700101); // 将日期设置为更早。
		pStock->SetUpdateProfileDB(true);
	}
	pStock->m_fUpdateEPSSurprise = false;
	pStock->m_fUpdateEPSSurpriseDB = true;
}

CEPSSurprisesPtr CProductFinnhubStockEstimatesEPSSurprise::ParseFinnhubEPSSurprise(const CWebDataPtr& pWebData) {
	auto pvEPSSurprise = make_shared<vector<CEPSSurprisePtr>>();
	string s;
	CEPSSurprisePtr pEPSSurprise = nullptr;
	long year = 0, month = 0, day = 0;
	string sError;
	json js;

	if (!pWebData->CreateJson(js)) return pvEPSSurprise;
	if (!IsValidData(pWebData)) return pvEPSSurprise;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			pEPSSurprise = make_shared<CEPSSurprise>();
			s = jsonGetString(it, _T("symbol"));
			pEPSSurprise->m_strSymbol = s.c_str();
			s = jsonGetString(it,_T("period"));
			pEPSSurprise->m_lDate = XferToYYYYMMDD(s);
			pEPSSurprise->m_dEstimate = jsonGetDouble(it, _T("estimate"));
			pEPSSurprise->m_dActual = jsonGetDouble(it, _T("actual"));
			pvEPSSurprise->push_back(pEPSSurprise);
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub EPS Surprise "), e.what());
		return pvEPSSurprise;
	}
	std::ranges::sort(pvEPSSurprise->begin(), pvEPSSurprise->end(),
	                  [](const CEPSSurprisePtr& p1, const CEPSSurprisePtr& p2) { return (p1->m_lDate < p2->m_lDate); }); // 以日期早晚顺序排列。
	return pvEPSSurprise;
}
