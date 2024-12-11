#include "pch.h"

import FireBird.System.Message;

import FireBird.Accessory.JsonParse;
import FireBird.Accessory.JsonGetValue;

import FireBird.Stock.TiingoCrypto;
#include "ProductTiingoCryptoSymbol.h"

import FireBird.DataSource.Tiingo;

CProductTiingoCryptoSymbol::CProductTiingoCryptoSymbol() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/crypto?");
	m_differ1 = 0;
	m_differ2 = 0;
	m_ratio = 3;
}

CString CProductTiingoCryptoSymbol::CreateMessage() {
	m_strInquiringSymbol = _T("All");
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
	CString strNULL = _T(" ");
	string s;
	CString str, strNumber;
	CTiingoCryptoPtr pTiingoCrypto = nullptr;
	nlohmann::ordered_json js;

	if (!pWebData->CreateJson(js)) return pvTiingoCrypto;
	if (!IsValidData(pWebData)) return pvTiingoCrypto;

	try {
		int iCount = 0;
		for (auto it = js.begin(); it != js.end(); ++it) {
			pTiingoCrypto = make_shared<CTiingoCrypto>();
			s = jsonGetString(it, _T("ticker"));
			pTiingoCrypto->SetSymbol(s.c_str());
			s = jsonGetString(it, _T("name"));
			if (!s.empty()) pTiingoCrypto->m_strName = s.c_str();
			s = jsonGetString(it, _T("baseCurrency"));
			if (!s.empty()) pTiingoCrypto->m_strBaseCurrency = s.c_str();
			s = jsonGetString(it, _T("quoteCurrency"));
			pTiingoCrypto->m_strQuoteCurrency = s.c_str();

			pTiingoCrypto->SetUpdateProfileDB(true); // 所有申请到的股票，皆当成新股票对待，需要存入数据库。
			pvTiingoCrypto->push_back(pTiingoCrypto);
			iCount++;
		}
	} catch (nlohmann::ordered_json::exception& e) {
		if (pTiingoCrypto != nullptr) ReportJSonErrorToSystemMessage(_T("Tiingo crypto symbol ") + pTiingoCrypto->GetSymbol(), e.what());
	}

	return pvTiingoCrypto;
}
void CProductTiingoCryptoSymbol::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CTiingoDataSource")) == 0);
	dynamic_pointer_cast<CTiingoDataSource>(pDataSource)->SetUpdateCryptoSymbol(false);
	gl_systemMessage.PushInformationMessage(_T("Tiingo crypto symbol已更新"));
}
