#include "pch.h"

#include"globedef.h"

#include "ProductFinnhubCryptoExchange.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCryptoExchange, CProductWebSourceData)

CProductFinnhubCryptoExchange::CProductFinnhubCryptoExchange() {
	m_strClassName = _T("Finnhub crypto exchange");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/crypto/exchange?");
	m_lIndex = -1;
}

CString CProductFinnhubCryptoExchange::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CProductFinnhubCryptoExchange::ProcessWebData(CWebDataPtr pWebData) {
	shared_ptr<vector<CString>> pvCryptoExchange = nullptr;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pvCryptoExchange = ParseFinnhubCryptoExchange(pWebData);
	for (int i = 0; i < pvCryptoExchange->size(); i++) {
		if (!((CWorldMarket*)m_pMarket)->IsCryptoExchange(pvCryptoExchange->at(i))) {
			((CWorldMarket*)m_pMarket)->AddCryptoExchange(pvCryptoExchange->at(i));
		}
	}
	((CWorldMarket*)m_pMarket)->SetFinnhubCryptoExchangeUpdated(true);

	return true;
}

shared_ptr<vector<CString>> CProductFinnhubCryptoExchange::ParseFinnhubCryptoExchange(CWebDataPtr pWebData) {
	ptree pt, pt2;
	string s;
	CString str = _T("");
	string sError;
	shared_ptr<vector<CString>> pvExchange = make_shared<vector<CString>>();

	if (!ConvertToJSON(pt, pWebData)) return pvExchange;

	try {
		for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
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