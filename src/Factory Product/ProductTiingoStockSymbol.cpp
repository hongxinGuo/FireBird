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
	gl_counter.start();
	const auto pvTiingoStock = ParseTiingoStockSymbol(pWebData);
	gl_counter.stop();
	if (!pvTiingoStock->empty()) {
		char buffer[100];
		long lTemp = 0;
		for (const auto& pTiingoStock : *pvTiingoStock) {
			if (!dynamic_cast<CWorldMarket*>(m_pMarket)->IsTiingoStock(pTiingoStock->m_strTicker)) { dynamic_cast<CWorldMarket*>(m_pMarket)->AddTiingoStock(pTiingoStock); }
			if (dynamic_cast<CWorldMarket*>(m_pMarket)->IsStock(pTiingoStock->m_strTicker)) {
				// Tiingo��Symbol��Ϣֻ������Finnhub��һ�����䡣
				lTemp++;
				const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(pTiingoStock->m_strTicker);
				if (pStock->IsNeedUpdateProfile(pTiingoStock)) {
					pStock->UpdateStockProfile(pTiingoStock);
					pStock->SetUpdateProfileDB(true);
				}
			}
			else {
				// new stock��
				// do nothing now.
			}
		}
		sprintf_s(buffer, _T("%6d"), lTemp);
		CString strNumber = buffer;
		CString str = _T("����Tiingo Stock Symbol��Ծ��Ʊ����Ϊ") + strNumber;
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
	string strNotAvailable{_T("Field not available for free/evaluation")}; // Tiingo����˻��ж������ݿ�ȱ���᷵�ش���Ϣ��
	CString strNULL = _T(" ");
	CTiingoStockPtr pStock = nullptr;
	string s;
	CString strNumber;
	long year, month, day;

	pWebData->CreateNlohmannJson(0, 0);
	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvTiingoStock;
	if (pWebData->IsVoidJson()) return pvTiingoStock;

	const auto pjs = pWebData->GetJSon();
	try {
		CString str;
		int iCount = 0;
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			pStock = make_shared<CTiingoStock>();
			pStock->m_strTiingoPermaTicker = jsonGetString(it,_T("permaTicker")).c_str();
			s = jsonGetString(it,_T("ticker"));
			ranges::transform(s, s.begin(), ::toupper); // ��֪Ϊʲô�������ɿ�ʱ��ʹ��toupper��������ʹ��_toupper����������ͨ����(��Ҫʹ��::toupper��
			pStock->m_strTicker = s.c_str();
			pStock->m_strName = jsonGetString(it,_T("name")).c_str();
			pStock->m_fIsActive = it->at(_T("isActive"));
			pStock->m_fIsADR = it->at(_T("isADR"));
			s = jsonGetString(it,_T("industry"));
			if (s != strNotAvailable) { pStock->m_strTiingoIndustry = s.c_str(); }
			else pStock->m_strTiingoIndustry = strNULL;
			s = jsonGetString(it,_T("sector"));
			if (s != strNotAvailable) { pStock->m_strTiingoSector = s.c_str(); }
			else pStock->m_strTiingoSector = strNULL;
			auto s2 = it->at(_T("sicCode"));
			if (s2.is_number()) pStock->m_iSICCode = s2;
			else pStock->m_iSICCode = 0;
			s = jsonGetString(it,_T("sicIndustry"));
			if (s != strNotAvailable) { pStock->m_strSICIndustry = s.c_str(); }
			else pStock->m_strSICIndustry = strNULL;
			s = jsonGetString(it,_T("sicSector"));
			if (s != strNotAvailable) { pStock->m_strSICSector = s.c_str(); }
			else pStock->m_strSICSector = strNULL;
			s = jsonGetString(it,_T("reportingCurrency"));
			if (s != strNotAvailable) {	// ����Ӧ����Զ����
				pStock->m_strReportingCurrency = s.c_str();
			}
			else pStock->m_strReportingCurrency = strNULL;
			s = jsonGetString(it,_T("location"));
			if (s != strNotAvailable) { pStock->m_strLocation = s.c_str(); }
			else pStock->m_strLocation = _T(" ");
			s = jsonGetString(it,_T("companyWebsite"));
			if (s != strNotAvailable) { pStock->m_strCompanyWebSite = s.c_str(); }
			else pStock->m_strCompanyWebSite = strNULL;
			s = jsonGetString(it,_T("secFilingWebsite"));
			if (s != strNotAvailable) { pStock->m_strSECFilingWebSite = s.c_str(); }
			else pStock->m_strSECFilingWebSite = strNULL;
			if (!s.empty()) str = jsonGetString(it,_T("statementLastUpdated")).c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pStock->m_lStatementUpdateDate = year * 10000 + month * 100 + day;
			str = jsonGetString(it,_T("dailyLastUpdated")).c_str();
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