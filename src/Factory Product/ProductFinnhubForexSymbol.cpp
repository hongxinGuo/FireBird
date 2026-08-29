#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductFinnhubForexSymbol.h"

#include "ContainerFinnhubForexExchange.h"
#include "containerFinnhubForexSymbol.h"
#include "FinnhubDataSource.h"
#include "FinnhubForex.h"
#include"cpr/cpr.h"

using std::make_shared;

CProductFinnhubForexSymbol::CProductFinnhubForexSymbol() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/forex/symbol?exchange=";
}

void CProductFinnhubForexSymbol::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		cpr::Response r = cpr::Get(cpr::Url{ inquireString });
		m_statusCode = r.status_code;
		m_elapsed = r.elapsed;

		if (m_statusCode != 200) {
			WebStatusCheck(r);
			return;
		}

		const auto pvForexSymbol = Parse(r.text);
		if (pvForexSymbol->empty()) return;
		for (const auto& pSymbol : *pvForexSymbol) {
			if (!gl_dataFinnhubForexSymbol.IsSymbol(pSymbol->GetSymbol())) {
				pSymbol->SetExchange(gl_dataContainerFinnhubForexExchange.GetItem(m_index));
				gl_dataFinnhubForexSymbol.Add(pSymbol);
			}
		}
	}
}

void CProductFinnhubForexSymbol::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0:
		break;
	case 401: // no right to access
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		CheckInaccessible();
		break;
	default:
		break;
	}
}

void CProductFinnhubForexSymbol::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductFinnhubForexSymbol::CreateMessage() {
	m_strInquiringExchange = gl_dataContainerFinnhubForexExchange.GetItem(m_index);
	m_inquiryString = m_strInquiryFunction + m_strInquiringExchange;
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

CForexSymbolsPtr CProductFinnhubForexSymbol::Parse(const string& text) {
	auto pvForexSymbol = make_shared<vector<CForexSymbolPtr>>();
	CForexSymbolPtr pSymbol = nullptr;
	string sError;
	nlohmannJson js;

	if (text.empty()) return pvForexSymbol;
	if (!::CreateJsonWithNlohmann(js, text)) return pvForexSymbol;
	if (::IsVoidJson(text)) return pvForexSymbol;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			pSymbol = make_shared<CFinnhubForex>();
			string s = jsonGetString(it, "description");
			if (!s.empty()) pSymbol->SetDescription(s);
			s = jsonGetString(it, "displaySymbol");
			pSymbol->SetDisplaySymbol(s);
			s = jsonGetString(it, "symbol");
			pSymbol->SetSymbol(s);
			pvForexSymbol->push_back(pSymbol);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Forex Symbol ", e.what());
	}

	return pvForexSymbol;
}
