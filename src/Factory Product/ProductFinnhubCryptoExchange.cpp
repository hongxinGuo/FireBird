module;
#include <absl/log/absl_check.h>
module FireBirdLib.Product.Finnhub.CryptoExchange;

import FireBirdLib.ContainerFinnhubCryptoExchange;
import FireBirdLib.Accessory.NlohmannJsonGetValue;
import FireBirdLib.Accessory.JsonParse;
import FireBirdLib.DataSource.Finnhub;
import FireBirdLib.SystemMessage;
import FireBirdLib.Accessory.NlohmannJsonDeclaration;

using namespace std;
using namespace std::chrono;

CProductFinnhubCryptoExchange::CProductFinnhubCryptoExchange() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/crypto/exchange?";
}

string CProductFinnhubCryptoExchange::CreateMessage() {
	ABSL_CHECK(m_strInquiringExchange == "ALL");
	m_strInquiringExchange = "ALL"; // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductFinnhubCryptoExchange::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvCryptoExchange = ParseFinnhubCryptoExchange(pWebData);
	for (const auto& str : *pvCryptoExchange) {
		if (!gl_dataContainerFinnhubCryptoExchange.IsExchange(str)) {
			gl_dataContainerFinnhubCryptoExchange.Add(str);
		}
	}
}

//
//
// ["KRAKEN", "HITBTC", "COINBASE", "GEMINI", "POLONIEX", "Binance", "ZB", "BITTREX", "KUCOIN", "OKEX", "BITFINEX", "HUOBI"]
//
//
shared_ptr<vector<string>> CProductFinnhubCryptoExchange::ParseFinnhubCryptoExchange(const CWebDataPtr& pWebData) {
	string sError;
	auto pvExchange = make_shared<vector<string>>();
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvExchange;
	if (!IsValidData(pWebData)) return pvExchange;

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
