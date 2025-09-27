#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include "ProductFinnhubCryptoExchange.h"

#include "FinnhubDataSource.h"
#include "WebData.h"

CProductFinnhubCryptoExchange::CProductFinnhubCryptoExchange() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/crypto/exchange?");
}

string CProductFinnhubCryptoExchange::CreateMessage() {
	ASSERT(m_strInquiringExchange.compare(_T("ALL")) == 0);
	m_strInquiringExchange = _T("ALL"); // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductFinnhubCryptoExchange::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvCryptoExchange = ParseFinnhubCryptoExchange(pWebData);
	for (int i = 0; i < pvCryptoExchange->size(); i++) {
		if (!gl_dataContainerFinnhubCryptoExchange.IsExchange(pvCryptoExchange->at(i))) {
			gl_dataContainerFinnhubCryptoExchange.Add(pvCryptoExchange->at(i));
		}
	}
}

//
//
// ["KRAKEN", "HITBTC", "COINBASE", "GEMINI", "POLONIEX", "Binance", "ZB", "BITTREX", "KUCOIN", "OKEX", "BITFINEX", "HUOBI"]
//
//
shared_ptr<vector<string>> CProductFinnhubCryptoExchange::ParseFinnhubCryptoExchange(const CWebDataPtr& pWebData) {
	string s;
	string sError;
	auto pvExchange = make_shared<vector<string>>();
	json js;

	if (!pWebData->CreateJson(js)) return pvExchange;
	if (!IsValidData(pWebData)) return pvExchange;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			s = jsonGetString(it);
			pvExchange->push_back(s);
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Exchange "), e.what());
		return pvExchange;
	}
	return pvExchange;
}

void CProductFinnhubCryptoExchange::UpdateSystemStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CFinnhubDataSource")) == 0);
	dynamic_pointer_cast<CFinnhubDataSource>(pDataSource)->SetUpdateCryptoExchange(false);
	gl_systemMessage.PushInformationMessage(_T("Finnhub crypto exchange updated"));
}
