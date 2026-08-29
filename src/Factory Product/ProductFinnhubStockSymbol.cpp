#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubStockSymbol.h"
#include "FinnhubStock.h"

#include "ContainerFinnhubStock.h"
#include "ContainerStockExchange.h"
#include "FinnhubDataSource.h"
#include "SystemMessage.h"

#include"cpr/cpr.h"

using std::make_shared;

CProductFinnhubStockSymbol::CProductFinnhubStockSymbol() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/symbol?exchange=";
}

void CProductFinnhubStockSymbol::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		cpr::Response r = cpr::Get(cpr::Url{ inquireString },
		                           cpr::Redirect{ 5, true, true, cpr::PostRedirectFlags::POST_ALL } // 允许重定向
		);
		m_statusCode = r.status_code;
		m_elapsed = r.elapsed;

		if (m_statusCode != 200) {
			WebStatusCheck(r);
			return;
		}

		const auto pvStock = Parse(r.text);
		const auto pExchange = gl_dataContainerStockExchange.GetItem(m_index);
		pExchange->SetUpdateStockSymbol(false);

		//检查合法性：只有美国股票代码无须加上交易所后缀。
		if (!pvStock->empty()) {
			const auto pStock = pvStock->at(0);
			if (IsBadStockSymbol(pStock->GetSymbol(), m_strInquiringExchange)) {
				string s = "股票代码格式不符：";
				s += pStock->GetSymbol();
				s += "  ";
				s += m_strInquiringExchange;
				gl_systemMessage.PushErrorMessage(s);
			}
		}
		for (const auto& pStock : *pvStock) {
			if (!gl_dataContainerFinnhubStock.IsSymbol(pStock)) {
				pStock->SetNewStock(true);
				pStock->SetUpdateProfileDB(true); // 此股票需要加入数据库中。
				gl_dataContainerFinnhubStock.Add(pStock);
			}
		}
	}
}

void CProductFinnhubStockSymbol::WebStatusCheck(cpr::Response& r) {
	string s;
	switch (r.status_code) {
	case 0:
		break;
	case 302://redirected, not an error
		break;
	case 401: // no right to access
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		CheckInaccessible();
		break;
	default:
		s = std::format("Finnhub stock symbol concise http error {}. code:{} message: {}", r.status_code, static_cast<int>(r.error.code), r.error.message);
		gl_systemMessage.PushInnerSystemInformationMessage(s);
		break;
	}
}

shared_ptr<vector<string>> CProductFinnhubStockSymbol::CreateMessage() {
	m_strInquiringExchange = gl_dataContainerStockExchange.GetItemExchangeCode(m_index);

	m_inquiryString = m_strInquiryFunction + m_strInquiringExchange;
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

bool CProductFinnhubStockSymbol::IsBadStockSymbol(const string& strStockSymbol, const string& strExchangeCode) {
	if (strExchangeCode == "US") return false; // 美国股票无需掭加交易所代码
	if (strStockSymbol.length() <= strExchangeCode.length()) return true; // 股票代码长度不大于交易所代码长度时，需要掭加。

	const auto iLength = strExchangeCode.length();
	const auto iSymbolLength = strStockSymbol.length();
	const string strRight = strStockSymbol.substr(strStockSymbol.length() - iLength, iLength);
	if ((strRight == strExchangeCode) && (strStockSymbol.at(iSymbolLength - iLength - 1) == '.')) {
		return false;
	}
	return true;
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
CFinnhubStocksPtr CProductFinnhubStockSymbol::Parse(const string& text) const {
	auto pvStock = make_shared<vector<CFinnhubStockPtr>>();
	CFinnhubStockPtr pStock = nullptr;
	string sError;
	nlohmannJson js;

	if (text.empty()) return pvStock;
	if (!::CreateJsonWithNlohmann(js, text)) return pvStock;
	if (::IsVoidJson(text)) return pvStock;

	pvStock->reserve(40000);
	try {
		string s;
		for (auto it = js.begin(); it != js.end(); ++it) {
			pStock = make_shared<CFinnhubStock>();
			pStock->SetExchange(m_strInquiringExchange); // 数据中没有交易所代码，在此处加上。
			s = jsonGetString(it, "currency");
			if (!s.empty()) pStock->SetCurrency(s);
			s = jsonGetString(it, "description");
			if (!s.empty()) pStock->SetDescription(s);
			s = jsonGetString(it, "displaySymbol");
			pStock->SetDisplaySymbol(s);
			s = jsonGetString(it, "figi");
			if (!s.empty()) pStock->SetFigi(s);
			s = jsonGetString(it, "isin");
			if (!s.empty()) pStock->SetIsin(s);
			s = jsonGetString(it, "mic");
			if (!s.empty()) pStock->SetMic(s);
			s = jsonGetString(it, "shareClassFIGI");
			if (!s.empty()) pStock->SetShareClassFIGI(s);
			s = jsonGetString(it, "symbol");
			pStock->SetSymbol(s);
			s = jsonGetString(it, "symbol2");
			pStock->SetSymbol2(s);
			s = jsonGetString(it, "type");
			if (!s.empty()) pStock->SetType(s);
			pvStock->push_back(pStock);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Stock Symbol ", e.what());
		return pvStock;
	}
	return pvStock;
}

void CProductFinnhubStockSymbol::UpdateSystemStatus() {
	//do nothing now
}
