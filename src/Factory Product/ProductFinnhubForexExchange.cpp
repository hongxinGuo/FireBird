#include "pch.h"

#include"SystemMessage.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubForexExchange.h"

#include "ContainerFinnhubForexExchange.h"
#include "FinnhubDataSource.h"
#include "WebData.h"

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
		cpr::Response r = cpr::Get(cpr::Url{ inquireString });
		m_statusCode = r.status_code;
		m_elapsed = r.elapsed;

		if (m_statusCode != 200) {
			WebStatusCheck(r);
			return;
		}

		const auto pvForexExchange = Parse(r.text);
		for (const auto& str : *pvForexExchange) {
			if (!gl_dataContainerFinnhubForexExchange.IsExchange(str)) {
				gl_dataContainerFinnhubForexExchange.Add(str);
			}
		}
	}
}

void CProductFinnhubForexExchange::WebStatusCheck(cpr::Response& r) {
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
