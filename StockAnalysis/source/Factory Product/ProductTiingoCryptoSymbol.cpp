#include "pch.h"

#include"globedef.h"
#include"accessory.h"
#include"SystemMessage.h"
#include"WorldMarket.h"

#include"TiingoCryptoSymbol.h"
#include "ProductTiingoCryptoSymbol.h"

#include"HighPerformanceCounter.h"

IMPLEMENT_DYNCREATE(CProductTiingoCryptoSymbol, CProductWebSourceData)

CProductTiingoCryptoSymbol::CProductTiingoCryptoSymbol() : CProductWebSourceData() {
	m_strClassName = _T("Tiingo crypto symbols");
	m_strInquiringStr = _T("https://api.tiingo.com/tiingo/crypto?");
}

CString CProductTiingoCryptoSymbol::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CProductTiingoCryptoSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	CTiingoCryptoVectorPtr pvTiingoCrypto = nullptr;
	char buffer[100];
	CString strNumber, str;

	CHighPerformanceCounter counter, counter2;

	// 测试结果，DEBUG状态下，ptree所需时间为json的160%；Release状态下，为3-4倍。
	/*
	counter.start();
	//for (int i = 0; i < 100; i++) {
	pWebData->CreatePropertyTree();
	//}
	counter.stop();

	counter2.start();
	//for (int i = 0; i < 100; i++) {
	pWebData->CreateJSon();
	//}
	counter2.stop();

	m_ratio = (double)counter.GetElapsedMicroSeconde() / counter2.GetElapsedMicroSeconde();
	*/
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
	CTiingoCryptoVectorPtr pvTiingoCrypto = make_shared<vector<CTiingoCryptoSymbolPtr>>();
	CString strNULL = _T(" ");
	CTiingoCryptoSymbolPtr pTiingoCrypto = nullptr;
	ptree pt2;
	string s;
	int iCount = 0;
	CString str, strNumber;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (pWebData->IsParsed()) {
		if (pWebData->IsVoidJSon()) return pvTiingoCrypto;
		ppt = pWebData->GetPTree();
		try {
			for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
				pTiingoCrypto = make_shared<CTiingoCryptoSymbol>();
				pt2 = it->second;
				s = pt2.get<string>(_T("ticker"));
				pTiingoCrypto->m_strTicker = s.c_str();
				s = pt2.get<string>(_T("name"));
				if (s.size() > 0) pTiingoCrypto->m_strName = s.c_str();
				s = pt2.get<string>(_T("baseCurrency"));
				if (s.size() > 0) pTiingoCrypto->m_strBaseCurrency = s.c_str();
				s = pt2.get<string>(_T("quoteCurrency"));
				pTiingoCrypto->m_strQuoteCurrency = s.c_str();

				pvTiingoCrypto->push_back(pTiingoCrypto);
				iCount++;
			}
		}
		catch (ptree_error& e) {
			ReportJSonErrorToSystemMessage(_T("Tiingo crypto symbol ") + pTiingoCrypto->m_strTicker, e);
		}
	}
	return pvTiingoCrypto;
}