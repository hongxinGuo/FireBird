#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"WorldMarket.h"
#include "ContainerFinnhubStock.h"
#include"FinnhubStock.h"
#include "InsiderTransaction.h"

#include "ProductFinnhubCompanyInsiderTransaction.h"

#include "FinnhubDataSource.h"
#include "TimeConvert.h"
#include"cpr/cpr.h"

using namespace std;

CProductFinnhubCompanyInsiderTransaction::CProductFinnhubCompanyInsiderTransaction() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/insider-transactions?symbol=";
}

void CProductFinnhubCompanyInsiderTransaction::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		m_r = cpr::Get(cpr::Url{ inquireString });

		if (m_r.status_code != 200) {
			WebStatusCheck(m_r);
			return;
		}

		const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
		CInsiderTransactionsPtr pvInsiderTransaction = Parse(m_r.text);
		pStock->SetInsiderTransactionUpdateDate(gl_pWorldMarket->GetMarketDate());
		pStock->SetUpdateInsiderTransaction(false);
		pStock->SetUpdateProfileDB(true);
		if (!pvInsiderTransaction->empty()) {
			pStock->UpdateInsiderTransaction(pvInsiderTransaction);
			pStock->SetUpdateInsiderTransactionDB(true);
			pvInsiderTransaction = nullptr;
		}
	}
}

void CProductFinnhubCompanyInsiderTransaction::WebStatusCheck(cpr::Response& r) {
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

shared_ptr<vector<string>> CProductFinnhubCompanyInsiderTransaction::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);

	m_strInquiringExchange = pStock->GetExchange();
	m_inquiryString = m_strInquiryFunction + pStock->GetFinnhubInsiderTransactionInquiryParam(GetUTCTime());
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

void CProductFinnhubCompanyInsiderTransaction::UpdateSystemStatus() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
	pStock->SetInsiderTransactionUpdateDate(gl_pWorldMarket->GetMarketDate());
	pStock->SetUpdateInsiderTransaction(false);
	pStock->SetUpdateProfileDB(true);
}

//
//
// {"data": [{
//      "name": "Baglino Andrew D",
//      "share": 20614,
//      "change": 1000,
//      "filingDate": "2021-03-31",
//      "transactionDate": "2021-03-29",
//      "transactionCode": "M",
//      "transactionPrice": 41.57
//    },
//    {
//      "name": "Baglino Andrew D",
//      "share": 19114,
//      "change": -1500,
//      "filingDate": "2021-03-31",
//      "transactionDate": "2021-03-29",
//      "transactionCode": "S",
//      "transactionPrice": 615.75
//    }],
//  "symbol": "TSLA"
//  }
//
//
CInsiderTransactionsPtr CProductFinnhubCompanyInsiderTransaction::Parse(const string& text) {
	auto pvInsiderTransaction = make_shared<vector<CInsiderTransaction>>();
	pvInsiderTransaction->reserve(500);

	nlohmannJson pt1;
	string sError;
	string stockSymbol;
	CInsiderTransaction insiderTransaction;
	nlohmannJson js;

	if (text.empty()) return pvInsiderTransaction;
	if (!::CreateJsonWithNlohmann(js, text)) return pvInsiderTransaction;
	if (::IsVoidJson(text)) return pvInsiderTransaction; // 即使为空，也完成了查询。
	if (IsNoRightToAccess()) return pvInsiderTransaction;

	try {
		pt1 = jsonGetChild(js, "data");
		stockSymbol = jsonGetString(js, "symbol");
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Stock Insider Transaction " + GetInquiryFunction(), e.what());
		return pvInsiderTransaction;
	}

	try {
		string s;
		for (auto it = pt1.begin(); it != pt1.end(); ++it) {
			insiderTransaction.m_strSymbol = stockSymbol;
			s = jsonGetString(it, "name");
			if (!s.empty()) insiderTransaction.m_strPersonName = s;
			insiderTransaction.m_lShare = jsonGetLongLong(it, "share");
			insiderTransaction.m_lShareChange = jsonGetLongLong(it, "change");
			s = jsonGetString(it, "filingDate");
			istringstream ss1(s);
			chrono::local_days ld1;
			ss1 >> chrono::parse("%F", ld1);
			insiderTransaction.m_lFilingDate = toFormattedDate(ld1);
			s = jsonGetString(it, "transactionDate");
			istringstream ss2(s);
			chrono::local_days ld2;
			ss2 >> chrono::parse("%F", ld2);
			insiderTransaction.m_lTransactionDate = toFormattedDate(ld2);
			s = jsonGetString(it, "transactionCode");
			insiderTransaction.m_strTransactionCode = s;
			insiderTransaction.m_dTransactionPrice = jsonGetDouble(it, "transactionPrice");
			pvInsiderTransaction->push_back(insiderTransaction);
		}
	} catch (nlohmannJson::exception& e) {
		string str = "Finnhub Stock ";
		str += insiderTransaction.m_strSymbol;
		str += " Insider Transaction ";
		ReportJSonErrorToSystemMessage(str, e.what());
		std::ranges::sort(pvInsiderTransaction->begin(), pvInsiderTransaction->end(),
		                  [](const CInsiderTransaction& p1, const CInsiderTransaction& p2) { return p1.m_lTransactionDate < p2.m_lTransactionDate; });
		return pvInsiderTransaction;
	}
	std::ranges::sort(pvInsiderTransaction->begin(), pvInsiderTransaction->end(),
	                  [](const CInsiderTransaction& p1, const CInsiderTransaction& p2) { return p1.m_lTransactionDate < p2.m_lTransactionDate; });
	return pvInsiderTransaction;
}
