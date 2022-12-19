#include "pch.h"

#include"jsonParse.h"
#include"WorldMarket.h"
#include"WorldStock.h"
#include"CallableFunction.h"

#include "ProductFinnhubCompanyInsiderSentiment.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyInsiderSentiment, CProductFinnhub)

CProductFinnhubCompanyInsiderSentiment::CProductFinnhubCompanyInsiderSentiment() {
	m_strClassName = _T("Finnhub company insider sentiment");
	m_strInquiry = _T("https://finnhub.io/api/v1/stock/insider-sentiment?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyInsiderSentiment::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));
	const long lCurrentDate = m_pMarket->GetMarketDate();
	const CWorldStockPtr pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	char buffer[100];

	sprintf_s(buffer, _T("%4d-%02d-%02d"), lCurrentDate / 10000, (lCurrentDate % 10000) / 100, lCurrentDate % 100);
	const CString strCurrentDate = buffer;
	m_strTotalInquiryMessage = m_strInquiry + pStock->GetSymbol() + _T("&from=1980-01-01&to=") + strCurrentDate;
	m_strInquiringExchange = pStock->GetExchangeCode();

	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCompanyInsiderSentiment::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CInsiderSentimentVectorPtr pvInsiderSentiment = nullptr;
	const CWorldStockPtr pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	pvInsiderSentiment = ParseFinnhubStockInsiderSentiment2(pWebData);
	pStock->SetInsiderSentimentUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
	pStock->SetInsiderSentimentNeedUpdate(false);
	pStock->SetUpdateProfileDB(true);
	if (!pvInsiderSentiment->empty()) {
		pStock->UpdateInsiderSentiment(*pvInsiderSentiment);
		pStock->SetInsiderSentimentNeedSave(true);
	}

	return true;
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
CInsiderSentimentVectorPtr CProductFinnhubCompanyInsiderSentiment::ParseFinnhubStockInsiderSentiment(CWebDataPtr pWebData) {
	auto pvInsiderSentiment = make_shared<vector<CInsiderSentimentPtr>>();
	ptree pt1, pt2;
	string sError;
	string s;
	string stockSymbol;
	CInsiderSentimentPtr pInsiderSentiment = nullptr;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvInsiderSentiment;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvInsiderSentiment;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvInsiderSentiment;
	}
	const auto ppt = pWebData->GetPTree();
	try {
		pt1 = ppt->get_child(_T("data"));
		stockSymbol = ppt->get<string>(_T("symbol"));
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Insider Sentiment ") + GetInquiry(), e);
		return pvInsiderSentiment;
	}

	try {
		for (ptree::iterator it = pt1.begin(); it != pt1.end(); ++it) {
			pInsiderSentiment = make_shared<CInsiderSentiment>();
			pInsiderSentiment->m_strSymbol = stockSymbol.c_str();
			pt2 = it->second;
			s = pt2.get<string>(_T("symbol"));
			if (!s.empty()) pInsiderSentiment->m_strSymbol = s.c_str();
			const long year = ptreeGetLongLong(pt2, _T("year"));
			const long month = ptreeGetLongLong(pt2, _T("month"));
			pInsiderSentiment->m_lDate = year * 10000 + month * 100 + 1; // 日期要有效，故而日子用每月的第一天
			pInsiderSentiment->m_lChange = ptreeGetLongLong(pt2, _T("change"));
			pInsiderSentiment->m_mspr = ptreeGetDouble(pt2, _T("mspr"));
			pvInsiderSentiment->push_back(pInsiderSentiment);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock ") + pInsiderSentiment->m_strSymbol + _T(" Insider Sentiment "),
		                               e);
		return pvInsiderSentiment;
	}
	ranges::sort(pvInsiderSentiment->begin(), pvInsiderSentiment->end(), CompareInsiderSentiment);
	return pvInsiderSentiment;
}

CInsiderSentimentVectorPtr CProductFinnhubCompanyInsiderSentiment::ParseFinnhubStockInsiderSentiment2(CWebDataPtr pWebData) {
	auto pvInsiderSentiment = make_shared<vector<CInsiderSentimentPtr>>();
	json pt1;
	string sError;
	string s;
	string stockSymbol;
	CInsiderSentimentPtr pInsiderSentiment = nullptr;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvInsiderSentiment;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvInsiderSentiment;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvInsiderSentiment;
	}
	const auto pjs = pWebData->GetJSon();
	try {
		jsonGetChild(pjs, _T("data"), &pt1);
		stockSymbol = jsonGetString(pjs, _T("symbol"));
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Insider Sentiment ") + GetInquiry(), e.what());
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
			pInsiderSentiment->m_lDate = year * 10000 + month * 100 + 1; // 日期要有效，故而日子用每月的第一天
			pInsiderSentiment->m_lChange = jsonGetLong(it,_T("change"));
			pInsiderSentiment->m_mspr = jsonGetDouble(it, _T("mspr"));
			pvInsiderSentiment->push_back(pInsiderSentiment);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock ") + pInsiderSentiment->m_strSymbol + _T(" Insider Sentiment "), e.what());
		return pvInsiderSentiment;
	}
	ranges::sort(pvInsiderSentiment->begin(), pvInsiderSentiment->end(), CompareInsiderSentiment);
	return pvInsiderSentiment;
}
