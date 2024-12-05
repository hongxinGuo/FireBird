#include "pch.h"

#include"TiingoStock.h"
#include "ProductTiingoFinancialState.h"

import simdjson.GetValue;
#include "WorldMarket.h"

#include"simdjson.h"
using namespace simdjson;

map<CString, int> s_mapItem{
	{ _T("accoci"), 1 },
	{ _T("acctPay"), 2 },
	{ _T("acctRec"), 3 },
	{ _T("assetsCurrent"), 4 },
	{ _T("assetsNonCurrent"), 5 },
	{ _T("assetTurnover"), 6 },
	{ _T("bookVal"), 7 },
	{ _T("businessAcqDisposals"), 8 },
	{ _T("bvps"), 9 },
	{ _T("capex"), 10 },
	{ _T("cashAndEq"), 11 },
	{ _T("consolidatedIncome"), 12 },
	{ _T("costRev"), 13 },
	{ _T("currentRatio"), 14 },
	{ _T("debt"), 15 },
	{ _T("debtCurrent"), 16 },
	{ _T("debtEquity"), 17 },
	{ _T("debtNonCurrent"), 18 },
	{ _T("deferredRev"), 19 },
	{ _T("depamor"), 20 },
	{ _T("deposits"), 21 },
	{ _T("ebit"), 22 },
	{ _T("ebitda"), 23 },
	{ _T("ebt"), 24 },
	{ _T("enterpriseVal"), 25 },
	{ _T("eps"), 26 },
	{ _T("epsDil"), 27 },
	{ _T("epsQoQ"), 28 },
	{ _T("equity"), 29 },
	{ _T("freeCashFlow"), 30 },
	{ _T("fxRate"), 31 },
	{ _T("grossMargin"), 32 },
	{ _T("grossProfit"), 33 },
	{ _T("intangibles"), 34 },
	{ _T("intexp"), 35 },
	{ _T("inventory"), 36 },
	{ _T("investments"), 37 },
	{ _T("investmentsAcqDisposals"), 38 },
	{ _T("investmentsCurrent"), 39 },
	{ _T("investmentsNonCurrent"), 40 },
	{ _T("issrepayDebt"), 41 },
	{ _T("issrepayEquity"), 42 },
	{ _T("liabilitiesCurrent"), 43 },
	{ _T("liabilitiesNonCurrent"), 44 },
	{ _T("longTermDebtEquity"), 45 },
	{ _T("marketCap"), 46 },
	{ _T("ncf"), 47 },
	{ _T("ncff"), 48 },
	{ _T("ncfi"), 49 },
	{ _T("ncfo"), 50 },
	{ _T("ncfx"), 51 },
	{ _T("netinc"), 52 },
	{ _T("netIncComStock"), 53 },
	{ _T("netIncDiscOps"), 54 },
	{ _T("netMargin"), 55 },
	{ _T("nonControllingInterests"), 56 },
	{ _T("opex"), 57 },
	{ _T("opinc"), 58 },
	{ _T("opMargin"), 59 },
	{ _T("payDiv"), 60 },
	{ _T("pbRatio"), 61 },
	{ _T("peRatio"), 62 },
	{ _T("piotroskiFScore"), 63 },
	{ _T("ppeq"), 64 },
	{ _T("prefDVDs"), 65 },
	{ _T("profitMargin"), 66 },
	{ _T("retainedEarnings"), 67 },
	{ _T("revenue"), 68 },
	{ _T("revenueQoQ"), 69 },
	{ _T("rnd"), 70 },
	{ _T("roa"), 71 },
	{ _T("roe"), 72 },
	{ _T("rps"), 73 },
	{ _T("sbcomp"), 74 },
	{ _T("sga"), 75 },
	{ _T("shareFactor"), 76 },
	{ _T("sharesBasic"), 77 },
	{ _T("shareswa"), 78 },
	{ _T("shareswaDil"), 79 },
	{ _T("taxAssets"), 80 },
	{ _T("taxExp"), 81 },
	{ _T("taxLiabilities"), 82 },
	{ _T("totalAssets"), 83 },
	{ _T("totalLiabilities"), 84 },
	{ _T("trailingPEG1Y"), 85 },
};

