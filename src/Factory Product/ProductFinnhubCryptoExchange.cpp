#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include "ProductFinnhubCryptoExchange.h"

#include "FinnhubDataSource.h"

CProductFinnhubCryptoExchange::CProductFinnhubCryptoExchange() {
	m_strClassName = _T("Finnhub crypto exchange");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/crypto/exchange?");
	m_lIndex = -1;
}

CString CProductFinnhubCryptoExchange::CreateMessage() {
	ASSERT(m_strInquiringExchange.Compare(_T("ALL")) == 0);
	m_strInquiringExchange = _T("ALL"); // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductFinnhubCryptoExchange::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvCryptoExchange = ParseFinnhubCryptoExchange(pWebData);
	for (int i = 0; i < pvCryptoExchange->size(); i++) {
		if (!gl_dataContainerFinnhubCryptoExchange.IsExchange(static_cast<LPCTSTR>(pvCryptoExchange->at(i)))) {
			gl_dataContainerFinnhubCryptoExchange.Add(static_cast<LPCTSTR>(pvCryptoExchange->at(i)));
		}
	}
}

//
//
// ["KRAKEN", "HITBTC", "COINBASE", "GEMINI", "POLONIEX", "Binance", "ZB", "BITTREX", "KUCOIN", "OKEX", "BITFINEX", "HUOBI"]
//
//
shared_ptr<vector<CString>> CProductFinnhubCryptoExchange::ParseFinnhubCryptoExchange(const CWebDataPtr& pWebData) {
	string s;
	CString str = _T("");
	string sError;
	auto pvExchange = make_shared<vector<CString>>();
	json js;

	if (!pWebData->CreateJson(js)) return pvExchange;
	if (!IsValidData(pWebData)) return pvExchange;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			s = jsonGetString(it);
			str = s.c_str();
			pvExchange->push_back(str);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Exchange "), e.what());
		return pvExchange;
	}
	return pvExchange;
}

void CProductFinnhubCryptoExchange::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CFinnhubDataSource")) == 0);
	dynamic_pointer_cast<CFinnhubDataSource>(pDataSource)->m_fUpdateCryptoExchange = false;
}
