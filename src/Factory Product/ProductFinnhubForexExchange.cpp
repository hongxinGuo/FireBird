#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubForexExchange.h"

#include "FinnhubDataSource.h"
#include "WebData.h"

CProductFinnhubForexExchange::CProductFinnhubForexExchange() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/forex/exchange?";
}

string CProductFinnhubForexExchange::CreateMessage() {
	ASSERT(m_strInquiringExchange == "ALL");
	m_strInquiringExchange = "ALL"; // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductFinnhubForexExchange::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvForexExchange = ParseFinnhubForexExchange(pWebData);
	for (const auto& str : *pvForexExchange) {
		if (!gl_dataContainerFinnhubForexExchange.IsExchange(str)) {
			gl_dataContainerFinnhubForexExchange.Add(str);
		}
	}
}

shared_ptr<vector<string>> CProductFinnhubForexExchange::ParseFinnhubForexExchange(const CWebDataPtr& pWebData) {
	auto pvExchange = make_shared<vector<string>>();
	string sError;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvExchange;
	if (!IsValidData(pWebData)) return pvExchange;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			string s = jsonGetString(it);
			string str = s;
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
