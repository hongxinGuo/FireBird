#include "pch.h"

#include"globedef.h"

#include "ProductFinnhubStockSymbol.h"

IMPLEMENT_DYNCREATE(CProductFinnhubStockSymbol, CProductWebSourceData)

CProductFinnhubStockSymbol::CProductFinnhubStockSymbol() {
	m_strClassName = _T("Finnhub company symbols");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/symbol?exchange=");
	m_lIndex = -1;
}

CString CProductFinnhubStockSymbol::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMiddle = ((CWorldMarket*)m_pMarket)->GetStockExchangeCode(m_lIndex);

	return m_strInquiringStr + strMiddle;
}

bool CProductFinnhubStockSymbol::ProcessWebData(CWebDataPtr pWebData) {
	CString str;
	CWorldStockVectorPtr  pvStock = nullptr;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pvStock = ParseFinnhubStockSymbol(pWebData);
	// ���Ͻ��������롣
	for (auto& pStock3 : *pvStock) {
		pStock3->SetExchangeCode(((CWorldMarket*)m_pMarket)->GetStockExchangeCode(m_lIndex));
	}
	for (auto& pStock2 : *pvStock) {
		if (!((CWorldMarket*)m_pMarket)->IsStock(pStock2->GetSymbol())) {
			((CWorldMarket*)m_pMarket)->AddStock(pStock2);
			str = _T("Finnhub�����´���:") + pStock2->GetSymbol();
			gl_systemMessage.PushInnerSystemInformationMessage(str);
		}
	}

	return true;
}

CWorldStockVectorPtr CProductFinnhubStockSymbol::ParseFinnhubStockSymbol(CWebDataPtr pWebData) {
	CWorldStockVectorPtr pvStock = make_shared<vector<CWorldStockPtr>>();
	CWorldStockPtr pStock = nullptr;
	ptree pt, pt2;
	string s, sError;

	if (!ConvertToJSON(pt, pWebData)) return pvStock;

	try {
		for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
			pStock = make_shared<CWorldStock>();
			pt2 = it->second;
			s = pt2.get<string>(_T("currency"));
			if (s.size() > 0) pStock->SetCurrency(s.c_str());
			s = pt2.get<string>(_T("description"));
			if (s.size() > 0) pStock->SetDescription(s.c_str());
			s = pt2.get<string>(_T("displaySymbol"));
			pStock->SetDisplaySymbol(s.c_str());
			s = pt2.get<string>(_T("figi"));
			if (s.size() > 0) pStock->SetFigi(s.c_str());
			s = pt2.get<string>(_T("mic"));
			if (s.size() > 0) pStock->SetMic(s.c_str());
			s = pt2.get<string>(_T("symbol"));
			pStock->SetSymbol(s.c_str());
			s = pt2.get<string>(_T("type"));
			if (s.size() > 0) pStock->SetType(s.c_str());
			pvStock->push_back(pStock);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Symbol "), e);
		return pvStock;
	}
	return pvStock;
}