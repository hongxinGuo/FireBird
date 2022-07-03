#include "pch.h"

#include"jsonParse.h"
#include"WorldMarket.h"

#include "ProductFinnhubForexExchange.h"

IMPLEMENT_DYNCREATE(CProductFinnhubForexExchange, CProductWebSourceData)

CProductFinnhubForexExchange::CProductFinnhubForexExchange() {
	m_strClassName = _T("Finnhub forex exchange");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/forex/exchange?");
	m_lIndex = -1;
}

CString CProductFinnhubForexExchange::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CProductFinnhubForexExchange::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	shared_ptr<vector<CString>> pvForexExchange = nullptr;

	pvForexExchange = ParseFinnhubForexExchange(pWebData);
	for (int i = 0; i < pvForexExchange->size(); i++) {
		if (!((CWorldMarket*)m_pMarket)->IsForexExchange(pvForexExchange->at(i))) {
			((CWorldMarket*)m_pMarket)->AddForexExchange(pvForexExchange->at(i));
		}
	}

	return true;
}

shared_ptr<vector<CString>> CProductFinnhubForexExchange::ParseFinnhubForexExchange(CWebDataPtr pWebData) {
	shared_ptr<vector<CString>> pvExchange = make_shared<vector<CString>>();
	ptree pt2;
	string s;
	CString str = _T("");
	string sError;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (pWebData->IsJSonContentType() && pWebData->IsParsed()) {
		if (pWebData->IsVoidJSon()) return pvExchange;
		ppt = pWebData->GetPTree();
		try {
			for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
				pt2 = it->second;
				s = pt2.get_value<string>();
				str = s.c_str();
				pvExchange->push_back(str);
			}
		}
		catch (ptree_error& e) {
			ReportJSonErrorToSystemMessage(_T("Finnhub Forex Exchange "), e);
		}
	}
	return pvExchange;
}