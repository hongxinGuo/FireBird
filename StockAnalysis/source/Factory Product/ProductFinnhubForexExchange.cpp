#include "pch.h"

#include"jsonParse.h"
#include"WorldMarket.h"

#include "ProductFinnhubForexExchange.h"

IMPLEMENT_DYNCREATE(CProductFinnhubForexExchange, CProductFinnhub)

CProductFinnhubForexExchange::CProductFinnhubForexExchange() {
	m_strClassName = _T("Finnhub forex exchange");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/forex/exchange?");
	m_lIndex = -1;
}

CString CProductFinnhubForexExchange::CreatMessage(void) {
	m_strTotalInquiryMessage = m_strInquiringStr;
	return m_strTotalInquiryMessage;
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
	if (!pWebData->IsParsed()) return pvExchange;
	if (pWebData->IsVoidJSon()) { m_iReceivedDataStatus = __VOID_DATA__; return pvExchange; }
	if (pWebData->NoRightToAccess()) { m_iReceivedDataStatus = __NO_ACCESS_RIGHT__; return pvExchange; }
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
	return pvExchange;
}