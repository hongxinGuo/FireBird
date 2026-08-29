#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"
#include"InsiderSentiment.h"

#include "ProductFinnhubCompanyInsiderSentiment.h"

#include "ContainerFinnhubStock.h"
#include "FinnhubDataSource.h"
#include "SystemMessage.h"

#include"cpr/cpr.h"

using std::make_shared;

CProductFinnhubCompanyInsiderSentiment::CProductFinnhubCompanyInsiderSentiment() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/insider-sentiment?symbol=";
}
void CProductFinnhubCompanyInsiderSentiment::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		cpr::Response r = cpr::Get(cpr::Url{ inquireString });
		m_statusCode = r.status_code;
		m_elapsed = r.elapsed;

		if (m_statusCode != 200) {
			WebStatusCheck(r);
			return;
		}
		CInsiderSentimentsPtr pvInsiderSentiment = Parse(r.text);
		if (!pvInsiderSentiment->empty()) {
			const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
			pStock->UpdateInsiderSentiment(pvInsiderSentiment);
			pStock->SetUpdateInsiderSentimentDB(true);
			pvInsiderSentiment = nullptr;
		}
	}
}

void CProductFinnhubCompanyInsiderSentiment::WebStatusCheck(cpr::Response& r) {
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
		string s = std::format("Finnhub company profile concise http error {}. code:{} message: {}", r.status_code,
		                       static_cast<int>(r.error.code), r.error.message);
		gl_systemMessage.PushInnerSystemInformationMessage(s);
		break;
	}
}

shared_ptr<vector<string>> CProductFinnhubCompanyInsiderSentiment::CreateMessage() {
	const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(m_index);

	const string sCurrentDate = ConvertDateToTimeStamp(gl_pWorldMarket->GetMarketDate());
	m_inquiryString = m_strInquiryFunction + pStock->GetSymbol() + "&from=1980-01-01&to=" + sCurrentDate;
	m_strInquiringExchange = pStock->GetExchange();

	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

void CProductFinnhubCompanyInsiderSentiment::UpdateSystemStatus() {
	const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
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
CInsiderSentimentsPtr CProductFinnhubCompanyInsiderSentiment::Parse(const string& text) {
	auto pvInsiderSentiment = make_shared<vector<CInsiderSentiment>>();
	pvInsiderSentiment->reserve(100);

	nlohmannJson pt1;
	string sError;
	string stockSymbol;
	CInsiderSentiment insiderSentiment;
	nlohmannJson js;

	if (text.empty()) return pvInsiderSentiment;
	if (!::CreateJsonWithNlohmann(js, text)) return pvInsiderSentiment;
	if (::IsVoidJson(text)) return pvInsiderSentiment; // 即使为空，也完成了查询。
	if (IsNoRightToAccess()) return pvInsiderSentiment;
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
