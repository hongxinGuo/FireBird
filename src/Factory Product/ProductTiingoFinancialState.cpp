#include "pch.h"

#include"TiingoStock.h"
#include "ProductTiingoFinancialState.h"

#include"simdjsonGetValue.h"
#include "WebData.h"
#include "WorldMarket.h"

map<string, int> s_mapItem{
	{ "accoci", 1 },
	{ "acctPay", 2 },
	{ "acctRec", 3 },
	{ "assetsCurrent", 4 },
	{ "assetsNonCurrent", 5 },
	{ "assetTurnover", 6 },
	{ "bookVal", 7 },
	{ "businessAcqDisposals", 8 },
	{ "bvps", 9 },
	{ "capex", 10 },
	{ "cashAndEq", 11 },
	{ "consolidatedIncome", 12 },
	{ "costRev", 13 },
	{ "currentRatio", 14 },
	{ "debt", 15 },
	{ "debtCurrent", 16 },
	{ "debtEquity", 17 },
	{ "debtNonCurrent", 18 },
	{ "deferredRev", 19 },
	{ "depamor", 20 },
	{ "deposits", 21 },
	{ "ebit", 22 },
	{ "ebitda", 23 },
	{ "ebt", 24 },
	{ "enterpriseVal", 25 },
	{ "eps", 26 },
	{ "epsDil", 27 },
	{ "epsQoQ", 28 },
	{ "equity", 29 },
	{ "freeCashFlow", 30 },
	{ "fxRate", 31 },
	{ "grossMargin", 32 },
	{ "grossProfit", 33 },
	{ "intangibles", 34 },
	{ "intexp", 35 },
	{ "inventory", 36 },
	{ "investments", 37 },
	{ "investmentsAcqDisposals", 38 },
	{ "investmentsCurrent", 39 },
	{ "investmentsNonCurrent", 40 },
	{ "issrepayDebt", 41 },
	{ "issrepayEquity", 42 },
	{ "liabilitiesCurrent", 43 },
	{ "liabilitiesNonCurrent", 44 },
	{ "longTermDebtEquity", 45 },
	{ "marketCap", 46 },
	{ "ncf", 47 },
	{ "ncff", 48 },
	{ "ncfi", 49 },
	{ "ncfo", 50 },
	{ "ncfx", 51 },
	{ "netinc", 52 },
	{ "netIncComStock", 53 },
	{ "netIncDiscOps", 54 },
	{ "netMargin", 55 },
	{ "nonControllingInterests", 56 },
	{ "opex", 57 },
	{ "opinc", 58 },
	{ "opMargin", 59 },
	{ "payDiv", 60 },
	{ "pbRatio", 61 },
	{ "peRatio", 62 },
	{ "piotroskiFScore", 63 },
	{ "ppeq", 64 },
	{ "prefDVDs", 65 },
	{ "profitMargin", 66 },
	{ "retainedEarnings", 67 },
	{ "revenue", 68 },
	{ "revenueQoQ", 69 },
	{ "rnd", 70 },
	{ "roa", 71 },
	{ "roe", 72 },
	{ "rps", 73 },
	{ "sbcomp", 74 },
	{ "sga", 75 },
	{ "shareFactor", 76 },
	{ "sharesBasic", 77 },
	{ "shareswa", 78 },
	{ "shareswaDil", 79 },
	{ "taxAssets", 80 },
	{ "taxExp", 81 },
	{ "taxLiabilities", 82 },
	{ "totalAssets", 83 },
	{ "totalLiabilities", 84 },
	{ "trailingPEG1Y", 85 },
};

