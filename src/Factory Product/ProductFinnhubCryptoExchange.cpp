#include "pch.h"

import FireBird.Accessory.JsonParse;
import FireBird.Accessory.JsonGetValue;

#include "ProductFinnhubCryptoExchange.h"

import FireBird.DataSource.Finnhub;

CProductFinnhubCryptoExchange::CProductFinnhubCryptoExchange() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/crypto/exchange?");
}

CString CProductFinnhubCryptoExchange::CreateMessage() {
	ASSERT(m_strInquiringExchange.Compare(_T("ALL")) == 0);
	m_strInquiringExchange = _T("ALL"); // �������轻�������������ʱ������������������Ϊ�����ALL��
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
	nlohmann::ordered_json js;

	if (!pWebData->CreateJson(js)) return pvExchange;
	if (!IsValidData(pWebData)) return pvExchange;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			s = jsonGetString(it);
			str = s.c_str();
			pvExchange->push_back(str);
		}
	} catch (nlohmann::ordered_json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Exchange "), e.what());
		return pvExchange;
	}
	return pvExchange;
}

void CProductFinnhubCryptoExchange::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CFinnhubDataSource")) == 0);
	dynamic_pointer_cast<CFinnhubDataSource>(pDataSource)->SetUpdateCryptoExchange(false);
}
