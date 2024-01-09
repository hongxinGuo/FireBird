#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include "ProductFinnhubForexSymbol.h"

CProductFinnhubForexSymbol::CProductFinnhubForexSymbol() {
	m_strClassName = _T("Finnhub forex symbols");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/forex/symbol?exchange=");
	m_lIndex = -1;
}

CString CProductFinnhubForexSymbol::CreateMessage() {
	const auto strParam = gl_dataContainerFinnhubForexExchange.GetExchange(m_lIndex);

	m_strInquiringExchange = strParam;
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

bool CProductFinnhubForexSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvForexSymbol = ParseFinnhubForexSymbol(pWebData);
	if (pvForexSymbol->empty()) return false;
	for (const auto& pSymbol : *pvForexSymbol) {
		if (!gl_dataFinnhubForexSymbol.IsSymbol(pSymbol->GetSymbol())) {
			pSymbol->SetExchangeCode(gl_dataContainerFinnhubForexExchange.GetExchange(m_lIndex));
			gl_dataFinnhubForexSymbol.Add(pSymbol);
		}
	}

	return true;
}

CForexSymbolVectorPtr CProductFinnhubForexSymbol::ParseFinnhubForexSymbol(const CWebDataPtr& pWebData) {
	auto pvForexSymbol = make_shared<vector<CForexSymbolPtr>>();
	CForexSymbolPtr pSymbol = nullptr;
	string s;
	string sError;

	if (!pWebData->IsParsed()) {
		if (!pWebData->CreateJson()) return pvForexSymbol;
	}
	if (!IsValidData(pWebData)) return pvForexSymbol;

	const auto pjs = pWebData->GetJSon();
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			pSymbol = make_shared<CFinnhubForexSymbol>();
			s = jsonGetString(it, _T("description"));
			if (!s.empty()) pSymbol->SetDescription(s.c_str());
			s = jsonGetString(it,_T("displaySymbol"));
			pSymbol->SetDisplaySymbol(s.c_str());
			s = jsonGetString(it,_T("symbol"));
			pSymbol->SetSymbol(s.c_str());
			pvForexSymbol->push_back(pSymbol);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Symbol "), e.what());
	}

	return pvForexSymbol;
}
