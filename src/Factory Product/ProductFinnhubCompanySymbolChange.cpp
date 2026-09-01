//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Finnhub经济日历。
///	此信息为premium，使用此信息来判断finnhub账户类型（免费还是付费）
///
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"SystemMessage.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubCompanySymbolChange.h"
#include"FinnhubCompanySymbolChange.h"

#include "FinnhubDataSource.h"
#include "SystemConfiguration.h"
#include<cpr/cpr.h>

using std::make_shared;

CProductFinnhubCompanySymbolChange::CProductFinnhubCompanySymbolChange() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/ca/symbol-change?from=2020-01-01&to=2025-12-31";
}

void CProductFinnhubCompanySymbolChange::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}
		auto pv = Parse(m_r.text);
		//gl_dataContainerFinnhubCompanySymbolChange.Update(*pv);
	}
}

void CProductFinnhubCompanySymbolChange::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 401:
	case 403: // no right to access
		if (r.text == R"({"error":"You don't have access to this resource."})") {
			m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		}
		break;
	default:
		WebErrorReport();
		break;
	}
}

shared_ptr<vector<string>> CProductFinnhubCompanySymbolChange::CreateMessage() {
	m_inquiryString = m_strInquiryFunction;
	ABSL_DCHECK(m_strInquiringExchange == "ALL");
	m_strInquiringExchange = "ALL"; // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

CCompanySymbolChangesPtr CProductFinnhubCompanySymbolChange::Parse(const string& text) {
	auto pvCompanySymbolChange = make_shared<vector<CCompanySymbolChange>>();
	nlohmannJson js;

	if (text.empty()) return pvCompanySymbolChange;
	if (!::CreateJsonWithNlohmann(js, text)) return pvCompanySymbolChange;
	if (::IsVoidJson(text)) return pvCompanySymbolChange;

	try {
		nlohmannJson js2 = jsonGetChild(js, "data");
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			CCompanySymbolChange companySymbolChange;
			string s = jsonGetString(it, "atDate");
			companySymbolChange.m_sTime = s;
			s = jsonGetString(it, "newSymbol");
			companySymbolChange.m_sNewSymbol = s;
			s = jsonGetString(it, "oldSymbol");
			companySymbolChange.m_sOldSymbol = s;
			pvCompanySymbolChange->push_back(companySymbolChange);
			companySymbolChange.Reset();
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub company symbol change ", e.what());
	}
	return pvCompanySymbolChange;
}

void CProductFinnhubCompanySymbolChange::UpdateSystemStatus() {
	if (m_r.status_code != 200 && m_r.status_code != 403) return;

	gl_pFinnhubDataSource->SetUpdateCompanySymbolChange(false);
	gl_systemMessage.PushInformationMessage("Finnhub company symbol change updated");
	if (IsNoRightToAccess()) {// Note 在此确定Finnhub账户类型
		gl_systemConfiguration.ChangeFinnhubAccountTypeToFree();
		gl_systemMessage.PushStockMarketInformationMessage("free finnhub account");
	}
	else {
		gl_systemConfiguration.ChangeFinnhubAccountTypeToPaid();
		gl_systemMessage.PushStockMarketInformationMessage("paid finnhub account");
	}
}
