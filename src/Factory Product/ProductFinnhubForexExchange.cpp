#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include "ProductFinnhubForexExchange.h"

#include "FinnhubDataSource.h"
#include "WebData.h"

CProductFinnhubForexExchange::CProductFinnhubForexExchange() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/forex/exchange?";
}

string CProductFinnhubForexExchange::CreateMessage() {
	ASSERT(m_strInquiringExchange.compare("ALL") == 0);
	m_strInquiringExchange = "ALL"; // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductFinnhubForexExchange::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvForexExchange = ParseFinnhubForexExchange(pWebData);
	for (int i = 0; i < pvForexExchange->size(); i++) {
		if (!gl_dataContainerFinnhubForexExchange.IsExchange(pvForexExchange->at(i))) {
			gl_dataContainerFinnhubForexExchange.Add(pvForexExchange->at(i));
		}
	}
}

shared_ptr<vector<string>> CProductFinnhubForexExchange::ParseFinnhubForexExchange(const CWebDataPtr& pWebData) {
	auto pvExchange = make_shared<vector<string>>();
	string s;
	string str = "";
	string sError;
	json js;

	if (!pWebData->CreateJson(js)) return pvExchange;
	if (!IsValidData(pWebData)) return pvExchange;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			s = jsonGetString(it);
			str = s;
			pvExchange->push_back(str);
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Forex Exchange ", e.what());
	}
	return pvExchange;
}

void CProductFinnhubForexExchange::UpdateSystemStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), "class CFinnhubDataSource") == 0);
	dynamic_pointer_cast<CFinnhubDataSource>(pDataSource)->SetUpdateForexExchange(false);
	gl_systemMessage.PushInformationMessage("Finnhub forex exchange updated");
}
