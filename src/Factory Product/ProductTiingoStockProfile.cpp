#include "pch.h"

#include"jsonParse.h"

#include"TiingoStock.h"
#include "ProductTiingoStockProfile.h"

#include "TiingoDataSource.h"

#include"simdjsonGetValue.h"
#include "TimeConvert.h"
#include "WebData.h"
#include "WorldMarket.h"

CProductTiingoStockProfile::CProductTiingoStockProfile() {
	m_strInquiryFunction = "https://api.tiingo.com/tiingo/fundamentals/meta?";
}

string CProductTiingoStockProfile::CreateMessage() {
	m_strInquiringSymbol = "All";
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

	ranges::sort(*pvTiingoStock, [](const CTiingoStockPtr& pData1, const CTiingoStockPtr& pData2) {
		auto s = pData1->GetSymbol();
		return s.compare(pData2->GetSymbol()) < 0;
	});

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
	string strNotAvailable{ "Field not available for free/evaluation" }; // Tiingo免费账户有多项内容空缺，会返回此信息。
	CTiingoStockPtr pStock = nullptr;

	if (!IsValidData(pWebData)) return pvTiingoStock;

	try {
		string s1;
		string_view sv;
		string_view svJson = pWebData->GetStringView();
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		int iCount = 0;
		for (auto item : doc) {
			string strNULL;
			char* stopString;
			auto itemValue = item.value();
			pStock = make_shared<CTiingoStock>();
			s1 = simdjsonGetStringView(itemValue, "permaTicker");
			pStock->SetTiingoPermaTicker(s1);
			s1 = simdjsonGetStringView(itemValue, "ticker");
			std::ranges::transform(s1, s1.begin(), ::toupper); // 不知为什么，当生成库时，使用toupper报错；而使用_toupper则正常编译通过。(需要使用::toupper）
			pStock->SetSymbol(s1);
			s1 = simdjsonGetStringView(itemValue, "name");
			pStock->SetName(s1);
			pStock->SetActive(simdjsonGetBool(itemValue, "isActive"));
			pStock->SetIsADR(simdjsonGetBool(itemValue, "isADR"));
			s1 = simdjsonGetStringView(itemValue, "industry");
			if (s1 != strNotAvailable) {
				pStock->SetTiingoIndustry(s1);
			}
			else pStock->SetTiingoIndustry(strNULL);
			s1 = simdjsonGetStringView(itemValue, "sector");
			if (s1 != strNotAvailable) {
				pStock->SetTiingoSector(s1);
			}
			else pStock->SetTiingoSector(strNULL);

			sv = itemValue["sicCode"].raw_json();
			if (sv == ("\"Field not available for free/evaluation\"") || sv.empty()) {
				pStock->SetSicCode(0);
			}
			else {
				string sTemp2(sv);
				pStock->SetSicCode(strtol(sTemp2.c_str(), &stopString, 10));
			}
			s1 = simdjsonGetStringView(itemValue, "sicIndustry");
			if (s1 != strNotAvailable) {
				pStock->SetSicIndustry(s1);
			}
			else pStock->SetSicIndustry(strNULL);
			s1 = simdjsonGetStringView(itemValue, "sicSector");
			if (s1 != strNotAvailable) {
				pStock->SetSicSector(s1);
			}
			else pStock->SetSicSector(strNULL);
			s1 = simdjsonGetStringView(itemValue, "reportingCurrency");
			if (s1 != strNotAvailable) { // 此项应该永远存在
				pStock->SetReportingCurrency(s1);
			}
			else pStock->SetReportingCurrency(strNULL);
			s1 = simdjsonGetStringView(itemValue, "location");
			if (s1 != strNotAvailable) {
				pStock->SetLocation(s1);
			}
			else pStock->SetLocation(strNULL);
			s1 = simdjsonGetStringView(itemValue, "companyWebsite");
			if (s1 != strNotAvailable) {
				pStock->SetCompanyWebSite(s1);
			}
			else pStock->SetCompanyWebSite(strNULL);
			s1 = simdjsonGetStringView(itemValue, "secFilingWebsite");
			if (s1 != strNotAvailable) {
				pStock->SetSECFilingWebSite(s1);
			}
			else pStock->SetSECFilingWebSite(strNULL);
			s1 = simdjsonGetStringView(itemValue, "statementLastUpdated");
			istringstream ss1(s1);
			chrono::local_time<chrono::milliseconds> ls1;
			ss1 >> parse("%FT%T%Z", ls1);
			pStock->SetStatementLastUpdatedDate(chrono::floor<chrono::days>(ls1));
			s1 = simdjsonGetStringView(itemValue, "dailyLastUpdated");
			istringstream ss2(s1);
			chrono::local_time<chrono::milliseconds> ls2;
			ss2 >> parse("%FT%T%Z", ls2);
			pStock->SetDailyUpdateDate(chrono::floor<chrono::days>(ls2));
			s1 = simdjsonGetStringView(itemValue, "dataProviderPermaTicker");
			if (s1 != strNotAvailable) {
				pStock->SetDataProviderPermaTicker(s1);
			}
			else pStock->SetSECFilingWebSite(strNULL);

			pStock->SetUpdateProfileDB(true); // 所有申请到的股票，皆当成新股票对待，需要存入数据库。
			pvTiingoStock->push_back(pStock);
			iCount++;
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage("Tiingo Stock Symbol ", error.what());
	}
	return pvTiingoStock;
}

void CProductTiingoStockProfile::UpdateSystemStatus() {
	gl_pTiingoDataSource->SetUpdateStockSymbol(false);
	gl_systemConfiguration.SetTiingoFundamentalsMetaUpdateDate(gl_pWorldMarket->GetMarketDate());
	gl_systemMessage.PushInformationMessage("Tiingo stock symbol updated");
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
		auto s = pStockFirst->GetSymbol();
		if (s != pStockNext->GetSymbol()) { // 代码不同
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
	using namespace StockMarket;
	const auto& t = TiingoStockNewSymbol{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);
	auto multi_insert = insert_into(t).columns(t.Symbol, t.Date);

	db(sqlpp::remove_from(t).unconditionally());
	int nValues = 0;
	for (size_t index = 0; index < gl_dataContainerTiingoNewSymbol.Size(); index++) {
		auto pStock = gl_dataContainerTiingoNewSymbol.GetStock(index);
		multi_insert.values.add(
			t.Symbol = pStock->GetSymbol(),
			t.Date = toFormattedDate(gl_pWorldMarket->GetMarketDate())
		);
		++nValues;
	}
	if (nValues > 0) db(multi_insert);
	tx.commit();
}

void CProductTiingoStockProfile::SaveDelistedSymbol() {
	using namespace StockMarket;
	const auto& t = TiingoStockDelistedSymbol{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);
	auto multi_insert = insert_into(t).columns(t.Symbol, t.Date);

	db(sqlpp::remove_from(t).where(t.Date == toFormattedDate(gl_pWorldMarket->GetMarketDate())));
	int nValues = 0;
	for (size_t index = 0; index < gl_dataContainerTiingoDelistedSymbol.Size(); index++) {
		auto pStock = gl_dataContainerTiingoDelistedSymbol.GetStock(index);
		multi_insert.values.add(t.Symbol = pStock->GetSymbol(), t.Date = toFormattedDate(gl_pWorldMarket->GetMarketDate()));
		++nValues;
	}
	if (nValues > 0) db(multi_insert);
	tx.commit();
}
