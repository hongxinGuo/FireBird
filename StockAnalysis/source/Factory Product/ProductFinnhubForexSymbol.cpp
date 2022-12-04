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

CString CProductFinnhubForexSymbol::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMiddle = ((CWorldMarket*)m_pMarket)->GetForexExchange(m_lIndex);

	m_strInquiringExchange = strMiddle;
	m_strTotalInquiryMessage = m_strInquiry + strMiddle;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubForexSymbol::ParseAndStoreWebData(CWebDataPtr pWebData, CVirtualDataSource* pDataSource) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CForexSymbolVectorPtr pvForexSymbol = nullptr;

	pvForexSymbol = ParseFinnhubForexSymbol(pWebData);
	if (pvForexSymbol->size() == 0) return false;
	for (auto& pSymbol : *pvForexSymbol) {
		if (!((CWorldMarket*)m_pMarket)->IsForexSymbol(pSymbol->GetSymbol())) {
			pSymbol->SetExchangeCode(((CWorldMarket*)m_pMarket)->GetForexExchange(m_lIndex));
			((CWorldMarket*)m_pMarket)->AddForexSymbol(pSymbol);
		}
	}

	return true;
}

CForexSymbolVectorPtr CProductFinnhubForexSymbol::ParseFinnhubForexSymbol(CWebDataPtr pWebData) {
	CForexSymbolVectorPtr pvForexSymbol = make_shared<vector<CForexSymbolPtr>>();
	CForexSymbolPtr pSymbol = nullptr;
	ptree pt2;
	string s;
	string sError;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvForexSymbol;
	if (pWebData->IsVoidJson()) { m_iReceivedDataStatus = __VOID_DATA__; return pvForexSymbol; }
	if (pWebData->CheckNoRightToAccess()) { m_iReceivedDataStatus = __NO_ACCESS_RIGHT__; return pvForexSymbol; }
	ppt = pWebData->GetPTree();
	try {
		for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
			pSymbol = make_shared<CFinnhubForexSymbol>();
			pt2 = it->second;
			s = pt2.get<string>(_T("description"));
			if (s.size() > 0) pSymbol->SetDescription(s.c_str());
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