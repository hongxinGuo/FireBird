#include "pch.h"

#include"jsonParse.h"

#include"TiingoStock.h"
#include "ProductTiingoStockProfile.h"

#include "SetTiingoStockDelistedSymbol.h"
#include "SetTiingoStockNewSymbol.h"
#include "TiingoDataSource.h"

#include"simdjsonGetValue.h"
#include "TimeConvert.h"
#include "WebData.h"
#include "WorldMarket.h"

CProductTiingoStockProfile::CProductTiingoStockProfile() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/fundamentals/meta?");
}

string CProductTiingoStockProfile::CreateMessage() {
	m_strInquiringSymbol = _T("All");
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从数据源处接收到的代码集中有重复代码，更新日期最近的那个活跃股票是正确的，其他皆需删除。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductTiingoStockProfile::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvTiingoStock = ParseTiingoStockSymbol(pWebData);

	ranges::sort(*pvTiingoStock, [](const CTiingoStockPtr& pData1, const CTiingoStockPtr& pData2) { return pData1->GetSymbol().compare(pData2->GetSymbol()) < 0; });

	gl_dataContainerTiingoDelistedSymbol.Reset();
	gl_dataContainerTiingoNewSymbol.Reset();

	if (pvTiingoStock->empty()) return;

	// 删除代码集中重复的代码，相同代码的多个活跃股票则保留DailyUpdate最新的那个，其他也都删除。
	CTiingoStocksPtr pvNewTiingoStock = DeleteDuplicatedSymbol(pvTiingoStock);

	for (const auto& pTiingoStock : *pvNewTiingoStock) {
		if (!pTiingoStock->IsActive()) { // 退市股票？
			if (gl_dataContainerTiingoStock.IsSymbol(pTiingoStock->GetSymbol())) { // 目前存在则准备删除，现有代码集中不存在的退市股票直接抛弃
				gl_dataContainerTiingoDelistedSymbol.Add(pTiingoStock); // 存入退市代码集中，准备删除其日线数据。
			}
			continue;
		}
		if (gl_dataContainerTiingoStock.IsSymbol(pTiingoStock->GetSymbol())) {
			if (gl_systemConfiguration.IsPaidTypeTiingoAccount()) {
				gl_dataContainerTiingoStock.UpdateProfile(pTiingoStock); // 付费账户使用新数据更新，免费账户无动作
			}
		}
		else {
			pTiingoStock->SetUpdateProfileDB(true);
			gl_dataContainerTiingoStock.Add(pTiingoStock); // 将此股票存入数据库。
			gl_dataContainerTiingoNewSymbol.Add(pTiingoStock); // 也存入新股容器中。
		}
	}
	gl_pWorldMarket->DeleteTiingoDelistedStock(); // 最后从代码即中删除已经退市的股票

	// Note 先在这里存储
	SaveNewSymbol();
	SaveDelistedSymbol();
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
// 使用simdjson解析，速度为Nlohmann-json的三倍。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTiingoStocksPtr CProductTiingoStockProfile::ParseTiingoStockSymbol(const CWebDataPtr& pWebData) {
	auto pvTiingoStock = make_shared<vector<CTiingoStockPtr>>();
	string strNotAvailable{ _T("Field not available for free/evaluation") }; // Tiingo免费账户有多项内容空缺，会返回此信息。
	CString strNULL = _T("");
	CTiingoStockPtr pStock = nullptr;
	string s1;
	CString strNumber;

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
			std::ranges::transform(s1, s1.begin(), ::toupper); // 不知为什么，当生成库时，使用toupper报错；而使用_toupper则正常编译通过。(需要使用::toupper）
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
			if (s1.compare(strNotAvailable) != 0) { // 此项应该永远存在
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
			pStock->SetStatementLastUpdatedDate(XferToYYYYMMDD(s1));
			s1 = jsonGetStringView(itemValue, _T("dailyLastUpdated"));
			pStock->SetDailyUpdateDate(XferToYYYYMMDD(s1));
			s1 = jsonGetStringView(itemValue, _T("dataProviderPermaTicker"));
			if (s1 != strNotAvailable) {
				pStock->m_strDataProviderPermaTicker = s1.c_str();
			}
			else pStock->m_strSECFilingWebSite = strNULL;

			pStock->SetUpdateProfileDB(true); // 所有申请到的股票，皆当成新股票对待，需要存入数据库。
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
	gl_systemMessage.PushInformationMessage(_T("Tiingo stock symbol updated"));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 接收到的数据中有重复代码。保留活跃的；如果有多个活跃的，则保留日线更新日期最新的那个。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTiingoStocksPtr CProductTiingoStockProfile::DeleteDuplicatedSymbol(const CTiingoStocksPtr& pvTiingoStock) {
	CTiingoStocksPtr pvNewTiingoStock = make_shared<vector<CTiingoStockPtr>>();

	CTiingoStockPtr pStockFirst = pvTiingoStock->at(0);
	for (size_t l = 1; l < pvTiingoStock->size(); l++) {
		CTiingoStockPtr pStockNext = pvTiingoStock->at(l);
		if (pStockFirst->GetSymbol().compare(pStockNext->GetSymbol()) != 0) { // 代码不同
			pvNewTiingoStock->push_back(pStockFirst);
			pStockFirst = pStockNext;
		}
		else { // 相同代码
			if (pStockFirst->IsActive()) {
				if (pStockNext->IsActive()) {
					if (pStockFirst->GetDailyUpdateDate() < pStockNext->GetDailyUpdateDate()) { // 都是活跃股票时，比较最后更新日期，保留较新的那个。
						//TRACE("active %s: %d ---- %d\n", pStockFirst->GetSymbol(), pStockFirst->GetDailyUpdateDate(), pStockNext->GetDailyUpdateDate());
						string str = "多个活跃股票:" + pStockFirst->GetSymbol();
						//gl_systemMessage.PushInnerSystemInformationMessage(str);
						pStockFirst = pStockNext;
					}
				}
			}
			else {
				pStockFirst = pStockNext;
			}
		}
	}
	return pvNewTiingoStock;
}

void CProductTiingoStockProfile::SaveNewSymbol() {
	string s = fmt::format("[Date] ={:8Ld}", gl_pWorldMarket->GetMarketDate());
	CSetTiingoStockNewSymbol setNewSymbol;
	setNewSymbol.m_strFilter = s.c_str();
	setNewSymbol.Open();
	setNewSymbol.m_pDatabase->BeginTrans();
	// 删除之前存储的代码
	while (!setNewSymbol.IsEOF()) {
		setNewSymbol.Delete();
		setNewSymbol.MoveNext();
	}
	for (size_t index = 0; index < gl_dataContainerTiingoNewSymbol.Size(); index++) {
		auto pStock = gl_dataContainerTiingoNewSymbol.GetStock(index);
		setNewSymbol.AddNew();
		setNewSymbol.m_date = gl_pWorldMarket->GetMarketDate();
		setNewSymbol.m_symbol = pStock->GetSymbol().c_str();
		setNewSymbol.Update();
	}
	setNewSymbol.m_pDatabase->CommitTrans();
	setNewSymbol.Close();
}

void CProductTiingoStockProfile::SaveDelistedSymbol() {
	CSetTiingoStockDelistedSymbol setDelistedSymbol;
	string s = fmt::format("[Date] ={:8Ld}", gl_pWorldMarket->GetMarketDate());
	setDelistedSymbol.m_strFilter = s.c_str();
	setDelistedSymbol.Open();
	setDelistedSymbol.m_pDatabase->BeginTrans();
	// 删除之前存储的代码
	while (!setDelistedSymbol.IsEOF()) {
		setDelistedSymbol.Delete();
		setDelistedSymbol.MoveNext();
	}
	for (size_t index = 0; index < gl_dataContainerTiingoDelistedSymbol.Size(); index++) {
		auto pStock = gl_dataContainerTiingoDelistedSymbol.GetStock(index);
		setDelistedSymbol.AddNew();
		setDelistedSymbol.m_date = gl_pWorldMarket->GetMarketDate();
		setDelistedSymbol.m_symbol = pStock->GetSymbol().c_str();
		setDelistedSymbol.Update();
	}
	setDelistedSymbol.m_pDatabase->CommitTrans();
	setDelistedSymbol.Close();
}
