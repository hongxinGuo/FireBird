#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"

#include "ProductFinnhubCompanyInsiderSentiment.h"

#include "WebData.h"

CProductFinnhubCompanyInsiderSentiment::CProductFinnhubCompanyInsiderSentiment() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/insider-sentiment?symbol=";
}

string CProductFinnhubCompanyInsiderSentiment::CreateMessage() {
	const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);

	const string sCurrentDate = ConvertDateToTimeStamp(gl_pWorldMarket->GetMarketDate());
	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol() + "&from=1980-01-01&to=" + sCurrentDate;
	m_strInquiringExchange = pStock->GetExchange();

	return m_strInquiry;
}

void CProductFinnhubCompanyInsiderSentiment::ParseAndStoreWebData(CWebDataPtr pWebData) {
	CInsiderSentimentsPtr pvInsiderSentiment = ParseFinnhubStockInsiderSentiment(pWebData);
	if (!pvInsiderSentiment->empty()) {
		const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
		pStock->UpdateInsiderSentiment(pvInsiderSentiment);
		pStock->SetUpdateInsiderSentimentDB(true);
		pvInsiderSentiment = nullptr;
	}
}

void CProductFinnhubCompanyInsiderSentiment::UpdateSystemStatus() {
	const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	pStock->SetInsiderSentimentUpdateDate(gl_pWorldMarket->GetMarketDate());
	pStock->SetUpdateInsiderSentiment(false);
	pStock->SetUpdateProfileDB(true);
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
	auto pvInsiderSentiment = make_shared<vector<CInsiderSentiment>>();
	pvInsiderSentiment->reserve(100);

	nlohmannJson pt1;
	string sError;
	string stockSymbol;
	CInsiderSentiment insiderSentiment;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvInsiderSentiment;
	if (!IsValidData(pWebData)) return pvInsiderSentiment;

	try {
		pt1 = jsonGetChild(js, "data");
		stockSymbol = jsonGetString(js, "symbol");
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Stock Insider Sentiment " + GetInquiryFunction(), e.what());
		return pvInsiderSentiment;
	}

	try {
		string s;
		for (auto it = pt1.begin(); it != pt1.end(); ++it) {
			insiderSentiment.m_strSymbol = stockSymbol;
			s = jsonGetString(it, "symbol");
			if (!s.empty()) insiderSentiment.m_strSymbol = s;
			const long year = jsonGetLong(it, "year");
			const long month = jsonGetLong(it, "month");
			insiderSentiment.m_lDate = year * 10000 + month * 100 + 1; // 日期要有效，故而使用每月的第一天
			insiderSentiment.m_lChange = jsonGetLong(it, "change");
			insiderSentiment.m_mspr = jsonGetDouble(it, "mspr");
			pvInsiderSentiment->emplace_back(insiderSentiment);
		}
	} catch (nlohmannJson::exception& e) {
		string str = "Finnhub Stock ";
		str += insiderSentiment.m_strSymbol;
		str += " Insider Sentiment ";
		ReportJSonErrorToSystemMessage(str, e.what());
		return pvInsiderSentiment;
	}
	std::ranges::sort(pvInsiderSentiment->begin(), pvInsiderSentiment->end(),
	                  [](const CInsiderSentiment& p1, const CInsiderSentiment& p2) { return p1.m_lDate < p2.m_lDate; });
	return pvInsiderSentiment;
}
