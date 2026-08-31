#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"
#include"SystemMessage.h"

#include "ProductFinnhubCryptoExchange.h"

#include "ContainerFinnhubCryptoExchange.h"
#include "FinnhubDataSource.h"
#include"cpr/cpr.h"

using std::make_shared;

CProductFinnhubCryptoExchange::CProductFinnhubCryptoExchange() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/crypto/exchange?";
}

void CProductFinnhubCryptoExchange::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}
		const auto pvCryptoExchange = Parse(m_r.text);
		for (const auto& str : *pvCryptoExchange) {
			if (!gl_dataContainerFinnhubCryptoExchange.IsExchange(str)) {
				gl_dataContainerFinnhubCryptoExchange.Add(str);
			}
		}
	}
}

void CProductFinnhubCryptoExchange::WebStatusCheck(cpr::Response& r) {
	string s;
	switch (r.status_code) {
	case 0:
		break;
	case 302: //redirected, not an error
	case 403: // forbidden
		WebErrorReport();
		break;
	default:
		WebErrorReport();
		break;
	}
}

shared_ptr<vector<string>> CProductFinnhubCryptoExchange::CreateMessage() {
	ABSL_DCHECK(m_strInquiringExchange == "ALL");
	m_strInquiringExchange = "ALL"; // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	m_inquiryString = m_strInquiryFunction;
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

//
//
// ["KRAKEN", "HITBTC", "COINBASE", "GEMINI", "POLONIEX", "Binance", "ZB", "BITTREX", "KUCOIN", "OKEX", "BITFINEX", "HUOBI"]
//
//
shared_ptr<vector<string>> CProductFinnhubCryptoExchange::Parse(const string& text) {
	string sError;
	auto pvExchange = make_shared<vector<string>>();
	nlohmannJson js;

	if (text.empty()) return pvExchange;
	if (!::CreateJsonWithNlohmann(js, text)) return pvExchange;
	if (::IsVoidJson(text)) return pvExchange; // 即使为空，也完成了查询。

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			string s = jsonGetString(it);
			pvExchange->push_back(s);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Crypto Exchange ", e.what());
		return pvExchange;
	}
	return pvExchange;
}

void CProductFinnhubCryptoExchange::UpdateSystemStatus() {
	gl_pFinnhubDataSource->SetUpdateCryptoExchange(false);
	gl_systemMessage.PushInformationMessage("Finnhub crypto exchange updated");
}