CProductTiingoFinancialState::CProductTiingoFinancialState() {
	m_strInquiryFunction = "https://api.tiingo.com/tiingo/fundamentals";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 格式为：https://api.tiingo.com/tiingo/fundamentals/AAPL/statements?startDate=1980-01-01
//
///////////////////////////////////////////////////////////////////////////////////////////////////
string CProductTiingoFinancialState::CreateMessage() {
	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	string strParam = "/" + pStock->GetSymbol() + "/statements?startDate=";
	m_strInquiringSymbol = pStock->GetSymbol();
	string sDate = ConvertDateToTimeStamp(pStock->GetCompanyFinancialStatementUpdateDate());
	m_strInquiry = m_strInquiryFunction + strParam + sDate;
	return m_strInquiry;
}

void CProductTiingoFinancialState::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_lIndex);
	const auto pvTiingoFinancialState = ParseTiingoFinancialState(pWebData);

	pTiingoStock->SetCompanyFinancialStatementUpdateDate(gl_pWorldMarket->GetMarketDate());
	pTiingoStock->SetUpdateFinancialState(false);
	pTiingoStock->SetUpdateProfileDB(true);
	if (pvTiingoFinancialState->size() > 0) { // 为空时没有更新的必要。
		pTiingoStock->UpdateFinancialState(pvTiingoFinancialState);
		pTiingoStock->SetUpdateFinancialStateDB(true);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//[
// {
//	 "date":"2024-06-29",
//   "year" : 2024,
//   "quarter" : 3,
//   "statementData" :
//     {
//       "cashFlow":
//         [
//          {"dataCode":"freeCashFlow", "value" : 26707000000.0}, { "dataCode":"ncff","value" : -36017000000.0 }, { "dataCode":"sbcomp","value" : 2869000000.0 }, { "dataCode":"ncfi","value" : -127000000.0 }, { "dataCode":"investmentsAcqDisposals","value" : 2412000000.0 }, { "dataCode":"ncfo","value" : 28858000000.0 }, { "dataCode":"payDiv","value" : -3895000000.0 }, { "dataCode":"businessAcqDisposals","value" : 0.0 }, { "dataCode":"issrepayEquity","value" : -26522000000.0 }, { "dataCode":"ncfx","value" : 0.0 }, { "dataCode":"capex","value" : -2151000000.0 }, { "dataCode":"depamor","value" : 2850000000.0 }, { "dataCode":"issrepayDebt","value" : -3253000000.0 }, { "dataCode":"ncf","value" : -7286000000.0 }
//         ],
//        "incomeStatement" :
//         [
//          {"dataCode":"shareswaDil", "value" : 15348175000.0}, { "dataCode":"intexp","value" : 0.0 }, { "dataCode":"netinc","value" : 21448000000.0 }, { "dataCode":"netIncDiscOps","value" : 0.0 }, { "dataCode":"ebitda","value" : 28344000000.0 }, { "dataCode":"ebt","value" : 25494000000.0 }, { "dataCode":"eps","value" : 1.4 }, { "dataCode":"opinc","value" : 25352000000.0 }, { "dataCode":"shareswa","value" : 15287521000.0 }, { "dataCode":"netIncComStock","value" : 21448000000.0 }, { "dataCode":"consolidatedIncome","value" : 21448000000.0 }, { "dataCode":"taxExp","value" : 4046000000.0 }, { "dataCode":"prefDVDs","value" : 0.0 }, { "dataCode":"sga","value" : 6320000000.0 }, { "dataCode":"epsDil","value" : 1.4 }, { "dataCode":"rnd","value" : 8006000000.0 }, { "dataCode":"nonControllingInterests","value" : 0.0 }, { "dataCode":"revenue","value" : 85777000000.0 }, { "dataCode":"opex","value" : 14326000000.0 }, { "dataCode":"costRev","value" : 46099000000.0 }, { "dataCode":"ebit","value" : 25494000000.0 }, { "dataCode":"grossProfit","value" : 39678000000.0 }
//         ],
//        "balanceSheet" :
//         [
//          {"dataCode":"debtNonCurrent", "value" : 86196000000.0}, { "dataCode":"liabilitiesCurrent","value" : 131624000000.0 }, { "dataCode":"totalLiabilities","value" : 264904000000.0 }, { "dataCode":"debtCurrent","value" : 15108000000.0 }, { "dataCode":"ppeq","value" : 44502000000.0 }, { "dataCode":"taxLiabilities","value" : 0.0 }, { "dataCode":"investmentsNonCurrent","value" : 91240000000.0 }, { "dataCode":"accoci","value" : -8416000000.0 }, { "dataCode":"acctRec","value" : 43172000000.0 }, { "dataCode":"totalAssets","value" : 331612000000.0 }, { "dataCode":"assetsNonCurrent","value" : 206177000000.0 }, { "dataCode":"intangibles","value" : 0.0 }, { "dataCode":"cashAndEq","value" : 25565000000.0 }, { "dataCode":"assetsCurrent","value" : 125435000000.0 }, { "dataCode":"deferredRev","value" : 8053000000.0 }, { "dataCode":"investmentsCurrent","value" : 36236000000.0 }, { "dataCode":"retainedEarnings","value" : -4726000000.0 }, { "dataCode":"taxAssets","value" : 0.0 }, { "dataCode":"deposits","value" : 0.0 }, { "dataCode":"debt","value" : 101304000000.0 }, { "dataCode":"inventory","value" : 6165000000.0 }, { "dataCode":"investments","value" : 127476000000.0 }, { "dataCode":"liabilitiesNonCurrent","value" : 133280000000.0 }, { "dataCode":"sharesBasic","value" : 15334082000.0 }, { "dataCode":"equity","value" : 66708000000.0 }, { "dataCode":"acctPay","value" : 47574000000.0 }
//         ],
//        "overview" :
//          [
//            {"dataCode":"bvps", "value" : 4.35030933054877}, { "dataCode":"debtEquity","value" : 1.51861845655693 }, { "dataCode":"bookVal","value" : 66708000000.0 }, { "dataCode":"piotroskiFScore","value" : 6.0 }, { "dataCode":"shareFactor","value" : 1.0 }, { "dataCode":"rps","value" : 5.59387904668829 }, { "dataCode":"epsQoQ","value" : 0.102362204724409 }, { "dataCode":"grossMargin","value" : 0.462571551814589 }, { "dataCode":"roe","value" : 1.47150259067358 }, { "dataCode":"profitMargin","value" : 0.462571551814589 }, { "dataCode":"revenueQoQ","value" : 0.0486570412117804 }, { "dataCode":"roa","value" : 0.296573389958695 }, { "dataCode":"longTermDebtEquity","value" : 1.29213887389818 }, { "dataCode":"currentRatio","value" : 0.952979699750805 }
//          ]
//      }
//   }
//]
// 使用simdjson解析，速度为Nlohmann-json的三倍。
// 有些股票缺乏某些部分的数据，忽略之即可。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTiingoCompanyFinancialStatesPtr CProductTiingoFinancialState::ParseTiingoFinancialState(const CWebDataPtr& pWebData) {
	auto pvTiingoFinancialState = make_shared<vector<CTiingoCompanyFinancialStatePtr>>();
	CTiingoStockPtr pStock = gl_dataContainerTiingoStock.GetStock(m_lIndex);
	string symbol = pStock->GetSymbol();
	string exchange = "US";
	string s1;

	if (!IsValidData(pWebData)) return pvTiingoFinancialState;

	try {
		string_view svJson = pWebData->GetStringView();
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		for (auto item : doc) {
			int iCount = 0;
			auto pFinancialStatePtr = make_shared<CTiingoCompanyFinancialState>();
			pFinancialStatePtr->m_symbol = symbol;
			pFinancialStatePtr->m_exchange = exchange;
			auto itemValue = item.value();
			auto year2 = simdjsonGetInt64(itemValue, "year");
			auto quarter = simdjsonGetInt64(itemValue, "quarter");
			pFinancialStatePtr->m_yearQuarter = year2 * 100 + quarter;
			auto date = simdjsonGetStringView(itemValue, "date");
			auto item2 = itemValue["statementData"];
			try {
				auto statesCashFlow = simdjsonGetArray(item2.value(), "cashFlow");
				for (auto item4 : statesCashFlow) {
					auto name = simdjsonGetStringView(item4.value(), "dataCode");
					string s(name.data(), name.length());
					auto value = item4["value"];
					double value4;
					if (value.is_integer()) {
						value4 = simdjsonGetInt64(value.value());
					}
					else {
						value4 = simdjsonGetDouble(value.value());
					}
					//TRACE("%s\n", s.c_str());
					int index = s_mapItem.at(s);
					iCount++;
					pFinancialStatePtr->Assign(index, value4);
				}
			} catch (simdjson_error&) {
			}
			try {
				auto incomeStatement = simdjsonGetArray(item2.value(), "incomeStatement");
				for (auto item4 : incomeStatement) {
					auto name = simdjsonGetStringView(item4.value(), "dataCode");
					string s(name.data(), name.length());
					auto value = item4["value"];
					double value4;
					if (value.is_integer()) {
						value4 = simdjsonGetInt64(value.value());
					}
					else {
						value4 = simdjsonGetDouble(value.value());
					}
					//TRACE("%s\n", s.c_str());
					int index = s_mapItem.at(s);
					iCount++;
					pFinancialStatePtr->Assign(index, value4);
				}
			} catch (simdjson_error&) {
			}
			try {
				auto balanceSheet = simdjsonGetArray(item2.value(), "balanceSheet");
				for (auto item4 : balanceSheet) {
					auto name = simdjsonGetStringView(item4.value(), "dataCode");
					string s(name.data(), name.length());
					auto value = item4["value"];
					double value4;
					if (value.is_integer()) {
						value4 = simdjsonGetInt64(value.value());
					}
					else {
						value4 = simdjsonGetDouble(value.value());
					}
					//TRACE("%s\n", s.c_str());
					int index = s_mapItem.at(s);
					iCount++;
					pFinancialStatePtr->Assign(index, value4);
				}
			} catch (simdjson_error&) {
			}
			try {
				auto overview = simdjsonGetArray(item2.value(), "overview");
				for (auto item4 : overview) {
					auto name = simdjsonGetStringView(item4.value(), "dataCode");
					string s(name.data(), name.length());
					auto value = item4["value"];
					double value4;
					if (value.is_integer()) {
						value4 = simdjsonGetInt64(value.value());
					}
					else {
						value4 = simdjsonGetDouble(value.value());
					}
					//TRACE("%s\n", s.c_str());
					int index = s_mapItem.at(s);
					iCount++;
					pFinancialStatePtr->Assign(index, value4);
				}
			} catch (simdjson_error&) {
			}
			pvTiingoFinancialState->push_back(pFinancialStatePtr);
		}
	} catch (simdjson_error&) {
	}
	std::ranges::sort(pvTiingoFinancialState->begin(), pvTiingoFinancialState->end(),
	                  [](const CTiingoCompanyFinancialStatePtr& p1, const CTiingoCompanyFinancialStatePtr& p2) { return p1->m_yearQuarter < p2->m_yearQuarter; });

	return pvTiingoFinancialState;
}
