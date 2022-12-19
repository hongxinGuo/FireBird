#include "pch.h"

#include"jsonParse.h"

#include"WorldMarket.h"
#include"WorldStock.h"
#include"CallableFunction.h"

#include "ProductFinnhubCompanyInsiderTransaction.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyInsiderTransaction, CProductFinnhub)

CProductFinnhubCompanyInsiderTransaction::CProductFinnhubCompanyInsiderTransaction() {
	m_strClassName = _T("Finnhub company insider transaction");
	m_strInquiry = _T("https://finnhub.io/api/v1/stock/insider-transactions?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyInsiderTransaction::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + pStock->GetSymbol();
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCompanyInsiderTransaction::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CInsiderTransactionVectorPtr pvInsiderTransaction = nullptr;
	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	pvInsiderTransaction = ParseFinnhubStockInsiderTransaction2(pWebData);
	pStock->SetInsiderTransactionUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
	pStock->SetInsiderTransactionNeedUpdate(false);
	pStock->SetUpdateProfileDB(true);
	if (!pvInsiderTransaction->empty()) {
		pStock->UpdateInsiderTransaction(*pvInsiderTransaction);
		pStock->SetInsiderTransactionNeedSave(true);
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
CInsiderTransactionVectorPtr CProductFinnhubCompanyInsiderTransaction::ParseFinnhubStockInsiderTransaction(CWebDataPtr pWebData) {
	auto pvInsiderTransaction = make_shared<vector<CInsiderTransactionPtr>>();
	ptree pt1;
	string sError;
	string s;
	string stockSymbol;
	long year, month, day;
	CInsiderTransactionPtr pInsiderTransaction = nullptr;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvInsiderTransaction;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvInsiderTransaction;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvInsiderTransaction;
	}
	const auto ppt = pWebData->GetPTree();
	try {
		pt1 = ppt->get_child(_T("data"));
		stockSymbol = ppt->get<string>(_T("symbol"));
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Insider Transaction ") + GetInquiry(), e);
		return pvInsiderTransaction;
	}

	try {
		for (ptree::iterator it = pt1.begin(); it != pt1.end(); ++it) {
			pInsiderTransaction = make_shared<CInsiderTransaction>();
			pInsiderTransaction->m_strSymbol = stockSymbol.c_str();
			ptree pt2 = it->second;
			s = pt2.get<string>(_T("name"));
			if (!s.empty()) pInsiderTransaction->m_strPersonName = s.c_str();
			pInsiderTransaction->m_lShare = ptreeGetLongLong(pt2, _T("share"));
			pInsiderTransaction->m_lChange = ptreeGetLongLong(pt2, _T("change"));
			s = pt2.get<string>(_T("filingDate"));
			sscanf_s(s.c_str(), _T("%4d-%2d-%2d"), &year, &month, &day);
			pInsiderTransaction->m_lFilingDate = year * 10000 + month * 100 + day;
			s = pt2.get<string>(_T("transactionDate"));
			sscanf_s(s.c_str(), _T("%4d-%2d-%2d"), &year, &month, &day);
			pInsiderTransaction->m_lTransactionDate = year * 10000 + month * 100 + day;
			s = pt2.get<string>(_T("transactionCode"));
			pInsiderTransaction->m_strTransactionCode = s.c_str();
			pInsiderTransaction->m_dTransactionPrice = ptreeGetDouble(pt2, _T("transactionPrice"));
			pvInsiderTransaction->push_back(pInsiderTransaction);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock ") + pInsiderTransaction->m_strSymbol + _T(" Insider Transaction "), e);
		return pvInsiderTransaction;
	}
	ranges::sort(pvInsiderTransaction->begin(), pvInsiderTransaction->end(), CompareInsiderTransaction);
	return pvInsiderTransaction;
}

CInsiderTransactionVectorPtr CProductFinnhubCompanyInsiderTransaction::ParseFinnhubStockInsiderTransaction2(CWebDataPtr pWebData) {
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
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvInsiderTransaction;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvInsiderTransaction;
	}
	const auto pjs = pWebData->GetJSon();
	try {
		jsonGetChild(pjs, _T("data"), &pt1);
		stockSymbol = jsonGetString(pjs, _T("symbol"));
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Insider Transaction ") + GetInquiry(), e.what());
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
			pInsiderTransaction->m_lFilingDate = year * 10000 + month * 100 + day;
			s = jsonGetString(it, _T("transactionDate"));
			sscanf_s(s.c_str(), _T("%4d-%2d-%2d"), &year, &month, &day);
			pInsiderTransaction->m_lTransactionDate = year * 10000 + month * 100 + day;
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
	ranges::sort(pvInsiderTransaction->begin(), pvInsiderTransaction->end(), CompareInsiderTransaction);
	return pvInsiderTransaction;
}
