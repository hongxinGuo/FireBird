#include "pch.h"

#include"SaveAndLoad.h"

#include"jsonParse.h"

#include "ProductFinnhubStockSymbol.h"

IMPLEMENT_DYNCREATE(CProductFinnhubStockSymbol, CProductFinnhub)

CProductFinnhubStockSymbol::CProductFinnhubStockSymbol() {
	m_strClassName = _T("Finnhub company symbols");
	m_strInquiry = _T("https://finnhub.io/api/v1/stock/symbol?exchange=");
	m_lIndex = -1;
}

CString CProductFinnhubStockSymbol::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto strMiddle = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStockExchangeCode(m_lIndex);

	m_strInquiringExchange = strMiddle;
	m_strTotalInquiryMessage = m_strInquiry + strMiddle;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubStockSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	// 临时输出数据至文件中。
	// SaveToFile(_T("C:\\StockAnalysis\\StockSymbol.json"), pWebData->GetDataBuffer());

	const auto strExchangeCode = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStockExchangeCode(m_lIndex);
	const auto pvStock = ParseFinnhubStockSymbol2(pWebData);
	const auto pExchange = gl_pWorldMarket->GetStockExchange(m_lIndex);
	pExchange->SetUpdated(true);
	// 加上交易所代码。
	for (const auto& pStock3 : *pvStock) {
		pStock3->SetExchangeCode(strExchangeCode);
	}
	//检查合法性：只有美国股票代码无须加上交易所后缀。
	if (!pvStock->empty()) {
		const auto pStock = pvStock->at(0);
		if (IsNeedAddExchangeCode(pStock->GetSymbol(), strExchangeCode) && (strExchangeCode.CompareNoCase(_T("US")) == 0)) {
			gl_systemMessage.PushErrorMessage(_T("股票代码格式不符：") + pStock->GetSymbol() + _T("  ") + strExchangeCode);
		}
	}
	for (const auto& pStock2 : *pvStock) {
		if (!dynamic_cast<CWorldMarket*>(m_pMarket)->IsStock(pStock2->GetSymbol())) {
			dynamic_cast<CWorldMarket*>(m_pMarket)->AddStock(pStock2);
			const auto str = _T("Finnhub发现新代码:") + pStock2->GetSymbol();
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
	auto pvStock = make_shared<vector<CWorldStockPtr>>();
	CWorldStockPtr pStock = nullptr;
	string s, sError;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvStock;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvStock;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvStock;
	}
	ppt = pWebData->GetPTree();
	try {
		ptree pt2;
		for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
			pStock = make_shared<CWorldStock>();
			pt2 = it->second;
			s = pt2.get<string>(_T("currency"));
			if (!s.empty()) pStock->SetCurrency(s.c_str());
			s = pt2.get<string>(_T("description"));
			if (!s.empty()) pStock->SetDescription(s.c_str());
			s = pt2.get<string>(_T("displaySymbol"));
			pStock->SetDisplaySymbol(s.c_str());
			s = pt2.get<string>(_T("figi"));
			if (!s.empty()) pStock->SetFigi(s.c_str());
			s = pt2.get<string>(_T("isin"));
			if (!s.empty()) pStock->SetIsin(s.c_str());
			s = pt2.get<string>(_T("mic"));
			if (!s.empty()) pStock->SetMic(s.c_str());
			s = pt2.get<string>(_T("shareClassFIGI"));
			if (!s.empty()) pStock->SetShareClassFIGI(s.c_str());
			s = pt2.get<string>(_T("symbol"));
			pStock->SetSymbol(s.c_str());
			s = pt2.get<string>(_T("symbol2"));
			pStock->SetSymbol2(s.c_str());
			s = pt2.get<string>(_T("type"));
			if (!s.empty()) pStock->SetType(s.c_str());
			pvStock->push_back(pStock);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Symbol "), e);
		return pvStock;
	}
	return pvStock;
}

CWorldStockVectorPtr CProductFinnhubStockSymbol::ParseFinnhubStockSymbol2(CWebDataPtr pWebData) {
	auto pvStock = make_shared<vector<CWorldStockPtr>>();
	CWorldStockPtr pStock = nullptr;
	string s, sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvStock;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvStock;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvStock;
	}
	auto pjs = pWebData->GetJSon();
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			pStock = make_shared<CWorldStock>();
			s = jsonGetString(it, _T("currency"));
			if (!s.empty()) pStock->SetCurrency(s.c_str());
			s = jsonGetString(it, _T("description"));
			if (!s.empty()) pStock->SetDescription(s.c_str());
			s = jsonGetString(it, _T("displaySymbol"));
			pStock->SetDisplaySymbol(s.c_str());
			s = jsonGetString(it, _T("figi"));
			if (!s.empty()) pStock->SetFigi(s.c_str());
			s = jsonGetString(it, _T("isin"));
			if (!s.empty()) pStock->SetIsin(s.c_str());
			s = jsonGetString(it, _T("mic"));
			if (!s.empty()) pStock->SetMic(s.c_str());
			s = jsonGetString(it, _T("shareClassFIGI"));
			if (!s.empty()) pStock->SetShareClassFIGI(s.c_str());
			s = jsonGetString(it, _T("symbol"));
			pStock->SetSymbol(s.c_str());
			s = jsonGetString(it, _T("symbol2"));
			pStock->SetSymbol2(s.c_str());
			s = jsonGetString(it, _T("type"));
			if (!s.empty()) pStock->SetType(s.c_str());
			pvStock->push_back(pStock);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Symbol "), e.what());
		return pvStock;
	}
	return pvStock;
}
