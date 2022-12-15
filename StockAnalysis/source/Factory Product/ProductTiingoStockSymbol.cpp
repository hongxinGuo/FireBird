#include "pch.h"

#include"jsonParse.h"
#include"WorldMarket.h"

#include"TiingoStock.h"
#include "ProductTiingoStockSymbol.h"

#include<algorithm>
#include<string>

IMPLEMENT_DYNCREATE(CProductTiingoStockSymbol, CVirtualWebProduct)

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
			if (!dynamic_cast<CWorldMarket*>(m_pMarket)->IsTiingoStock(pTiingoStock->m_strTicker)) {
				dynamic_cast<CWorldMarket*>(m_pMarket)->AddTiingoStock(pTiingoStock);
			}
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

	const auto ppt = pWebData->GetPTree();
	try {
		CString str;
		int iCount = 0;
		ptree pt2;
		for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
			pStock = make_shared<CTiingoStock>();
			pt2 = it->second;
			s = pt2.get<string>(_T("permaTicker"));
			if (!s.empty()) pStock->m_strTiingoPermaTicker = s.c_str();
			s = pt2.get<string>(_T("ticker"));
			ranges::transform(s.begin(), s.end(), s.begin(), ::toupper); // 不知为什么，当生成库时，使用toupper报错；而使用_toupper则正常编译通过。(需要使用::toupper）
			pStock->m_strTicker = s.c_str();
			s = pt2.get<string>(_T("name"));
			if (!s.empty()) pStock->m_strName = s.c_str();
			pStock->m_fIsActive = pt2.get<bool>(_T("isActive"));
			pStock->m_fIsADR = pt2.get<bool>(_T("isADR"));
			s = pt2.get<string>(_T("industry"));
			if (s != strNotAvailable) {
				if (!s.empty()) pStock->m_strTiingoIndustry = s.c_str();
			}
			else pStock->m_strTiingoIndustry = strNULL;
			s = pt2.get<string>(_T("sector"));
			if (s != strNotAvailable) {
				if (!s.empty()) pStock->m_strTiingoSector = s.c_str();
			}
			else pStock->m_strTiingoSector = strNULL;
			s = pt2.get<string>(_T("sicCode"));
			if (s != strNotAvailable) {
				pStock->m_iSICCode = atoi(s.c_str());
			}
			else pStock->m_iSICCode = 0;
			s = pt2.get<string>(_T("sicIndustry"));
			if (s != strNotAvailable) {
				if (!s.empty()) pStock->m_strSICIndustry = s.c_str();
			}
			else pStock->m_strSICIndustry = strNULL;
			s = pt2.get<string>(_T("sicSector"));
			if (s != strNotAvailable) {
				if (!s.empty()) pStock->m_strSICSector = s.c_str();
			}
			else pStock->m_strSICSector = strNULL;
			s = pt2.get<string>(_T("reportingCurrency"));
			if (s != strNotAvailable) {
				// 此项应该永远存在
				if ((!s.empty())) pStock->m_strReportingCurrency = s.c_str();
			}
			else pStock->m_strReportingCurrency = strNULL;
			s = pt2.get<string>(_T("location"));
			if (s != strNotAvailable) {
				if ((!s.empty())) pStock->m_strLocation = s.c_str();
			}
			else pStock->m_strLocation = _T(" ");
			s = pt2.get<string>(_T("companyWebsite"));
			if (s != strNotAvailable) {
				if (!s.empty()) pStock->m_strCompanyWebSite = s.c_str();
			}
			else pStock->m_strCompanyWebSite = strNULL;
			s = pt2.get<string>(_T("secFilingWebsite"));
			if (s != strNotAvailable) {
				if (!s.empty()) pStock->m_strSECFilingWebSite = s.c_str();
			}
			else pStock->m_strSECFilingWebSite = strNULL;
			s = pt2.get<string>(_T("statementLastUpdated"));
			if (!s.empty()) str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pStock->m_lStatementUpdateDate = year * 10000 + month * 100 + day;
			s = pt2.get<string>(_T("dailyLastUpdated"));
			if (!s.empty()) str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pStock->m_lDailyDataUpdateDate = year * 10000 + month * 100 + day;
			pvTiingoStock->push_back(pStock);
			iCount++;
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Stock Symbol "), e);
	}

	return pvTiingoStock;
}
