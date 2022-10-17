#include "pch.h"

#include"jsonParse.h"

#include "ProductFinnhubStockSymbol.h"

IMPLEMENT_DYNCREATE(CProductFinnhubStockSymbol, CProductFinnhub)

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

bool CProductFinnhubStockSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	CString str;
	CWorldStockVectorPtr  pvStock = nullptr;
	CString strExchangeCode;
	CFinnhubStockExchangePtr pExchange;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	strExchangeCode = ((CWorldMarket*)m_pMarket)->GetStockExchangeCode(m_lIndex);
	pvStock = ParseFinnhubStockSymbol(pWebData);
	pExchange = gl_pWorldMarket->GetStockExchange(m_lIndex);
	pExchange->SetUpdated(true);
	// 加上交易所代码。
	for (auto& pStock3 : *pvStock) {
		pStock3->SetExchangeCode(strExchangeCode);
	}
	//检查合法性：只有美国股票代码无须加上交易所后缀。
	if (pvStock->size() > 0) {
		auto pStock = pvStock->at(0);
		if (IsNeedAddExchangeCode(pStock->GetSymbol(), strExchangeCode) && (strExchangeCode.CompareNoCase(_T("US")) == 0)) {
			gl_systemMessage.PushErrorMessage(_T("股票代码格式不符：") + pStock->GetSymbol() + _T("  ") + strExchangeCode);
		}
	}
	for (auto& pStock2 : *pvStock) {
		if (!((CWorldMarket*)m_pMarket)->IsStock(pStock2->GetSymbol())) {
			((CWorldMarket*)m_pMarket)->AddStock(pStock2);
			str = _T("Finnhub发现新代码:") + pStock2->GetSymbol();
			gl_systemMessage.PushInnerSystemInformationMessage(str);
		}
	}

	return true;
}

bool CProductFinnhubStockSymbol::IsNeedAddExchangeCode(CString strStockSymbol, CString strExchangeCode) {
	int iLength = strExchangeCode.GetLength();
	int iSymbolLength = strStockSymbol.GetLength();
	CString strRight = strStockSymbol.Right(iLength);
	if ((strRight.CompareNoCase((LPCSTR)strExchangeCode) == 0) && (strStockSymbol.GetAt(iSymbolLength - iLength - 1) == '.')) {
		return true;
	}
	else {
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 网址：https://finnhub.io/docs/api/stock-symbols
//
// {
// "currency": "EUR",
// "description": "ASN Groenprojectenfonds",
// "displaySymbol" : "NL0012314660.AS",
// "figi" : "",
// "isin" : null,
// "mic" : "XAMS",
// "shareClassFIGI" : "",
// "symbol" : "NL0012314660.AS",
// "symbol2" : "",
// "type" : ""
// }
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
CWorldStockVectorPtr CProductFinnhubStockSymbol::ParseFinnhubStockSymbol(CWebDataPtr pWebData) {
	CWorldStockVectorPtr pvStock = make_shared<vector<CWorldStockPtr>>();
	CWorldStockPtr pStock = nullptr;
	ptree pt2;
	string s, sError;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (pWebData->IsInvalidData()) return pvStock;
	ppt = pWebData->GetPTree();
	try {
		for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
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
			s = pt2.get<string>(_T("isin"));
			if (s.size() > 0) pStock->SetIsin(s.c_str());
			s = pt2.get<string>(_T("mic"));
			if (s.size() > 0) pStock->SetMic(s.c_str());
			s = pt2.get<string>(_T("shareClassFIGI"));
			if (s.size() > 0) pStock->SetShareClassFIGI(s.c_str());
			s = pt2.get<string>(_T("symbol"));
			pStock->SetSymbol(s.c_str());
			s = pt2.get<string>(_T("symbol2"));
			pStock->SetSymbol2(s.c_str());
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