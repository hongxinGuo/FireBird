#include "pch.h"

#include"jsonParse.h"

#include"TiingoStock.h"
#include "ProductTiingoStockProfile.h"

#include "TiingoDataSource.h"

#include"simdjsonGetValue.h"
#include "TimeConvert.h"

CProductTiingoStockProfile::CProductTiingoStockProfile() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/fundamentals/meta?");
}

CString CProductTiingoStockProfile::CreateMessage() {
	m_strInquiringSymbol = _T("All");
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// tiingo fundamental meta���������ظ����룬��������������Ǹ���Ծ��Ʊ����ȷ�ģ���������ɾ����
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductTiingoStockProfile::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvTiingoStock = ParseTiingoStockSymbol(pWebData);
	CTiingoStocksPtr pvNewTiingoStock = make_shared<vector<CTiingoStockPtr>>();

	std::ranges::sort(*pvTiingoStock, [](const CTiingoStockPtr& pData1, const CTiingoStockPtr& pData2) { return pData1->GetSymbol().Compare(pData2->GetSymbol()) < 0; });

	CTiingoStockPtr pStock = pvTiingoStock->at(0);
	for (size_t l = 1; l < pvTiingoStock->size(); l++) {
		CTiingoStockPtr pStock2 = pvTiingoStock->at(l);
		if (pStock->GetSymbol().Compare(pStock2->GetSymbol()) != 0) { // ���벻ͬ
			pvNewTiingoStock->push_back(pStock);
			pStock = pStock2;
		}
		else { // ��ͬ����
			if (pStock->IsActive()) {
				if (pStock2->IsActive()) {
					if (pStock->GetDailyUpdateDate() < pStock2->GetDailyUpdateDate()) { // ���ǻ�Ծ��Ʊʱ���Ƚ����������ڣ��������µ��Ǹ���
						TRACE("active %s: %d ---- %d\n", pStock->GetSymbol(), pStock->GetDailyUpdateDate(), pStock2->GetDailyUpdateDate());
						pStock = pStock2;
					}
				}
			}
			else {
				pStock = pStock2;
			}
		}
	}
	if (!pvNewTiingoStock->empty()) {
		for (const auto& pTiingoStock : *pvNewTiingoStock) {
			if (gl_dataContainerTiingoStock.IsSymbol(pTiingoStock->GetSymbol())) {
				if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) {
					gl_dataContainerTiingoStock.UpdateProfile(pTiingoStock); // �����˻�ʹ�������ݸ��£�����˻��޶���
				}
			}
			else {
				pTiingoStock->SetUpdateProfileDB(true);
				gl_dataContainerTiingoStock.Add(pTiingoStock); // ���˹�Ʊ�������ݿ⡣
				gl_dataContainerTiingoNewSymbol.Add(pTiingoStock); // Ҳ�����¹������С�
			}
		}
	}
	gl_systemConfiguration.DecreaseTiingoBandWidth(pWebData->GetBufferLength());
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
CTiingoStocksPtr CProductTiingoStockProfile::ParseTiingoStockSymbol(const CWebDataPtr& pWebData) {
	auto pvTiingoStock = make_shared<vector<CTiingoStockPtr>>();
	string strNotAvailable{ _T("Field not available for free/evaluation") }; // Tiingo����˻��ж������ݿ�ȱ���᷵�ش���Ϣ��
	CString strNULL = _T("");
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
			pStock->SetActive(jsonGetBool(itemValue, _T("isActive")));
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
				pStock->m_iSicCode = 0;
			}
			else {
				string sTemp2(sv);
				pStock->m_iSicCode = atoi(sTemp2.c_str());
			}
			s1 = jsonGetStringView(itemValue, _T("sicIndustry"));
			if (s1.compare(strNotAvailable) != 0) {
				pStock->m_strSicIndustry = s1.c_str();;
			}
			else pStock->m_strSicIndustry = strNULL;
			s1 = jsonGetStringView(itemValue, _T("sicSector"));
			if (s1.compare(strNotAvailable) != 0) {
				pStock->m_strSicSector = s1.c_str();;
			}
			else pStock->m_strSicSector = strNULL;
			s1 = jsonGetStringView(itemValue, _T("reportingCurrency"));
			if (s1.compare(strNotAvailable) != 0) { // ����Ӧ����Զ����
				pStock->m_strReportingCurrency = s1.c_str();;
			}
			else pStock->m_strReportingCurrency = strNULL;
			s1 = jsonGetStringView(itemValue, _T("location"));
			if (s1 != strNotAvailable) {
				pStock->m_strLocation = s1.c_str();;
			}
			else pStock->m_strLocation = strNULL;
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
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02dT"), &year, &month, &day); // ֻ��������
			pStock->SetStatementLastUpdatedDate(XferYearMonthDayToYYYYMMDD(year, month, day));
			s1 = jsonGetStringView(itemValue, _T("dailyLastUpdated"));
			str = s1.c_str();;
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02dT"), &year, &month, &day); // ֻ��������
			pStock->SetDailyUpdateDate(XferYearMonthDayToYYYYMMDD(year, month, day));
			s1 = jsonGetStringView(itemValue, _T("dataProviderPermaTicker"));
			if (s1 != strNotAvailable) {
				pStock->m_strDataProviderPermaTicker = s1.c_str();
			}
			else pStock->m_strSECFilingWebSite = strNULL;

			pStock->SetUpdateProfileDB(true); // �������뵽�Ĺ�Ʊ���Ե����¹�Ʊ�Դ�����Ҫ�������ݿ⡣
			pvTiingoStock->push_back(pStock);
			iCount++;
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Stock Symbol "), error.what());
	}
	return pvTiingoStock;
}

void CProductTiingoStockProfile::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CTiingoDataSource")) == 0);
	dynamic_pointer_cast<CTiingoDataSource>(pDataSource)->SetUpdateStockSymbol(false);
	gl_systemMessage.PushInformationMessage(_T("Tiingo stock symbol�Ѹ���"));
}
