#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"

#include"TiingoStock.h"
#include "ProductTiingoStockSymbol.h"

using namespace std;

CProductTiingoStockSymbol::CProductTiingoStockSymbol() : CVirtualWebProduct() {
	m_strClassName = _T("Tiingo stock symbols");
	m_strInquiry = _T("https://api.tiingo.com/tiingo/fundamentals/meta?");
}

CString CProductTiingoStockSymbol::CreateMessage(void) {
	m_strTotalInquiryMessage = m_strInquiry;
	return m_strTotalInquiryMessage;
}

bool CProductTiingoStockSymbol::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvTiingoStock = ParseTiingoStockSymbol(pWebData);
	if (!pvTiingoStock->empty()) {
		char buffer[100];
		long lTemp = 0;
		for (const auto& pTiingoStock : *pvTiingoStock) {
			if (!dynamic_cast<CWorldMarket*>(m_pMarket)->IsTiingoStock(pTiingoStock->m_strTicker)) { dynamic_cast<CWorldMarket*>(m_pMarket)->AddTiingoStock(pTiingoStock); }
			if (dynamic_cast<CWorldMarket*>(m_pMarket)->IsStock(pTiingoStock->m_strTicker)) {
				// Tiingo的Symbol信息只是用于Finnhub的一个补充。
				lTemp++;
				const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(pTiingoStock->m_strTicker);
				if (pStock->IsNeedUpdateProfile(pTiingoStock)) {
					pStock->UpdateStockProfile(pTiingoStock);
					pStock->SetUpdateProfileDB(true);
				}
			}
			else {
				// new stock，
				// do nothing now.
			}
		}
		TRACE("今日Tiingo活跃股票数为：%d\n", lTemp);
		sprintf_s(buffer, _T("%6d"), lTemp);
		CString strNumber = buffer;
		CString str = _T("今日Tiingo Stock Symbol活跃股票总数为") + strNumber;
		gl_systemMessage.PushInnerSystemInformationMessage(str);
	}

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
CTiingoStockVectorPtr CProductTiingoStockSymbol::ParseTiingoStockSymbol(CWebDataPtr pWebData) {
	auto pvTiingoStock = make_shared<vector<CTiingoStockPtr>>();
	string strNotAvailable{_T("Field not available for free/evaluation")}; // Tiingo免费账户有多项内容空缺，会返回此信息。
	CString strNULL = _T(" ");
	CTiingoStockPtr pStock = nullptr;
	string s;
	CString strNumber;
	long year, month, day;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvTiingoStock;
	if (pWebData->IsVoidJson()) return pvTiingoStock;

	const auto pjs = pWebData->GetJSon();
	try {
		CString str;
		int iCount = 0;
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			pStock = make_shared<CTiingoStock>();
			s = jsonGetString(it,_T("permaTicker"));
			if (!s.empty()) pStock->m_strTiingoPermaTicker = s.c_str();
			s = jsonGetString(it,_T("ticker"));
			ranges::transform(s.begin(), s.end(), s.begin(), ::toupper); // 不知为什么，当生成库时，使用toupper报错；而使用_toupper则正常编译通过。(需要使用::toupper）
			pStock->m_strTicker = s.c_str();
			s = jsonGetString(it,_T("name"));
			if (!s.empty()) pStock->m_strName = s.c_str();
			pStock->m_fIsActive = it->at(_T("isActive"));
			pStock->m_fIsADR = it->at(_T("isADR"));
			s = jsonGetString(it,_T("industry"));
			if (s != strNotAvailable) { if (!s.empty()) pStock->m_strTiingoIndustry = s.c_str(); }
			else pStock->m_strTiingoIndustry = strNULL;
			s = jsonGetString(it,_T("sector"));
			if (s != strNotAvailable) { if (!s.empty()) pStock->m_strTiingoSector = s.c_str(); }
			else pStock->m_strTiingoSector = strNULL;
			auto s2 = it->at(_T("sicCode"));
			if (s2.is_number()) pStock->m_iSICCode = s2;
			else pStock->m_iSICCode = 0;
			s = jsonGetString(it,_T("sicIndustry"));
			if (s != strNotAvailable) { if (!s.empty()) pStock->m_strSICIndustry = s.c_str(); }
			else pStock->m_strSICIndustry = strNULL;
			s = jsonGetString(it,_T("sicSector"));
			if (s != strNotAvailable) { if (!s.empty()) pStock->m_strSICSector = s.c_str(); }
			else pStock->m_strSICSector = strNULL;
			s = jsonGetString(it,_T("reportingCurrency"));
			if (s != strNotAvailable) {	// 此项应该永远存在
				if ((!s.empty())) pStock->m_strReportingCurrency = s.c_str();
			}
			else pStock->m_strReportingCurrency = strNULL;
			s = jsonGetString(it,_T("location"));
			if (s != strNotAvailable) { if ((!s.empty())) pStock->m_strLocation = s.c_str(); }
			else pStock->m_strLocation = _T(" ");
			s = jsonGetString(it,_T("companyWebsite"));
			if (s != strNotAvailable) { if (!s.empty()) pStock->m_strCompanyWebSite = s.c_str(); }
			else pStock->m_strCompanyWebSite = strNULL;
			s = jsonGetString(it,_T("secFilingWebsite"));
			if (s != strNotAvailable) { if (!s.empty()) pStock->m_strSECFilingWebSite = s.c_str(); }
			else pStock->m_strSECFilingWebSite = strNULL;
			s = jsonGetString(it,_T("statementLastUpdated"));
			if (!s.empty()) str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pStock->m_lStatementUpdateDate = year * 10000 + month * 100 + day;
			s = jsonGetString(it,_T("dailyLastUpdated"));
			if (!s.empty()) str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pStock->m_lDailyDataUpdateDate = year * 10000 + month * 100 + day;
			pvTiingoStock->push_back(pStock);
			iCount++;
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Stock Symbol "), e.what());
	}

	return pvTiingoStock;
}
