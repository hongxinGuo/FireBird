#include "pch.h"

#include"jsonParse.h"

#include"WorldMarket.h"

#include"TiingoCryptoSymbol.h"
#include "ProductTiingoCryptoSymbol.h"

#include"HighPerformanceCounter.h"

IMPLEMENT_DYNCREATE(CProductTiingoCryptoSymbol, CVirtualWebProduct)

CProductTiingoCryptoSymbol::CProductTiingoCryptoSymbol() : CVirtualWebProduct() {
	m_strClassName = _T("Tiingo crypto symbols");
	m_strInquiry = _T("https://api.tiingo.com/tiingo/crypto?");
	m_differ1 = 0;
	m_differ2 = 0;
	m_ratio = 3;
}

CString CProductTiingoCryptoSymbol::CreateMessage(void) {
	m_strTotalInquiryMessage = m_strInquiry;
	return m_strTotalInquiryMessage;
}

bool CProductTiingoCryptoSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	CHighPerformanceCounter counter, counter2;

	const auto pvTiingoCrypto = ParseTiingoCryptoSymbol(pWebData);
	if (!pvTiingoCrypto->empty()) {
		char buffer[100];
		for (const auto& pTiingoCrypto : *pvTiingoCrypto) {
			if (!dynamic_cast<CWorldMarket*>(m_pMarket)->IsTiingoCryptoSymbol(pTiingoCrypto->m_strTicker)) {
				dynamic_cast<CWorldMarket*>(m_pMarket)->AddTiingoCryptoSymbol(pTiingoCrypto);
			}
		}
		TRACE("今日Tiingo crypto symbol活跃数为：%d\n", pvTiingoCrypto->size());
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
CTiingoCryptoVectorPtr CProductTiingoCryptoSymbol::ParseTiingoCryptoSymbol(CWebDataPtr pWebData) {
	auto pvTiingoCrypto = make_shared<vector<CTiingoCryptoSymbolPtr>>();
	CString strNULL = _T(" ");
	CTiingoCryptoSymbolPtr pTiingoCrypto = nullptr;
	string s;
	CString str, strNumber;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvTiingoCrypto;
	if (pWebData->IsVoidJson()) return pvTiingoCrypto;

	const auto ppt = pWebData->GetPTree();
	try {
		int iCount = 0;
		for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
			pTiingoCrypto = make_shared<CTiingoCryptoSymbol>();
			ptree pt2 = it->second;
			s = pt2.get<string>(_T("ticker"));
			pTiingoCrypto->m_strTicker = s.c_str();
			s = pt2.get<string>(_T("name"));
			if (!s.empty()) pTiingoCrypto->m_strName = s.c_str();
			s = pt2.get<string>(_T("baseCurrency"));
			if (!s.empty()) pTiingoCrypto->m_strBaseCurrency = s.c_str();
			s = pt2.get<string>(_T("quoteCurrency"));
			pTiingoCrypto->m_strQuoteCurrency = s.c_str();

			pvTiingoCrypto->push_back(pTiingoCrypto);
			iCount++;
		}
	}
	catch (ptree_error& e) {
		if (pTiingoCrypto != nullptr) ReportJSonErrorToSystemMessage(_T("Tiingo crypto symbol ") + pTiingoCrypto->m_strTicker, e);
	}

	return pvTiingoCrypto;
}
