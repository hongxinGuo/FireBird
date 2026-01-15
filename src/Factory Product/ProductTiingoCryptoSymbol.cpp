#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"TiingoCrypto.h"
#include "ProductTiingoCryptoSymbol.h"

#include "TiingoDataSource.h"
#include "WebData.h"
#include "WorldMarket.h"

CProductTiingoCryptoSymbol::CProductTiingoCryptoSymbol() {
	m_strInquiryFunction = "https://api.tiingo.com/tiingo/crypto?";
	m_differ1 = 0;
	m_differ2 = 0;
	m_ratio = 3;
}

string CProductTiingoCryptoSymbol::CreateMessage() {
	m_strInquiringSymbol = "All";
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductTiingoCryptoSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvTiingoCrypto = ParseTiingoCryptoSymbol(pWebData);
	if (!pvTiingoCrypto->empty()) {
		for (const auto& pTiingoCrypto : *pvTiingoCrypto) {
			if (!gl_dataContainerTiingoCryptoSymbol.IsSymbol(pTiingoCrypto->GetSymbol())) {
				pTiingoCrypto->SetUpdateProfileDB(true);
				gl_dataContainerTiingoCryptoSymbol.Add(pTiingoCrypto);
			}
		}
	}
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
CTiingoCryptosPtr CProductTiingoCryptoSymbol::ParseTiingoCryptoSymbol(const CWebDataPtr& pWebData) {
	auto pvTiingoCrypto = make_shared<vector<CTiingoCryptoPtr>>();
	CTiingoCryptoPtr pTiingoCrypto = nullptr;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvTiingoCrypto;
	if (!IsValidData(pWebData)) return pvTiingoCrypto;

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
