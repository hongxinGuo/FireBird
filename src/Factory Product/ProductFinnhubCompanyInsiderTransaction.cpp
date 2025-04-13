#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"

#include "ProductFinnhubCompanyInsiderTransaction.h"

#include "TimeConvert.h"
#include "WebData.h"

CProductFinnhubCompanyInsiderTransaction::CProductFinnhubCompanyInsiderTransaction() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/insider-transactions?symbol=");
}

CString CProductFinnhubCompanyInsiderTransaction::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pStock->GetFinnhubInsiderTransactionInquiryParam(GetUTCTime());
	return m_strInquiry;
}

void CProductFinnhubCompanyInsiderTransaction::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);
	const CInsiderTransactionsPtr pvInsiderTransaction = ParseFinnhubStockInsiderTransaction(pWebData);
	pStock->SetInsiderTransactionUpdateDate(GetMarket()->GetMarketDate());
	pStock->SetUpdateInsiderTransaction(false);
	pStock->SetUpdateProfileDB(true);
	if (!pvInsiderTransaction->empty()) {
		pStock->UpdateInsiderTransaction(*pvInsiderTransaction);
		pStock->SetUpdateInsiderTransactionDB(true);
	}
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
	auto pvInsiderTransaction = make_shared<vector<CInsiderTransactionPtr>>();
	json pt1;
	string sError;
	string s;
	string stockSymbol;
	CInsiderTransactionPtr pInsiderTransaction = nullptr;
	json js;

	if (!pWebData->CreateJson(js)) return pvInsiderTransaction;
	if (!IsValidData(pWebData)) return pvInsiderTransaction;

	try {
		pt1 = jsonGetChild(js, _T("data"));
		stockSymbol = jsonGetString(js, _T("symbol"));
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Insider Transaction ") + GetInquiryFunction(), e.what());
		return pvInsiderTransaction;
	}

	try {
		for (auto it = pt1.begin(); it != pt1.end(); ++it) {
			pInsiderTransaction = make_shared<CInsiderTransaction>();
			pInsiderTransaction->m_strSymbol = stockSymbol.c_str();
			s = jsonGetString(it, _T("name"));
			if (!s.empty()) pInsiderTransaction->m_strPersonName = s.c_str();
			pInsiderTransaction->m_lShare = jsonGetLongLong(it, _T("share"));
			pInsiderTransaction->m_lChange = jsonGetLongLong(it, _T("change"));
			s = jsonGetString(it, _T("filingDate"));
			pInsiderTransaction->m_lFilingDate = XferToYYYYMMDD(s);
			s = jsonGetString(it, _T("transactionDate"));
			pInsiderTransaction->m_lTransactionDate = XferToYYYYMMDD(s);
			s = jsonGetString(it, _T("transactionCode"));
			pInsiderTransaction->m_strTransactionCode = s.c_str();
			pInsiderTransaction->m_dTransactionPrice = jsonGetDouble(it, _T("transactionPrice"));
			pvInsiderTransaction->push_back(pInsiderTransaction);
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock ") + pInsiderTransaction->m_strSymbol + _T(" Insider Transaction "), e.what());
		std::ranges::sort(pvInsiderTransaction->begin(), pvInsiderTransaction->end(),
		                  [](const CInsiderTransactionPtr& p1, const CInsiderTransactionPtr& p2) { return p1->m_lTransactionDate < p2->m_lTransactionDate; });
		return pvInsiderTransaction;
	}
	std::ranges::sort(pvInsiderTransaction->begin(), pvInsiderTransaction->end(),
	                  [](const CInsiderTransactionPtr& p1, const CInsiderTransactionPtr& p2) { return p1->m_lTransactionDate < p2->m_lTransactionDate; });
	return pvInsiderTransaction;
}
