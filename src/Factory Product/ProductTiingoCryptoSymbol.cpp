#include "pch.h"

#include"SystemMessage.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"TiingoCrypto.h"
#include "ProductTiingoCryptoSymbol.h"

#include "ContainerTiingoCryptoSymbol.h"
#include "SystemConfiguration.h"
#include "TiingoDataSource.h"
#include "WorldMarket.h"
#include"cpr/cpr.h"

using namespace std;

CProductTiingoCryptoSymbol::CProductTiingoCryptoSymbol() {
	m_strInquiryFunction = "https://api.tiingo.com/tiingo/crypto?";
	m_differ1 = 0;
	m_differ2 = 0;
	m_ratio = 3;
}

void CProductTiingoCryptoSymbol::InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string s = inquiry + "&token=" + gl_pTiingoDataSource->GetToken();
		cpr::Response r = cpr::Get(cpr::Url{ s });
		m_statusCode = r.status_code;
		m_elapsed = r.elapsed;

		if (m_statusCode != 200) {
			WebStatusCheck(r);
			return;
		}
		const auto pvTiingoCrypto = Parse(r.text);
		if (!pvTiingoCrypto->empty()) {
			for (auto& tiingoCrypto : *pvTiingoCrypto) {
				if (!gl_dataContainerTiingoCryptoSymbol.IsSymbol(tiingoCrypto->GetSymbol())) {
					tiingoCrypto->SetUpdateProfileDB(true);
					gl_dataContainerTiingoCryptoSymbol.Add(tiingoCrypto);
				}
			}
		}
	}
}

void CProductTiingoCryptoSymbol::WebStatusCheck(cpr::Response& r) {
}

shared_ptr<vector<string>> CProductTiingoCryptoSymbol::CreateMessage() {
	m_strInquiringSymbol = "All";
	m_inquiryString = m_strInquiryFunction;
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// https://api.tiingo.com/documentation/crypto
// 格式：
// [
// {
//	"quoteCurrency" : "btc",
//	"name" : "CureCoin (CURE/BTC)",
//	"ticker":"curebtc",
//	"baseCurrency" : "cure"
// },
// {
//	"quoteCurrency": "ust",
//	"name" : "KSM (KSM/UST)",
//	"ticker" : "ksmust",
//	"baseCurrency" : "ksm"
// }
// ]
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTiingoCryptosPtr CProductTiingoCryptoSymbol::Parse(const string& text) {
	auto pvTiingoCrypto = make_shared<vector<CTiingoCryptoPtr>>();
	CTiingoCryptoPtr pTiingoCrypto = nullptr;
	nlohmannJson js;

	if (text.empty()) return pvTiingoCrypto;
	if (!::CreateJsonWithNlohmann(js, text)) return pvTiingoCrypto;
	if (::IsVoidJson(text)) return pvTiingoCrypto;

	try {
		int iCount = 0;
		for (auto it = js.begin(); it != js.end(); ++it) {
			pTiingoCrypto = make_shared<CTiingoCrypto>();
			string s = jsonGetString(it, "ticker");
			pTiingoCrypto->SetSymbol(s);
			s = jsonGetString(it, "name");
			if (!s.empty()) pTiingoCrypto->m_strName = s;
			s = jsonGetString(it, "baseCurrency");
			if (!s.empty()) pTiingoCrypto->m_strBaseCurrency = s;
			s = jsonGetString(it, "quoteCurrency");
			pTiingoCrypto->m_strQuoteCurrency = s;

			pTiingoCrypto->SetUpdateProfileDB(true); // 所有申请到的股票，皆当成新股票对待，需要存入数据库。
			pvTiingoCrypto->push_back(pTiingoCrypto);
			iCount++;
		}
	} catch (nlohmannJson::exception& e) {
		if (pTiingoCrypto != nullptr) ReportJSonErrorToSystemMessage("Tiingo crypto symbol " + pTiingoCrypto->GetSymbol(), e.what());
	}

	return pvTiingoCrypto;
}
void CProductTiingoCryptoSymbol::UpdateSystemStatus() {
	gl_pTiingoDataSource->SetUpdateCryptoSymbol(false);
	gl_systemConfiguration.SetTiingoCryptoSymbolUpdateDate(gl_pWorldMarket->GetMarketDate());
	gl_systemMessage.PushInformationMessage("Tiingo crypto symbol updated");
}
