#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubCompanyInsiderTransaction.h"

#include "TimeConvert.h"

using namespace std;

CProductFinnhubCompanyInsiderTransaction::CProductFinnhubCompanyInsiderTransaction() {
	m_strClassName = _T("Finnhub company insider transaction");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/insider-transactions?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyInsiderTransaction::CreateMessage() {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = dynamic_pointer_cast<CWorldMarket>(GetMarket())->GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol();
	return m_strInquiry;
}

bool CProductFinnhubCompanyInsiderTransaction::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = dynamic_pointer_cast<CWorldMarket>(GetMarket())->GetStock(m_lIndex);
	const CInsiderTransactionVectorPtr pvInsiderTransaction = ParseFinnhubStockInsiderTransaction(pWebData);
	pStock->SetInsiderTransactionUpdateDate(GetMarket()->GetMarketDate());
	pStock->SetUpdateInsiderTransaction(false);
	pStock->SetUpdateProfileDB(true);
	if (!pvInsiderTransaction->empty()) {
		pStock->UpdateInsiderTransaction(*pvInsiderTransaction);
		pStock->SetSaveInsiderTransaction(true);
	}

	return true;
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
CInsiderTransactionVectorPtr CProductFinnhubCompanyInsiderTransaction::ParseFinnhubStockInsiderTransaction(const CWebDataPtr& pWebData) {
	auto pvInsiderTransaction = make_shared<vector<CInsiderTransactionPtr>>();
	json pt1;
	string sError;
	string s;
	string stockSymbol;
	long year, month, day;
	CInsiderTransactionPtr pInsiderTransaction = nullptr;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvInsiderTransaction;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return pvInsiderTransaction;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return pvInsiderTransaction;
	}
	const auto pjs = pWebData->GetJSon();
	try {
		pt1 = jsonGetChild(pjs, _T("data"));
		stockSymbol = jsonGetString(pjs, _T("symbol"));
	}
	catch (json::exception& e) {
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
			sscanf_s(s.c_str(), _T("%4d-%2d-%2d"), &year, &month, &day);
			pInsiderTransaction->m_lFilingDate = XferYearMonthDayToYYYYMMDD(year, month, day);
			s = jsonGetString(it, _T("transactionDate"));
			sscanf_s(s.c_str(), _T("%4d-%2d-%2d"), &year, &month, &day);
			pInsiderTransaction->m_lTransactionDate = XferYearMonthDayToYYYYMMDD(year, month, day);
			s = jsonGetString(it, _T("transactionCode"));
			pInsiderTransaction->m_strTransactionCode = s.c_str();
			pInsiderTransaction->m_dTransactionPrice = jsonGetDouble(it, _T("transactionPrice"));
			pvInsiderTransaction->push_back(pInsiderTransaction);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock ") + pInsiderTransaction->m_strSymbol + _T(" Insider Transaction "), e.what());
		return pvInsiderTransaction;
	}
	ranges::sort(pvInsiderTransaction->begin(), pvInsiderTransaction->end(),
	             [](const CInsiderTransactionPtr& p1, const CInsiderTransactionPtr& p2) { return p1->m_lTransactionDate < p2->m_lTransactionDate; });
	return pvInsiderTransaction;
}
