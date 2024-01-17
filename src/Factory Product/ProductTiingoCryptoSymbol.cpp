#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"TiingoCryptoSymbol.h"
#include "ProductTiingoCryptoSymbol.h"

#include "TiingoDataSource.h"

CProductTiingoCryptoSymbol::CProductTiingoCryptoSymbol() {
	m_strClassName = _T("Tiingo crypto symbols");
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/crypto?");
	m_differ1 = 0;
	m_differ2 = 0;
	m_ratio = 3;
}

CString CProductTiingoCryptoSymbol::CreateMessage() {
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

bool CProductTiingoCryptoSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvTiingoCrypto = ParseTiingoCryptoSymbol(pWebData);
	if (!pvTiingoCrypto->empty()) {
		char buffer[100];
		for (const auto& pTiingoCrypto : *pvTiingoCrypto) {
			if (!gl_dataContainerTiingoCryptoSymbol.IsSymbol(pTiingoCrypto->m_strTicker)) {
				gl_dataContainerTiingoCryptoSymbol.Add(pTiingoCrypto);
			}
		}
		sprintf_s(buffer, _T("%zd"), pvTiingoCrypto->size());
		const CString strNumber = buffer;
		const CString str = _T("今日Tiingo Crypto Symbol活跃总数为") + strNumber;
		gl_systemMessage.PushInnerSystemInformationMessage(str);
	}
	return true;
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
CTiingoCryptoVectorPtr CProductTiingoCryptoSymbol::ParseTiingoCryptoSymbol(const CWebDataPtr& pWebData) {
	auto pvTiingoCrypto = make_shared<vector<CTiingoCryptoSymbolPtr>>();
	CString strNULL = _T(" ");
	string s;
	CString str, strNumber;
	CTiingoCryptoSymbolPtr pTiingoCrypto = nullptr;
	json js;

	if (!pWebData->CreateJson(js)) return pvTiingoCrypto;
	if (!IsValidData(pWebData)) return pvTiingoCrypto;

	try {
		int iCount = 0;
		for (auto it = js.begin(); it != js.end(); ++it) {
			pTiingoCrypto = make_shared<CTiingoCryptoSymbol>();
			s = jsonGetString(it, _T("ticker"));
			pTiingoCrypto->m_strTicker = s.c_str();
			s = jsonGetString(it, _T("name"));
			if (!s.empty()) pTiingoCrypto->m_strName = s.c_str();
			s = jsonGetString(it, _T("baseCurrency"));
			if (!s.empty()) pTiingoCrypto->m_strBaseCurrency = s.c_str();
			s = jsonGetString(it, _T("quoteCurrency"));
			pTiingoCrypto->m_strQuoteCurrency = s.c_str();

			pvTiingoCrypto->push_back(pTiingoCrypto);
			iCount++;
		}
	}
	catch (json::exception& e) {
		if (pTiingoCrypto != nullptr) ReportJSonErrorToSystemMessage(_T("Tiingo crypto symbol ") + pTiingoCrypto->m_strTicker, e.what());
	}

	return pvTiingoCrypto;
}
void CProductTiingoCryptoSymbol::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CTiingoDataSource")) == 0);
	dynamic_pointer_cast<CTiingoDataSource>(pDataSource)->m_fUpdateCryptoSymbol = false;
}
