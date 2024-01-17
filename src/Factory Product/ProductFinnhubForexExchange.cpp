#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include "ProductFinnhubForexExchange.h"

#include "FinnhubDataSource.h"

CProductFinnhubForexExchange::CProductFinnhubForexExchange() {
	m_strClassName = _T("Finnhub forex exchange");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/forex/exchange?");
	m_lIndex = -1;
}

CString CProductFinnhubForexExchange::CreateMessage() {
	ASSERT(m_strInquiringExchange.Compare(_T("ALL")) == 0);
	m_strInquiringExchange = _T("ALL"); // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

bool CProductFinnhubForexExchange::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvForexExchange = ParseFinnhubForexExchange(pWebData);
	for (int i = 0; i < pvForexExchange->size(); i++) {
		if (!gl_dataContainerFinnhubForexExchange.IsExchange(pvForexExchange->at(i))) {
			gl_dataContainerFinnhubForexExchange.Add(pvForexExchange->at(i));
		}
	}

	return true;
}

shared_ptr<vector<CString>> CProductFinnhubForexExchange::ParseFinnhubForexExchange(const CWebDataPtr& pWebData) {
	auto pvExchange = make_shared<vector<CString>>();
	string s;
	CString str = _T("");
	string sError;
	json js;

	if (!pWebData->CreateJson(js)) return pvExchange;
	if (!IsValidData(pWebData)) return pvExchange;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			s = jsonGetString(it);
			str = s.c_str();
			pvExchange->push_back(str);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Exchange "), e.what());
	}
	return pvExchange;
}

void CProductFinnhubForexExchange::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CFinnhubDataSource")) == 0);
	dynamic_pointer_cast<CFinnhubDataSource>(pDataSource)->m_fUpdateForexExchange = false;
}
