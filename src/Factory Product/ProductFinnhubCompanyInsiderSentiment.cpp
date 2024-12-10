#include "pch.h"

import FireBird.Accessory.TimeConvert;
import FireBird.Accessory.JsonParse;
import FireBird.Accessory.JsonGetValue;

#include"WorldMarket.h"
#include"FinnhubStock.h"

#include "ProductFinnhubCompanyInsiderSentiment.h"

CProductFinnhubCompanyInsiderSentiment::CProductFinnhubCompanyInsiderSentiment() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/insider-sentiment?symbol=");
}

CString CProductFinnhubCompanyInsiderSentiment::CreateMessage() {
	const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);

	const CString strCurrentDate = ConvertDateToTimeStamp(GetMarket()->GetMarketDate());
	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol() + _T("&from=1980-01-01&to=") + strCurrentDate;
	m_strInquiringExchange = pStock->GetExchangeCode();

	return m_strInquiry;
}

void CProductFinnhubCompanyInsiderSentiment::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);
	const CInsiderSentimentsPtr pvInsiderSentiment = ParseFinnhubStockInsiderSentiment(pWebData);
	pStock->SetInsiderSentimentUpdateDate(GetMarket()->GetMarketDate());
	pStock->SetUpdateInsiderSentiment(false);
	pStock->SetUpdateProfileDB(true);
	if (!pvInsiderSentiment->empty()) {
		pStock->UpdateInsiderSentiment(*pvInsiderSentiment);
		pStock->SetUpdateInsiderSentimentDB(true);
	}
}

////////
// {"data": [{
//      "name": "Kirkhorn Zachary",
//      "share": 57234,
//      "change": -1250,
//      "filingDate": "2021-03-19",
//      "transactionDate": "2021-03-17",
//      "transactionCode": "S",
//      "transactionPrice": 655.81
//    },
//    {
//      "name": "Baglino Andrew D",
//      "share": 20614,
//      "change": 1000,
//      "filingDate": "2021-03-31",
//      "transactionDate": "2021-03-29",
//      "transactionCode": "M",
//      "transactionPrice": 41.57
//    }
//  ],
//  "symbol": "TSLA"}
//
CInsiderSentimentsPtr CProductFinnhubCompanyInsiderSentiment::ParseFinnhubStockInsiderSentiment(const CWebDataPtr& pWebData) {
	auto pvInsiderSentiment = make_shared<vector<CInsiderSentimentPtr>>();
	nlohmann::ordered_json pt1;
	string sError;
	string s;
	string stockSymbol;
	CInsiderSentimentPtr pInsiderSentiment = nullptr;
	nlohmann::ordered_json js;

	if (!pWebData->CreateJson(js)) return pvInsiderSentiment;
	if (!IsValidData(pWebData)) return pvInsiderSentiment;

	try {
		pt1 = jsonGetChild(js, _T("data"));
		stockSymbol = jsonGetString(js, _T("symbol"));
	} catch (nlohmann::ordered_json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Insider Sentiment ") + GetInquiryFunction(), e.what());
		return pvInsiderSentiment;
	}

	try {
		for (auto it = pt1.begin(); it != pt1.end(); ++it) {
			pInsiderSentiment = make_shared<CInsiderSentiment>();
			pInsiderSentiment->m_strSymbol = stockSymbol.c_str();
			s = jsonGetString(it, _T("symbol"));
			if (!s.empty()) pInsiderSentiment->m_strSymbol = s.c_str();
			const long year = jsonGetLong(it,_T("year"));
			const long month = jsonGetLong(it,_T("month"));
			pInsiderSentiment->m_lDate = XferYearMonthDayToYYYYMMDD(year, month, 1); // 日期要有效，故而使用每月的第一天
			pInsiderSentiment->m_lChange = jsonGetLong(it,_T("change"));
			pInsiderSentiment->m_mspr = jsonGetDouble(it, _T("mspr"));
			pvInsiderSentiment->push_back(pInsiderSentiment);
		}
	} catch (nlohmann::ordered_json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock ") + pInsiderSentiment->m_strSymbol + _T(" Insider Sentiment "), e.what());
		return pvInsiderSentiment;
	}
	std::ranges::sort(pvInsiderSentiment->begin(), pvInsiderSentiment->end(),
	                  [](const CInsiderSentimentPtr& p1, const CInsiderSentimentPtr& p2) { return p1->m_lDate < p2->m_lDate; });
	return pvInsiderSentiment;
}
