#include "pch.h"

#include"globedef.h"
#include"accessory.h"
#include"WorldMarket.h"

#include"TiingoCryptoSymbol.h"
#include "ProductTiingoCryptoSymbol.h"

IMPLEMENT_DYNCREATE(CProductTiingoCryptoSymbol, CProductWebSourceData)

CProductTiingoCryptoSymbol::CProductTiingoCryptoSymbol() : CProductWebSourceData() {
	m_strClassName = _T("Tiingo crypto symbols");
	m_strInquiringStr = _T("https://api.tiingo.com/tiingo/crypto?");
}

CString CProductTiingoCryptoSymbol::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CProductTiingoCryptoSymbol::ProcessWebData(CWebDataPtr pWebData) {
	CTiingoCryptoVectorPtr pvTiingoCrypto = nullptr;
	char buffer[100];
	CString strNumber, str;

	pvTiingoCrypto = ParseTiingoCryptoSymbol(pWebData);
	if (pvTiingoCrypto->size() > 0) {
		for (auto& pTiingoCrypto : *pvTiingoCrypto) {
			if (!((CWorldMarket*)m_pMarket)->IsTiingoCryptoSymbol(pTiingoCrypto->m_strTicker)) {
				((CWorldMarket*)m_pMarket)->AddTiingoCryptoSymbol(pTiingoCrypto);
			}
		}
		TRACE("今日Tiingo crypto symbol活跃数为：%d\n", pvTiingoCrypto->size());
		sprintf_s(buffer, _T("%zd"), pvTiingoCrypto->size());
		strNumber = buffer;
		str = _T("今日Tiingo Crypto Symbol活跃总数为") + strNumber;
		gl_systemMessage.PushInnerSystemInformationMessage(str);
	}
	((CWorldMarket*)m_pMarket)->SetTiingoCryptoSymbolUpdated(true);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//[
//{
//	"ticker":"curebtc",
//	"baseCurrency" : "cure",
//	"name" : "CureCoin (CURE/BTC)",
//	"quoteCurrency" : "btc",
//	"description" : "CureCoin (CURE/BTC)"
//}
//]
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTiingoCryptoVectorPtr CProductTiingoCryptoSymbol::ParseTiingoCryptoSymbol(CWebDataPtr pWebData) {
	CTiingoCryptoVectorPtr pvTiingoCrypto = make_shared<vector<CTiingoCryptoSymbolPtr>>();
	CString strNULL = _T(" ");
	CTiingoCryptoSymbolPtr pTiingoCrypto = nullptr;
	ptree pt, pt2;
	string s;
	int iCount = 0;
	CString str, strNumber;

	if (!pWebData->CreatePTree(pt, 0, 0)) return pvTiingoCrypto;
	try {
		for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
			pTiingoCrypto = make_shared<CTiingoCryptoSymbol>();
			pt2 = it->second;
			s = pt2.get<string>(_T("ticker"));
			pTiingoCrypto->m_strTicker = s.c_str();
			s = pt2.get<string>(_T("name"));
			if (s.size() > 0) pTiingoCrypto->m_strName = s.c_str();
			s = pt2.get<string>(_T("description"));
			if (s.size() > 0) pTiingoCrypto->m_strDescription = s.c_str();
			s = pt2.get<string>(_T("baseCurrency"));
			if (s.size() > 0) pTiingoCrypto->m_strBaseCurrency = s.c_str();
			s = pt2.get<string>(_T("quoteCurrency"));
			pTiingoCrypto->m_strQuoteCurrency = s.c_str();

			pvTiingoCrypto->push_back(pTiingoCrypto);
			iCount++;
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Crypto Symbol "), e);
	}

	return pvTiingoCrypto;
}