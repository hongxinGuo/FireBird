#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"

#include "ProductFinnhubCompanyInsiderTransaction.h"

#include "TimeConvert.h"
#include "WebData.h"

CProductFinnhubCompanyInsiderTransaction::CProductFinnhubCompanyInsiderTransaction() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/insider-transactions?symbol=";
}

string CProductFinnhubCompanyInsiderTransaction::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pStock->GetFinnhubInsiderTransactionInquiryParam(GetUTCTime());
	return m_strInquiry;
}

void CProductFinnhubCompanyInsiderTransaction::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	CInsiderTransactionsPtr pvInsiderTransaction = ParseFinnhubStockInsiderTransaction(pWebData);
	pStock->SetInsiderTransactionUpdateDate(gl_pWorldMarket->GetMarketDate());
	pStock->SetUpdateInsiderTransaction(false);
	pStock->SetUpdateProfileDB(true);
	if (!pvInsiderTransaction->empty()) {
		pStock->UpdateInsiderTransaction(pvInsiderTransaction);
		pStock->SetUpdateInsiderTransactionDB(true);
		pvInsiderTransaction = nullptr;
	}
}

void CProductFinnhubCompanyInsiderTransaction::UpdateSystemStatus() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
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
CInsiderTransactionsPtr CProductFinnhubCompanyInsiderTransaction::ParseFinnhubStockInsiderTransaction(const CWebDataPtr& pWebData) {
	auto pvInsiderTransaction = make_shared<vector<CInsiderTransaction>>();
	pvInsiderTransaction->reserve(500);

	nlohmannJson pt1;
	string sError;
	string stockSymbol;
	CInsiderTransaction insiderTransaction;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvInsiderTransaction;
	if (!IsValidData(pWebData)) return pvInsiderTransaction;

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
			insiderTransaction.m_lFilingDate = XferToYYYYMMDD(s);
			s = jsonGetString(it, "transactionDate");
			insiderTransaction.m_lTransactionDate = XferToYYYYMMDD(s);
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
