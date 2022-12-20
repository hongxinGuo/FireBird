#include "pch.h"

#include"jsonParse.h"
#include"WorldMarket.h"

#include "ProductFinnhubForexExchange.h"

IMPLEMENT_DYNCREATE(CProductFinnhubForexExchange, CProductFinnhub)

CProductFinnhubForexExchange::CProductFinnhubForexExchange() {
	m_strClassName = _T("Finnhub forex exchange");
	m_strInquiry = _T("https://finnhub.io/api/v1/forex/exchange?");
	m_lIndex = -1;
}

CString CProductFinnhubForexExchange::CreateMessage(void) {
	ASSERT(m_strInquiringExchange.Compare(_T("ALL")) == 0);
	m_strInquiringExchange = _T("ALL"); // 申请无需交易所代码的数据时，将交易所代码设置为虚拟的ALL。
	m_strTotalInquiryMessage = m_strInquiry;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubForexExchange::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pvForexExchange = ParseFinnhubForexExchange(pWebData);
	for (int i = 0; i < pvForexExchange->size(); i++) {
		if (!dynamic_cast<CWorldMarket*>(m_pMarket)->IsForexExchange(pvForexExchange->at(i))) {
			dynamic_cast<CWorldMarket*>(m_pMarket)->AddForexExchange(pvForexExchange->at(i));
		}
	}

	return true;
}

shared_ptr<vector<CString>> CProductFinnhubForexExchange::ParseFinnhubForexExchange(CWebDataPtr pWebData) {
	auto pvExchange = make_shared<vector<CString>>();
	string s;
	CString str = _T("");
	string sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvExchange;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvExchange;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvExchange;
	}
	const auto pjs = pWebData->GetJSon();
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			s = it->get<string>();
			str = s.c_str();
			pvExchange->push_back(str);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Exchange "), e.what());
	}
	return pvExchange;
}
