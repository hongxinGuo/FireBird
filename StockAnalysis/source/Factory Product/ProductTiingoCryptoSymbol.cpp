#include "pch.h"

#include"globedef.h"

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
		TRACE("今日Tiingo活跃股票数为：%d\n", pvTiingoCrypto->size());
		sprintf_s(buffer, _T("%6d"), pvTiingoCrypto->size());
		strNumber = buffer;
		str = _T("今日Tiingo Crypto Symbol活跃股票总数为") + strNumber;
		gl_systemMessage.PushInnerSystemInformationMessage(str);
	}
	((CWorldMarket*)m_pMarket)->SetTiingoCryptoSymbolUpdated(true);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// [
// {
//	"permaTicker":"US000000000133",
//		"ticker" : "IBM",
//		"name" : "International Business Machines Corp",
//    "isADR" : false,
//    "industry":"Information Technology Services",
//    "sector":"Technology",
//    "sicCode":3570,
//    "sicIndustry":"Computer & Office Equipment",
//    "sicSector":"Manufacturing",
//		"reportingCurrency":"usd",
//		"location":"New York, USA",
//		"companyWebsite":"http://www.ibm.com",
//		"secFillingWebsite":"https://www.sec.gov/cgi-bin/browse-edgar?action=getcompany&CIK=0000051143",
//		"isActive" : true,
//		"statementLastUpdated" : "2019-12-22T22:08:11.534Z",
//		"dailyLastUpdated" : "2019-12-22T22:08:17.530Z"
// },
// {
// ...
// }
// ]
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTiingoCryptoVectorPtr CProductTiingoCryptoSymbol::ParseTiingoCryptoSymbol(CWebDataPtr pWebData) {
	CTiingoCryptoVectorPtr pvTiingoCrypto = make_shared<vector<CTiingoCryptoSymbolPtr>>();
	string strNotAvailable{ _T("Field not available for free/evaluation") }; // Tiingo免费账户有多项内容空缺，会返回此信息。
	CString strNULL = _T(" ");
	CTiingoCryptoSymbolPtr pTiingoCrypto = nullptr;
	ptree pt, pt2;
	string s;
	int iCount = 0;
	CString str, strNumber;
	char buffer[30];
	long year, month, day;

	if (!ConvertToJSON(pt, pWebData)) return pvTiingoCrypto;
	try {
		for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
			pTiingoCrypto = make_shared<CTiingoCryptoSymbol>();
			pt2 = it->second;
			s = pt2.get<string>(_T("ticker"));
			transform(s.begin(), s.end(), s.begin(), _toupper); // 不知为什么，当生成库时，使用toupper报错；而使用_toupper则正常编译通过。
			pTiingoCrypto->m_strTicker = s.c_str();
			s = pt2.get<string>(_T("name"));
			if (s.size() > 0) pTiingoCrypto->m_strName = s.c_str();
			s = pt2.get<string>(_T("description"));
			if (s.compare(strNotAvailable) != 0) {
				if (s.size() > 0) pTiingoCrypto->m_strDescription = s.c_str();
			}
			else pTiingoCrypto->m_strDescription = strNULL;
			s = pt2.get<string>(_T("baseCurrency"));
			if (s.compare(strNotAvailable) != 0) {
				if (s.size() > 0) pTiingoCrypto->m_strBaseCurrency = s.c_str();
			}
			else pTiingoCrypto->m_strBaseCurrency = strNULL;
			s = pt2.get<string>(_T("quoteCurrency"));
			if (s.compare(strNotAvailable) != 0) {
				pTiingoCrypto->m_strQuoteCurrency = s.c_str();
			}
			else pTiingoCrypto->m_strQuoteCurrency = strNULL;

			pvTiingoCrypto->push_back(pTiingoCrypto);
			iCount++;
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Crypto Symbol "), e);
	}

	return pvTiingoCrypto;
}