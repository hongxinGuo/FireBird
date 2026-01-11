#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubCryptoSymbol.h"

#include "WebData.h"

CProductFinnhubCryptoSymbol::CProductFinnhubCryptoSymbol() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/crypto/symbol?exchange=";
}

string CProductFinnhubCryptoSymbol::CreateMessage() {
	const string strParam = gl_dataContainerFinnhubCryptoExchange.GetItem(m_lIndex);

	m_strInquiringExchange = strParam;
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductFinnhubCryptoSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvCryptoSymbol = ParseFinnhubCryptoSymbol(pWebData);
	for (const auto& pSymbol : *pvCryptoSymbol) {
		if (!gl_dataFinnhubCryptoSymbol.IsSymbol(pSymbol->GetSymbol())) {
			pSymbol->SetExchangeCode(gl_dataContainerFinnhubCryptoExchange.GetItem(m_lIndex));
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
CFinnhubCryptosPtr CProductFinnhubCryptoSymbol::ParseFinnhubCryptoSymbol(const CWebDataPtr& pWebData) {
	auto pvCryptoSymbol = make_shared<vector<CFinnhubCryptoPtr>>();
	string s;
	string sError;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvCryptoSymbol;
	if (!IsValidData(pWebData)) return pvCryptoSymbol;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			auto pSymbol = make_shared<CFinnhubCrypto>();
			s = jsonGetString(it, "description");
			if (!s.empty()) pSymbol->SetDescription(s);
			s = jsonGetString(it, "displaySymbol");
			pSymbol->SetDisplaySymbol(s);
			s = jsonGetString(it, "symbol");
			pSymbol->SetSymbol(s);
			pvCryptoSymbol->push_back(pSymbol);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Crypto Symbol ", e.what());
	}
	return pvCryptoSymbol;
}
