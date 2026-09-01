#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubCryptoSymbol.h"

#include <cpr/api.h>

#include "ContainerFinnhubCrypto.h"
#include "ContainerFinnhubCryptoExchange.h"
#include "FinnhubCrypto.h"
#include "FinnhubDataSource.h"
#include"cpr/cpr.h"

using std::make_shared;

CProductFinnhubCryptoSymbol::CProductFinnhubCryptoSymbol() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/crypto/symbol?exchange=";
}
void CProductFinnhubCryptoSymbol::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}

		const auto pvCryptoSymbol = Parse(m_r.text);
		for (const auto& pSymbol : *pvCryptoSymbol) {
			if (!gl_dataFinnhubCryptoSymbol.IsSymbol(pSymbol->GetSymbol())) {
				pSymbol->SetExchange(gl_dataContainerFinnhubCryptoExchange.GetItem(m_index));
				gl_dataFinnhubCryptoSymbol.Add(pSymbol);
			}
		}
	}
}
void CProductFinnhubCryptoSymbol::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0:
		break;
	case 302: //redirected, not an error
		break;
	case 401:
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		CheckInaccessible();
		break;
	default:
		WebErrorReport();
		break;
	}
}

void CProductFinnhubCryptoSymbol::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductFinnhubCryptoSymbol::CreateMessage() {
	const string strParam = gl_dataContainerFinnhubCryptoExchange.GetItem(m_index);

	m_strInquiringExchange = strParam;
	m_inquiryString = m_strInquiryFunction + strParam;

	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
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
CFinnhubCryptosPtr CProductFinnhubCryptoSymbol::Parse(const string& text) {
	auto pvCryptoSymbol = make_shared<vector<CFinnhubCryptoPtr>>();
	string sError;
	nlohmannJson js;

	if (text.empty()) return pvCryptoSymbol;
	if (!::CreateJsonWithNlohmann(js, text)) return pvCryptoSymbol;
	if (::IsVoidJson(text)) return pvCryptoSymbol;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			auto pSymbol = make_shared<CFinnhubCrypto>();
			string s = jsonGetString(it, "description");
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
