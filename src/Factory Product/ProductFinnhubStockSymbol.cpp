#include "pch.h"

import FireBird.System.Message;

import FireBird.Accessory.JsonParse;
import FireBird.Accessory.JsonGetValue;
import FireBird.System.Data;

#include "ProductFinnhubStockSymbol.h"

CProductFinnhubStockSymbol::CProductFinnhubStockSymbol() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/symbol?exchange=");
}

CString CProductFinnhubStockSymbol::CreateMessage() {
	const auto strParam = gl_dataContainerFinnhubStockExchange.GetExchangeCode(m_lIndex);

	m_strInquiringExchange = strParam;
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductFinnhubStockSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto strExchangeCode = gl_dataContainerFinnhubStockExchange.GetExchangeCode(m_lIndex);
	const auto pvStock = ParseFinnhubStockSymbol(pWebData);
	const auto pExchange = gl_dataContainerFinnhubStockExchange.GetExchange(m_lIndex);
	pExchange->SetStockSymbolUpdated(true);
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
		if (!gl_dataContainerFinnhubStock.IsSymbol(pStock2)) {
			pStock2->SetTodayNewStock(true);
			pStock2->SetUpdateProfileDB(true); // 此股票需要加入数据库中。
			gl_dataContainerFinnhubStock.Add(pStock2);
			//const auto str = _T("Finnhub发现新代码:") + pStock2->GetSymbol();
			//gl_systemMessage.PushInnerSystemInformationMessage(str);
		}
	}
	char buffer[30];
	sprintf_s(buffer, _T("%lld"), pvStock->size());
	CString str = _T("今日美国市场股票总数为：");
	str += buffer;
	//gl_systemMessage.PushInnerSystemInformationMessage(str);
}

bool CProductFinnhubStockSymbol::IsNeedAddExchangeCode(const CString& strStockSymbol, const CString& strExchangeCode) {
	const int iLength = strExchangeCode.GetLength();
	const int iSymbolLength = strStockSymbol.GetLength();
	const CString strRight = strStockSymbol.Right(iLength);
	if ((strRight.CompareNoCase(strExchangeCode) == 0) && (strStockSymbol.GetAt(iSymbolLength - iLength - 1) == '.')) {
		return true;
	}
	return false;
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
CFinnhubStocksPtr CProductFinnhubStockSymbol::ParseFinnhubStockSymbol(const CWebDataPtr& pWebData) {
	auto pvStock = make_shared<vector<CFinnhubStockPtr>>();
	CFinnhubStockPtr pStock = nullptr;
	string s, sError;
	nlohmann::ordered_json js;

	if (!pWebData->CreateJson(js)) return pvStock;
	if (!IsValidData(pWebData)) return pvStock;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			pStock = make_shared<CFinnhubStock>();
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
	} catch (nlohmann::ordered_json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Symbol "), e.what());
		return pvStock;
	}
	return pvStock;
}
