#include "pch.h"

#include"jsonParse.h"
#include"StockCodeConverter.h"
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

CString CProductFinnhubCompanyInsiderSentiment::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));
	long lCurrentDate = ((CWorldMarket*)m_pMarket)->GetMarketDate();
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	char buffer[100];
	CString strCurrentDate;

	sprintf_s(buffer, _T("%4d-%02d-%02d"), lCurrentDate / 10000, (lCurrentDate % 10000) / 100, lCurrentDate % 100);
	strCurrentDate = buffer;
	m_strTotalInquiryMessage = m_strInquiry + pStock->GetSymbol() + _T("&from=1980-01-01&to=") + strCurrentDate;
	m_strInquiringExchange = pStock->GetExchangeCode();

	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCompanyInsiderSentiment::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CInsiderSentimentVectorPtr pvInsiderSentiment = nullptr;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	pvInsiderSentiment = ParseFinnhubStockInsiderSentiment(pWebData);
	pStock->SetInsiderSentimentUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
	pStock->SetInsiderSentimentNeedUpdate(false);
	pStock->SetUpdateProfileDB(true);
	if (pvInsiderSentiment->size() > 0) {
		pStock->UpdateInsiderSentiment(*pvInsiderSentiment);
		pStock->SetInsiderSentimentNeedSave(true);
	}

	return true;
}

CInsiderSentimentVectorPtr CProductFinnhubCompanyInsiderSentiment::ParseFinnhubStockInsiderSentiment(CWebDataPtr pWebData) {
	CInsiderSentimentVectorPtr pvInsiderSentiment = make_shared<vector<CInsiderSentimentPtr>>();
	ptree pt1, pt2;
	string sError;
	string s;
	string stockSymbol;
	long year, month, day;
	CInsiderSentimentPtr pInsiderSentiment = nullptr;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvInsiderSentiment;
	if (pWebData->IsVoidJson()) { m_iReceivedDataStatus = __VOID_DATA__; return pvInsiderSentiment; }
	if (pWebData->NoRightToAccess()) { m_iReceivedDataStatus = __NO_ACCESS_RIGHT__; return pvInsiderSentiment; }
	ppt = pWebData->GetPTree();
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
			if (s.size() > 0) pInsiderSentiment->m_strSymbol = s.c_str();
			year = ptreeGetLongLong(pt2, _T("year"));
			month = ptreeGetLongLong(pt2, _T("month"));
			pInsiderSentiment->m_lDate = year * 10000 + month * 100 + 1; // 日期要有效，故而日子用每月的第一天
			pInsiderSentiment->m_lChange = ptreeGetLongLong(pt2, _T("change"));
			pInsiderSentiment->m_mspr = ptreeGetDouble(pt2, _T("mspr"));
			pvInsiderSentiment->push_back(pInsiderSentiment);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock ") + pInsiderSentiment->m_strSymbol + _T(" Insider Sentiment "), e);
		return pvInsiderSentiment;
	}
	sort(pvInsiderSentiment->begin(), pvInsiderSentiment->end(), CompareInsiderSentiment);
	return pvInsiderSentiment;
}