#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"FinnhubStock.h"
#include "ProductFinnhubStockEstimatesEPSSurprise.h"
#include"EPSSurprise.h"

#include "ContainerFinnhubStock.h"
#include "FinnhubDataSource.h"
#include "SystemMessage.h"
#include"cpr/cpr.h"

using std::make_shared;
using std::istringstream;

CProductFinnhubStockEstimatesEPSSurprise::CProductFinnhubStockEstimatesEPSSurprise() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/earnings?symbol=";
}

void CProductFinnhubStockEstimatesEPSSurprise::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}
		const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
		const auto pvEPSSurprise = Parse(m_r.text);
		if (!pvEPSSurprise->empty()) { pStock->UpdateEPSSurprise(pvEPSSurprise); }
		else {
			pStock->SetLastEPSSurpriseUpdateDate(local_days(days(0))); // 将日期设置为更早。
			pStock->SetUpdateProfileDB(true);
		}
		pStock->SetUpdateEPSSurprise(false);
		pStock->m_fUpdateEPSSurpriseDB = true;
	}
}

void CProductFinnhubStockEstimatesEPSSurprise::WebStatusCheck(cpr::Response& r) {
	string s;
	switch (r.status_code) {
	case 0:
		break;
	case 302: //redirected, not an error
	case 403: // forbidden
		WebErrorReport();
		break;
	default:
		WebErrorReport();
		break;
	}
}

void CProductFinnhubStockEstimatesEPSSurprise::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductFinnhubStockEstimatesEPSSurprise::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
	const auto strParam = pStock->GetSymbol();

	m_strInquiringExchange = pStock->GetExchange();
	m_inquiryString = m_strInquiryFunction + strParam;
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

CEPSSurprisesPtr CProductFinnhubStockEstimatesEPSSurprise::Parse(const string& text) {
	auto pvEPSSurprise = make_shared<vector<CEPSSurprise>>();
	string sError;
	nlohmannJson js;

	if (text.empty()) return pvEPSSurprise;
	if (!::CreateJsonWithNlohmann(js, text)) return pvEPSSurprise;
	if (::IsVoidJson(text)) return pvEPSSurprise; // 即使为空，也完成了查询。
	if (IsNoRightToAccess()) return pvEPSSurprise;

	try {
		CEPSSurprise pEPSSurprise;
		for (auto it = js.begin(); it != js.end(); ++it) {
			string s = jsonGetString(it, "symbol");
			pEPSSurprise.m_strSymbol = s;
			s = jsonGetString(it, "period");
			istringstream ss(s);
			local_days ld;
			ss >> parse("%F", ld);
			pEPSSurprise.m_lDate = ld;
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
