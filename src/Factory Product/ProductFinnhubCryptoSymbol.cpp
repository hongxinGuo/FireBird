#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include "ProductFinnhubCryptoSymbol.h"

CProductFinnhubCryptoSymbol::CProductFinnhubCryptoSymbol() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/crypto/symbol?exchange=");
}

CString CProductFinnhubCryptoSymbol::CreateMessage() {
	const CString strParam = gl_dataContainerFinnhubCryptoExchange.GetExchange(m_lIndex).c_str();

	m_strInquiringExchange = strParam;
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductFinnhubCryptoSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvCryptoSymbol = ParseFinnhubCryptoSymbol(pWebData);
	for (const auto& pSymbol : *pvCryptoSymbol) {
		if (!gl_dataFinnhubCryptoSymbol.IsSymbol(pSymbol->GetSymbol())) {
			pSymbol->SetExchangeCode(gl_dataContainerFinnhubCryptoExchange.GetExchange(m_lIndex).c_str());
			gl_dataFinnhubCryptoSymbol.Add(pSymbol);
		}
	}
}

//
//
// [{
//		"description": "Binance ETHBTC",
//		"displaySymbol": "ETH/BTC",
//		"symbol": "ETHBTC"
//  },
//  {
//    "description": "Binance LTCBTC",
//    "displaySymbol": "LTC/BTC",
//    "symbol": "BINANCE:LTCBTC"
//  }]
//
//
CFinnhubCryptoSymbolsPtr CProductFinnhubCryptoSymbol::ParseFinnhubCryptoSymbol(const CWebDataPtr& pWebData) {
	auto pvCryptoSymbol = make_shared<vector<CFinnhubCryptoSymbolPtr>>();
	string s;
	string sError;
	json js;

	if (!pWebData->CreateJson(js)) return pvCryptoSymbol;
	if (!IsValidData(pWebData)) return pvCryptoSymbol;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			auto pSymbol = make_shared<CFinnhubCryptoSymbol>();
			s = jsonGetString(it, _T("description"));
			if (!s.empty()) pSymbol->SetDescription(s.c_str());
			s = jsonGetString(it,_T("displaySymbol"));
			pSymbol->SetDisplaySymbol(s.c_str());
			s = jsonGetString(it,_T("symbol"));
			pSymbol->SetSymbol(s.c_str());
			pvCryptoSymbol->push_back(pSymbol);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Symbol "), e.what());
	}
	return pvCryptoSymbol;
}