CProductTiingoFinancialState::CProductTiingoFinancialState() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/fundamentals");
}

CString CProductTiingoFinancialState::CreateMessage() {
	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	CString strParam = _T("/") + pStock->GetSymbol() + _T("/statements?");
	m_strInquiringSymbol = pStock->GetSymbol();

	m_strInquiry = m_strInquiryFunction + strParam;
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
	CString symbol = pStock->GetSymbol();
	CString exchange = _T("US");
	string s1;
	CString strNumber;

	if (!IsValidData(pWebData)) return pvTiingoFinancialState;

	try {
		string_view svJson = pWebData->GetStringView(0, pWebData->GetBufferLength());
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		for (auto item : doc) {
			int iCount = 0;
			auto pFinancialStatePtr = make_shared<CTiingoCompanyFinancialState>();
			pFinancialStatePtr->m_symbol = symbol;
			pFinancialStatePtr->m_exchange = exchange;
			auto itemValue = item.value();
			auto year2 = jsonGetInt64(itemValue, _T("year"));
			auto quarter = jsonGetInt64(itemValue, _T("quarter"));
			pFinancialStatePtr->m_yearQuarter = year2 * 100 + quarter;
			auto date = jsonGetStringView(itemValue, _T("date"));
			auto item2 = itemValue["statementData"];
			try {
				auto statesCashFlow = jsonGetArray(item2.value(), _T("cashFlow"));
				for (auto item4 : statesCashFlow) {
					auto name = jsonGetStringView(item4.value(), _T("dataCode"));
					string s(name.data(), name.length());
					auto value = item4[_T("value")];
					double value4;
					if (value.is_integer()) {
						value4 = jsonGetInt64(value.value());
					}
					else {
						value4 = jsonGetDouble(value.value());
					}
					//TRACE("%s\n", s.c_str());
					int index = s_mapItem.at(s.c_str());
					iCount++;
					pFinancialStatePtr->Assign(index, value4);
				}
			} catch (simdjson_error&) {
			}
			try {
				auto incomeStatement = jsonGetArray(item2.value(), _T("incomeStatement"));
				for (auto item4 : incomeStatement) {
					auto name = jsonGetStringView(item4.value(), _T("dataCode"));
					string s(name.data(), name.length());
					auto value = item4[_T("value")];
					double value4;
					if (value.is_integer()) {
						value4 = jsonGetInt64(value.value());
					}
					else {
						value4 = jsonGetDouble(value.value());
					}
					//TRACE("%s\n", s.c_str());
					int index = s_mapItem.at(s.c_str());
					iCount++;
					pFinancialStatePtr->Assign(index, value4);
				}
			} catch (simdjson_error&) {
			}
			try {
				auto balanceSheet = jsonGetArray(item2.value(), _T("balanceSheet"));
				for (auto item4 : balanceSheet) {
					auto name = jsonGetStringView(item4.value(), _T("dataCode"));
					string s(name.data(), name.length());
					auto value = item4[_T("value")];
					double value4;
					if (value.is_integer()) {
						value4 = jsonGetInt64(value.value());
					}
					else {
						value4 = jsonGetDouble(value.value());
					}
					//TRACE("%s\n", s.c_str());
					int index = s_mapItem.at(s.c_str());
					iCount++;
					pFinancialStatePtr->Assign(index, value4);
				}
			} catch (simdjson_error&) {
			}
			try {
				auto overview = jsonGetArray(item2.value(), _T("overview"));
				for (auto item4 : overview) {
					auto name = jsonGetStringView(item4.value(), _T("dataCode"));
					string s(name.data(), name.length());
					auto value = item4[_T("value")];
					double value4;
					if (value.is_integer()) {
						value4 = jsonGetInt64(value.value());
					}
					else {
						value4 = jsonGetDouble(value.value());
					}
					//TRACE("%s\n", s.c_str());
					int index = s_mapItem.at(s.c_str());
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
