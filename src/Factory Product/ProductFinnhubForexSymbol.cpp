#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubForexSymbol.h"

#include "WebData.h"

CProductFinnhubForexSymbol::CProductFinnhubForexSymbol() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/forex/symbol?exchange=";
}

string CProductFinnhubForexSymbol::CreateMessage() {
	m_strInquiringExchange = gl_dataContainerFinnhubForexExchange.GetItem(m_lIndex);
	m_strInquiry = m_strInquiryFunction + m_strInquiringExchange;
	return m_strInquiry;
}

void CProductFinnhubForexSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvForexSymbol = ParseFinnhubForexSymbol(pWebData);
	if (pvForexSymbol->empty()) return;
	for (const auto& pSymbol : *pvForexSymbol) {
		if (!gl_dataFinnhubForexSymbol.IsSymbol(pSymbol->GetSymbol())) {
			pSymbol->SetExchange(gl_dataContainerFinnhubForexExchange.GetItem(m_lIndex));
			gl_dataFinnhubForexSymbol.Add(pSymbol);
		}
	}
}

CForexSymbolsPtr CProductFinnhubForexSymbol::ParseFinnhubForexSymbol(const CWebDataPtr& pWebData) {
	auto pvForexSymbol = make_shared<vector<CForexSymbolPtr>>();
	CForexSymbolPtr pSymbol = nullptr;
	string sError;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvForexSymbol;
	if (!IsValidData(pWebData)) return pvForexSymbol;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			pSymbol = make_shared<CFinnhubForex>();
			string s = jsonGetString(it, "description");
			if (!s.empty()) pSymbol->SetDescription(s);
			s = jsonGetString(it, "displaySymbol");
			pSymbol->SetDisplaySymbol(s);
			s = jsonGetString(it, "symbol");
			pSymbol->SetSymbol(s);
			pvForexSymbol->push_back(pSymbol);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Forex Symbol ", e.what());
	}

	return pvForexSymbol;
}
