#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"WorldStock.h"
#include"CallableFunction.h"

#include "ProductFinnhubCompanyInsiderTransaction.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyInsiderTransaction, CProductWebSourceData)

CProductFinnhubCompanyInsiderTransaction::CProductFinnhubCompanyInsiderTransaction() {
	m_strClassName = _T("Finnhub company insider transaction");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/insider-transactions?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyInsiderTransaction::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetInsiderTransactionNeedUpdate(false);

	return strMessage;
}

bool CProductFinnhubCompanyInsiderTransaction::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CInsiderTransactionVectorPtr pvInsiderTransaction = nullptr;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	pvInsiderTransaction = ParseFinnhubStockInsiderTransaction(pWebData);
	pStock->SetInsiderTransactionUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
	pStock->SetUpdateProfileDB(true);
	if (pvInsiderTransaction->size() > 0) {
		pStock->UpdateInsiderTransaction(*pvInsiderTransaction);
		pStock->SetInsiderTransactionNeedSave(true);
	}

	return true;
}

CInsiderTransactionVectorPtr CProductFinnhubCompanyInsiderTransaction::ParseFinnhubStockInsiderTransaction(CWebDataPtr pWebData) {
	CInsiderTransactionVectorPtr pvInsiderTransaction = make_shared<vector<CInsiderTransactionPtr>>();
	ptree pt, pt1, pt2;
	string sError;
	string s;
	string stockSymbol;
	long year, month, day;
	CInsiderTransactionPtr pInsiderTransaction = nullptr;

	if (!ConvertToJSON(pt, pWebData)) return pvInsiderTransaction;

	try {
		pt1 = pt.get_child(_T("data"));
		stockSymbol = pt.get<string>(_T("symbol"));
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Insider Transaction "), e);
		return pvInsiderTransaction;
	}

	try {
		for (ptree::iterator it = pt1.begin(); it != pt1.end(); ++it) {
			pInsiderTransaction = make_shared<CInsiderTransaction>();
			pInsiderTransaction->m_strSymbol = stockSymbol.c_str();
			pt2 = it->second;
			s = pt2.get<string>(_T("name"));
			if (s.size() > 0) pInsiderTransaction->m_strPersonName = s.c_str();
			pInsiderTransaction->m_lShare = pt2.get<INT64>(_T("share"));
			pInsiderTransaction->m_lChange = pt2.get<INT64>(_T("change"));
			s = pt2.get<string>(_T("filingDate"));
			sscanf_s(s.c_str(), _T("%4d-%2d-%2d"), &year, &month, &day);
			pInsiderTransaction->m_lFilingDate = year * 10000 + month * 100 + day;
			s = pt2.get<string>(_T("transactionDate"));
			sscanf_s(s.c_str(), _T("%4d-%2d-%2d"), &year, &month, &day);
			pInsiderTransaction->m_lTransactionDate = year * 10000 + month * 100 + day;
			s = pt2.get<string>(_T("transactionCode"));
			pInsiderTransaction->m_strTransactionCode = s.c_str();
			pInsiderTransaction->m_dTransactionPrice = pt2.get<double>(_T("transactionPrice"));
			pvInsiderTransaction->push_back(pInsiderTransaction);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock ") + pInsiderTransaction->m_strSymbol + _T(" Insider Transaction "), e);
		return pvInsiderTransaction;
	}
	sort(pvInsiderTransaction->begin(), pvInsiderTransaction->end(), CompareInsiderTransaction);

	return pvInsiderTransaction;
}