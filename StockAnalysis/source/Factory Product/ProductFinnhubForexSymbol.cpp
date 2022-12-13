#include "pch.h"

#include"jsonParse.h"
#include"WorldMarket.h"

#include "ProductFinnhubForexSymbol.h"

IMPLEMENT_DYNCREATE(CProductFinnhubForexSymbol, CProductFinnhub)

CProductFinnhubForexSymbol::CProductFinnhubForexSymbol() {
	m_strClassName = _T("Finnhub forex symbols");
	m_strInquiry = _T("https://finnhub.io/api/v1/forex/symbol?exchange=");
	m_lIndex = -1;
}

CString CProductFinnhubForexSymbol::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto strMiddle = dynamic_cast<CWorldMarket*>(m_pMarket)->GetForexExchange(m_lIndex);

	m_strInquiringExchange = strMiddle;
	m_strTotalInquiryMessage = m_strInquiry + strMiddle;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubForexSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pvForexSymbol = ParseFinnhubForexSymbol(pWebData);
	if (pvForexSymbol->empty()) return false;
	for (const auto& pSymbol : *pvForexSymbol) {
		if (!dynamic_cast<CWorldMarket*>(m_pMarket)->IsForexSymbol(pSymbol->GetSymbol())) {
			pSymbol->SetExchangeCode(dynamic_cast<CWorldMarket*>(m_pMarket)->GetForexExchange(m_lIndex));
			dynamic_cast<CWorldMarket*>(m_pMarket)->AddForexSymbol(pSymbol);
		}
	}

	return true;
}

CForexSymbolVectorPtr CProductFinnhubForexSymbol::ParseFinnhubForexSymbol(CWebDataPtr pWebData) {
	auto pvForexSymbol = make_shared<vector<CForexSymbolPtr>>();
	CForexSymbolPtr pSymbol = nullptr;
	string s;
	string sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvForexSymbol;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvForexSymbol;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvForexSymbol;
	}
	const auto ppt = pWebData->GetPTree();
	try {
		for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
			pSymbol = make_shared<CFinnhubForexSymbol>();
			ptree pt2 = it->second;
			s = pt2.get<string>(_T("description"));
			if (!s.empty()) pSymbol->SetDescription(s.c_str());
			s = pt2.get<string>(_T("displaySymbol"));
			pSymbol->SetDisplaySymbol(s.c_str());
			s = pt2.get<string>(_T("symbol"));
			pSymbol->SetSymbol(s.c_str());
			pvForexSymbol->push_back(pSymbol);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Symbol "), e);
	}

	return pvForexSymbol;
}
