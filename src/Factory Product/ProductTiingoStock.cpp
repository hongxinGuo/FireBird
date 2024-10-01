#include "pch.h"

#include"jsonParse.h"

#include"TiingoStock.h"
#include "ProductTiingoStock.h"

#include "TiingoDataSource.h"

#include"simdjsonGetValue.h"

CProductTiingoStock::CProductTiingoStock() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/fundamentals/meta?");
}

CString CProductTiingoStock::CreateMessage() {
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductTiingoStock::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvTiingoStock = ParseTiingoStockSymbol(pWebData);
	if (!pvTiingoStock->empty()) {
		char buffer[100];
		for (const auto& pTiingoStock : *pvTiingoStock) {
			if (!gl_dataContainerTiingoStock.IsSymbol(pTiingoStock->GetSymbol())) {
				pTiingoStock->SetUpdateProfileDB(true); // ���˹�Ʊ�������ݿ⡣
				gl_dataContainerTiingoStock.Add(pTiingoStock);
			}
			if (gl_dataContainerFinnhubStock.IsSymbol(pTiingoStock->GetSymbol())) { // Tiingo��Symbol��Ϣֻ������Finnhub��һ�����䡣
				const auto pStock = gl_dataContainerFinnhubStock.GetStock(pTiingoStock->GetSymbol());
				if (pStock->IsNeedUpdateProfile(pTiingoStock)) {
					pStock->SetUpdateProfileDB(true);
					pStock->UpdateStockProfile(pTiingoStock);
				}
			}
			else {
				// new stock��
				// do nothing now.
			}
		}
		sprintf_s(buffer, _T("%6d"), pvTiingoStock->size());
		const CString strNumber = buffer;
		const CString str = _T("����Tiingo Stock����Ϊ") + strNumber;
		gl_systemMessage.PushInnerSystemInformationMessage(str);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// [
// {
//	  "permaTicker":"US000000000133",
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
// ʹ��simdjson�������ٶ�ΪNlohmann-json��������
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTiingoStocksPtr CProductTiingoStock::ParseTiingoStockSymbol(const CWebDataPtr& pWebData) {
	auto pvTiingoStock = make_shared<vector<CTiingoStockPtr>>();
	string strNotAvailable{ _T("Field not available for free/evaluation") }; // Tiingo����˻��ж������ݿ�ȱ���᷵�ش���Ϣ��
	CString strNULL = _T(" ");
	CTiingoStockPtr pStock = nullptr;
	string s1;
	CString strNumber;
	long year, month, day;

	if (!IsValidData(pWebData)) return pvTiingoStock;

	try {
		string_view sv;
		string_view svJson = pWebData->GetStringView(0, pWebData->GetBufferLength());
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		CString str;
		int iCount = 0;
		for (auto item : doc) {
			auto itemValue = item.value();
			pStock = make_shared<CTiingoStock>();
			s1 = jsonGetStringView(itemValue, _T("permaTicker"));
			pStock->m_strTiingoPermaTicker = s1.c_str();;
			s1 = jsonGetStringView(itemValue, _T("ticker"));
			std::ranges::transform(s1, s1.begin(), ::toupper); // ��֪Ϊʲô�������ɿ�ʱ��ʹ��toupper������ʹ��_toupper����������ͨ����(��Ҫʹ��::toupper��
			pStock->SetSymbol(s1.c_str());
			s1 = jsonGetStringView(itemValue, _T("name"));
			pStock->m_strName = s1.c_str();;
			pStock->m_fIsActive = jsonGetBool(itemValue, _T("isActive"));
			pStock->m_fIsADR = jsonGetBool(itemValue,_T("isADR"));
			s1 = jsonGetStringView(itemValue, _T("industry"));
			if (s1.compare(strNotAvailable) != 0) {
				pStock->m_strTiingoIndustry = s1.c_str();;
			}
			else pStock->m_strTiingoIndustry = strNULL;
			s1 = jsonGetStringView(itemValue, _T("sector"));
			if (s1.compare(strNotAvailable) != 0) {
				pStock->m_strTiingoSector = s1.c_str();;
			}
			else pStock->m_strTiingoSector = strNULL;

			sv = itemValue["sicCode"].raw_json();
			if (sv == ("\"Field not available for free/evaluation\"") || sv.empty()) {
				pStock->m_iSICCode = 0;
			}
			else {
				string sTemp2(sv);
				pStock->m_iSICCode = atoi(sTemp2.c_str());
			}
			s1 = jsonGetStringView(itemValue, _T("sicIndustry"));
			if (s1.compare(strNotAvailable) != 0) {
				pStock->m_strSICIndustry = s1.c_str();;
			}
			else pStock->m_strSICIndustry = strNULL;
			s1 = jsonGetStringView(itemValue, _T("sicSector"));
			if (s1.compare(strNotAvailable) != 0) {
				pStock->m_strSICSector = s1.c_str();;
			}
			else pStock->m_strSICSector = strNULL;
			s1 = jsonGetStringView(itemValue, _T("reportingCurrency"));
			if (s1.compare(strNotAvailable) != 0) { // ����Ӧ����Զ����
				pStock->m_strReportingCurrency = s1.c_str();;
			}
			else pStock->m_strReportingCurrency = strNULL;
			s1 = jsonGetStringView(itemValue, _T("location"));
			if (s1 != strNotAvailable) {
				pStock->m_strLocation = s1.c_str();;
			}
			else pStock->m_strLocation = _T(" ");
			s1 = jsonGetStringView(itemValue, _T("companyWebsite"));
			if (s1 != strNotAvailable) {
				pStock->m_strCompanyWebSite = s1.c_str();;
			}
			else pStock->m_strCompanyWebSite = strNULL;
			s1 = jsonGetStringView(itemValue, _T("secFilingWebsite"));
			if (s1 != strNotAvailable) {
				pStock->m_strSECFilingWebSite = s1.c_str();;
			}
			else pStock->m_strSECFilingWebSite = strNULL;
			s1 = jsonGetStringView(itemValue, _T("statementLastUpdated"));
			if (!s1.empty()) str = s1.c_str();;
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			//pStock->SetCompanyFinancialStatementUpdateDate(XferYearMonthDayToYYYYMMDD(year, month, day));
			s1 = jsonGetStringView(itemValue, _T("dailyLastUpdated"));
			str = s1.c_str();;
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			//pStock->SetDailyDataUpdateDate(XferYearMonthDayToYYYYMMDD(year, month, day));
			pStock->SetUpdateProfileDB(true); // �������뵽�Ĺ�Ʊ���Ե����¹�Ʊ�Դ�����Ҫ�������ݿ⡣
			pvTiingoStock->push_back(pStock);
			iCount++;
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Stock Symbol "), error.what());
	}

	return pvTiingoStock;
}

void CProductTiingoStock::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CTiingoDataSource")) == 0);
	dynamic_pointer_cast<CTiingoDataSource>(pDataSource)->SetUpdateStockSymbol(false);
	gl_systemMessage.PushInformationMessage(_T("Tiingo stock symbol�Ѹ���"));
}
