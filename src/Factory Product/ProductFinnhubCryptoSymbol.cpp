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
	const CString strParam = dynamic_cast<CWorldMarket*>(m_pMarket)->GetCryptoExchange(m_lIndex);

	m_strInquiringExchange = strParam;
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

bool CProductFinnhubCryptoSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto pvCryptoSymbol = ParseFinnhubCryptoSymbol(pWebData);
	for (const auto& pSymbol : *pvCryptoSymbol) {
		if (!dynamic_cast<CWorldMarket*>(m_pMarket)->IsFinnhubCryptoSymbol(pSymbol->GetSymbol())) {
			pSymbol->SetExchangeCode(dynamic_cast<CWorldMarket*>(m_pMarket)->GetCryptoExchange(m_lIndex));
			dynamic_cast<CWorldMarket*>(m_pMarket)->AddFinnhubCryptoSymbol(pSymbol);
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
CFinnhubCryptoSymbolVectorPtr CProductFinnhubCryptoSymbol::ParseFinnhubCryptoSymbol(CWebDataPtr pWebData) {
	auto pvCryptoSymbol = make_shared<vector<CFinnhubCryptoSymbolPtr>>();
	string s;
	string sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvCryptoSymbol;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return pvCryptoSymbol;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return pvCryptoSymbol;
	}
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
