#include "pch.h"

#include"SystemMessage.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubForexExchange.h"

#include "ContainerFinnhubForexExchange.h"
#include "FinnhubDataSource.h"

#include"cpr/cpr.h"

using std::make_shared;

CProductFinnhubForexExchange::CProductFinnhubForexExchange() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/forex/exchange?";
}

void CProductFinnhubForexExchange::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });
		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}

		const auto pvForexExchange = Parse(m_r.text);
		for (const auto& str : *pvForexExchange) {
			if (!gl_dataContainerFinnhubForexExchange.IsExchange(str)) {
				gl_dataContainerFinnhubForexExchange.Add(str);
			}
		}
	}
}

void CProductFinnhubForexExchange::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0:
		break;
	case 302: //redirected, not an error
		break;
	case 401:
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		break;
	default:
		WebErrorReport();
		break;
	}
}

shared_ptr<vector<string>> CProductFinnhubForexExchange::CreateMessage() {
	ABSL_DCHECK(m_strInquiringExchange == "ALL");
	m_strInquiringExchange = "ALL"; // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	m_inquiryString = m_strInquiryFunction;
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

shared_ptr<vector<string>> CProductFinnhubForexExchange::Parse(const string& text) {
	auto pvExchange = make_shared<vector<string>>();
	string sError;
	nlohmannJson js;

	if (text.empty()) return pvExchange;
	if (!::CreateJsonWithNlohmann(js, text)) return pvExchange;
	if (::IsVoidJson(text)) return pvExchange;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			string str = jsonGetString(it);
			pvExchange->push_back(str);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Forex Exchange ", e.what());
	}
	return pvExchange;
}

void CProductFinnhubForexExchange::UpdateSystemStatus() {
	gl_pFinnhubDataSource->SetUpdateForexExchange(false);
	gl_systemMessage.PushInformationMessage("Finnhub forex exchange updated");
}
