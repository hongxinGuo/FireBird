#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include "ProductFinnhubCryptoSymbol.h"

CProductFinnhubCryptoSymbol::CProductFinnhubCryptoSymbol() {
	m_strClassName = _T("Finnhub crypto symbols");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/crypto/symbol?exchange=");
	m_lIndex = -1;
}

CString CProductFinnhubCryptoSymbol::CreateMessage() {
	const CString strParam = GetMarket()->GetCryptoExchange(m_lIndex);

	m_strInquiringExchange = strParam;
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

bool CProductFinnhubCryptoSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pvCryptoSymbol = ParseFinnhubCryptoSymbol(pWebData);
	for (const auto& pSymbol : *pvCryptoSymbol) {
		if (!GetMarket()->IsFinnhubCryptoSymbol(pSymbol->GetSymbol())) {
			pSymbol->SetExchangeCode(GetMarket()->GetCryptoExchange(m_lIndex));
			GetMarket()->AddFinnhubCryptoSymbol(pSymbol);
		}
	}

	return true;
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
CFinnhubCryptoSymbolVectorPtr CProductFinnhubCryptoSymbol::ParseFinnhubCryptoSymbol(const CWebDataPtr& pWebData) {
	auto pvCryptoSymbol = make_shared<vector<CFinnhubCryptoSymbolPtr>>();
	string s;
	string sError;

	if (!IsValidData(pWebData)) return pvCryptoSymbol;

	const auto pjs = pWebData->GetJSon();
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
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
