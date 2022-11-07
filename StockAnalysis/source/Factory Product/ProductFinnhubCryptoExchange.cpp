#include "pch.h"

#include"jsonParse.h"
#include"WorldMarket.h"

#include "ProductFinnhubCryptoExchange.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCryptoExchange, CProductFinnhub)

CProductFinnhubCryptoExchange::CProductFinnhubCryptoExchange() {
	m_strClassName = _T("Finnhub crypto exchange");
	m_strInquiry = _T("https://finnhub.io/api/v1/crypto/exchange?");
	m_lIndex = -1;
}

CString CProductFinnhubCryptoExchange::CreatMessage(void) {
	ASSERT(m_strInquiringExchange.Compare(_T("ALL")) == 0);
	m_strInquiringExchange = _T("ALL"); // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	m_strTotalInquiryMessage = m_strInquiry;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCryptoExchange::ParseAndStoreWebData(CWebDataPtr pWebData) {
	shared_ptr<vector<CString>> pvCryptoExchange = nullptr;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pvCryptoExchange = ParseFinnhubCryptoExchange(pWebData);
	for (int i = 0; i < pvCryptoExchange->size(); i++) {
		if (!((CWorldMarket*)m_pMarket)->IsCryptoExchange(pvCryptoExchange->at(i))) {
			((CWorldMarket*)m_pMarket)->AddCryptoExchange(pvCryptoExchange->at(i));
		}
	}

	return true;
}

shared_ptr<vector<CString>> CProductFinnhubCryptoExchange::ParseFinnhubCryptoExchange(CWebDataPtr pWebData) {
	ptree pt2;
	string s;
	CString str = _T("");
	string sError;
	shared_ptr<vector<CString>> pvExchange = make_shared<vector<CString>>();
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvExchange;
	if (pWebData->IsVoidJson()) { m_iReceivedDataStatus = __VOID_DATA__; return pvExchange; }
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
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Exchange "), e);
		return pvExchange;
	}
	return pvExchange;
}