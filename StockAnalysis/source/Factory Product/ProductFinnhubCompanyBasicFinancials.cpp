#include "pch.h"

#include"globedef.h"
#include"accessory.h"
#include"worldMarket.h"

#include"FinnhubStockBasicFinancials.h"
#include "ProductFinnhubCompanyBasicFinancials.h"

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyBasicFinancials, CProductWebSourceData)

CProductFinnhubCompanyBasicFinancials::CProductFinnhubCompanyBasicFinancials() {
	m_strClassName = _T("Finnhub company basic financials");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/metric?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyBasicFinancials::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMessage;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetBasicFinancialsUpdated(true);

	return strMessage;
}

bool CProductFinnhubCompanyBasicFinancials::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	return true;
}

/// <summary>
/// Processes the web data.∏Ò Ω£∫
//{
//  "metric": {
//    "10DayAverageTradingVolume": 85.81857,
//      "13WeekPriceReturnDaily" : -1.20811,
//      "26WeekPriceReturnDaily" : 19.02729,
//      "3MonthAverageTradingVolume" : 1969.60687,
//      "52WeekHigh" : 182.94,
//      "52WeekHighDate" : "2022-01-04",
//      "52WeekLow" : 122.25,
//      "52WeekLowDate" : "2021-05-12",
//      "52WeekPriceReturnDaily" : 30.47714,
//      "5DayPriceReturnDaily" : -2.42097,
//      "assetTurnoverAnnual" : 1.08408,
//      "assetTurnoverTTM" : 1.02911,
//      "beta" : 1.19283,
//      "bookValuePerShareAnnual" : 3.84068,
//      "bookValuePerShareQuarterly" : 4.40197,
//      "bookValueShareGrowth5Y" : -8.56159,
//      "capitalSpendingGrowth5Y" : -2.73554,
//      "cashFlowPerShareAnnual" : 6.2831,
//      "cashFlowPerShareTTM" : 6.69226,
//      "cashPerSharePerShareAnnual" : 3.81322,
//      "cashPerSharePerShareQuarterly" : 3.91124,
//      "currentDividendYieldTTM" : 0.50855,
//      "currentEv/freeCashFlowAnnual" : 35.30197,
//      "currentEv/freeCashFlowTTM" : 32.62128,
//      "currentRatioAnnual" : 1.07455,
//      "currentRatioQuarterly" : 1.03781,
//      "dividendGrowthRate5Y" : 7.72173,
//      "dividendPerShare5Y" : 0.735,
//      "dividendPerShareAnnual" : 0.85,
//      "dividendYield5Y" : 0.89882,
//      "dividendYieldIndicatedAnnual" : 0.51121,
//      "dividendsPerShareTTM" : 0.865,
//      "ebitdPerShareTTM" : 7.67023,
//      "ebitdaCagr5Y" : 11.48175,
//      "ebitdaInterimCagr5Y" : 12.94863,
//      "epsBasicExclExtraItemsAnnual" : 5.66903,
//      "epsBasicExclExtraItemsTTM" : 6.07716,
//      "epsExclExtraItemsAnnual" : 5.61402,
//      "epsExclExtraItemsTTM" : 6.02325,
//      "epsGrowth3Y" : 22.53337,
//      "epsGrowth5Y" : 22.00709,
//      "epsGrowthQuarterlyYoy" : 24.76506,
//      "epsGrowthTTMYoy" : 62.83015,
//      "epsInclExtraItemsAnnual" : 5.61402,
//      "epsInclExtraItemsTTM" : 6.02325,
//      "epsNormalizedAnnual" : 5.61402,
//      "focfCagr5Y" : 13.67622,
//      "freeCashFlowAnnual" : 78486,
//      "freeCashFlowPerShareTTM" : 5.22047,
//      "freeCashFlowTTM" : 87267,
//      "freeOperatingCashFlow/revenue5Y" : 19.49073,
//      "freeOperatingCashFlow/revenueTTM" : 23.0668,
//      "grossMargin5Y" : 39.14537,
//      "grossMarginAnnual" : 41.77936,
//      "grossMarginTTM" : 43.01906,
//      "inventoryTurnoverAnnual" : 40.03026,
//      "inventoryTurnoverTTM" : 39.74044,
//      "longTermDebt/equityAnnual" : 174.156,
//      "longTermDebt/equityQuarterly" : 148.2358,
//      "marketCapitalization" : 2775774,
//      "monthToDatePriceReturnDaily" : -2.58863,
//      "netDebtAnnual" : 62928,
//      "netDebtInterim" : 58885,
//      "netIncomeEmployeeAnnual" : 629103,
//      "netIncomeEmployeeTTM" : 652954.6,
//      "netInterestCoverageAnnual" : null,
//      "netInterestCoverageTTM" : null,
//      "netMarginGrowth5Y" : 4.08444,
//      "netProfitMargin5Y" : 22.69914,
//      "netProfitMarginAnnual" : 25.88179,
//      "netProfitMarginTTM" : 26.57914,
//      "operatingMargin5Y" : 26.61791,
//      "operatingMarginAnnual" : 29.78238,
//      "operatingMarginTTM" : 30.90032,
//      "payoutRatioAnnual" : 15.24187,
//      "payoutRatioTTM" : 14.4687,
//      "pbAnnual" : 44.28645,
//      "pbQuarterly" : 38.63948,
//      "pcfShareTTM" : 24.81249,
//      "peBasicExclExtraTTM" : 29.00697,
//      "peExclExtraAnnual" : 30.29736,
//      "peExclExtraHighTTM" : 35.67625,
//      "peExclExtraTTM" : 28.23891,
//      "peExclLowTTM" : 13.09033,
//      "peInclExtraTTM" : 28.23891,
//      "peNormalizedAnnual" : 30.29736,
//      "pfcfShareAnnual" : 35.36648,
//      "pfcfShareTTM" : 31.80783,
//      "pretaxMargin5Y" : 27.16387,
//      "pretaxMarginAnnual" : 29.8529,
//      "pretaxMarginTTM" : 30.89133,
//      "priceRelativeToS&P50013Week" : null,
//      "priceRelativeToS&P50026Week" : null,
//      "priceRelativeToS&P5004Week" : null,
//      "priceRelativeToS&P50052Week" : null,
//      "priceRelativeToS&P500Ytd" : null,
//      "psAnnual" : 7.58788,
//      "psTTM" : 7.33705,
//      "ptbvAnnual" : 43.99705,
//      "ptbvQuarterly" : 38.58886,
//      "quickRatioAnnual" : 1.02211,
//      "quickRatioQuarterly" : 0.99799,
//      "receivablesTurnoverAnnual" : 17.25633,
//      "receivablesTurnoverTTM" : 13.20177,
//      "revenueEmployeeAnnual" : 2430678,
//      "revenueEmployeeTTM" : 2456643,
//      "revenueGrowth3Y" : 11.26228,
//      "revenueGrowth5Y" : 11.14945,
//      "revenueGrowthQuarterlyYoy" : 11.22228,
//      "revenueGrowthTTMYoy" : 28.62223,
//      "revenuePerShareAnnual" : 21.691,
//      "revenuePerShareTTM" : 22.63196,
//      "revenueShareGrowth5Y" : 17.21938,
//      "roaRfy" : 28.05791,
//      "roaa5Y" : 18.20499,
//      "roae5Y" : 64.28559,
//      "roaeTTM" : 145.5673,
//      "roeRfy" : 147.4433,
//      "roeTTM" : 27.35279,
//      "roi5Y" : 26.18254,
//      "roiAnnual" : 42.64702,
//      "roiTTM" : 44.18407,
//      "tangibleBookValuePerShareAnnual" : 3.84068,
//      "tangibleBookValuePerShareQuarterly" : 4.40197,
//      "tbvCagr5Y" : -12.01172,
//      "totalDebt/totalEquityAnnual" : 199.0284,
//      "totalDebt/totalEquityQuarterly" : 170.714,
//      "totalDebtCagr5Y" : 7.6067,
//      "yearToDatePriceReturnDaily" : -4.21242
//  },
//    "metricType": "all",
//      "series" : {
//				"annual":{
 //"cashRatio": [{"period":"2020-12-31", "v" : 0.7634660421545667}, { "period":"2019-12-31","v" : 0.11672683513838748 }],
 //"currentRatio":[{"period":"2020-12-31", "v" : 2.7471}, { "period":"2019-12-31","v" : 1.2804 }, { "period":"2018-12-31","v" : 2.4764 }],
 //"ebitPerShare":[{"period":"2020-12-31","v":-57.636},{"period":"2019-12-31","v":-8.9067}],
 //"eps":[{"period":"2020-12-31","v":-64.74},{"period":"2019-12-31","v":-2.64}],
 //"grossMargin":[],
 //"longtermDebtTotalAsset":[{"period":"2019-12-31","v":0.3499},{"period":"2018-12-31","v":0.3572}],
 //"longtermDebtTotalCapital":[{"period":"2019-12-31","v":0.3857},{"period":"2018-12-31","v":0.3824}],
 //"longtermDebtTotalEquity":[{"period":"2019-12-31","v":0.6362},{"period":"2018-12-31","v":0.6192}],
 //"netDebtToTotalCapital":[{"period":"2020-12-31","v":-0.0745},{"period":"2019-12-31","v":0.3875}],
 //"netDebtToTotalEquity":[{"period":"2020-12-31","v":-0.0745},{"period":"2019-12-31","v":0.6391}],
 //"netMargin":[{"period":"2020-12-31","v":-3.4021},{"period":"2019-12-31","v":-0.0964}],
 //"operatingMargin":[{"period":"2020-12-31","v":-3.0288},{"period":"2019-12-31","v":-0.3253}],
 //"pretaxMargin":[{"period":"2020-12-31","v":-3.5853},{"period":"2019-12-31","v":-0.0311}],
 //"salesPerShare":[{"period":"2020-12-31","v":19.0293},{"period":"2019-12-31","v" : 27.376 }] ,
 //"sgaToSale" : [] , "totalDebtToEquity" : [{"period":"2020-12-31", "v" : 0}, { "period":"2019-12-31","v" : 0.6496 }] ,
 //"totalDebtToTotalAsset" : [{"period":"2020-12-31", "v" : 0}, { "period":"2019-12-31","v" : 0.3572 }] ,
 //"totalDebtToTotalCapital" : [{"period":"2020-12-31", "v" : 0}, { "period":"2019-12-31","v" : 0.3938 }] ,
 //"totalRatio" : [{"period":"2020-12-31", "v" : 1.5146}, { "period":"2019-12-31","v" : 2.2219 }] },
	//			"quarterly" : {
 //"cashRatio": [{"period":"2021-03-31", "v" : 0.6252676659528907}, { "period":"2020-12-31","v" : 0.7634660421545667 }],
 //"currentRatio" : [{"period":"2021-03-31", "v" : 2.409}, { "period":"2020-12-31","v" : 2.7471 }],
 //"ebitPerShare" : [{"period":"2021-03-31", "v" : -11.2813}, { "period":"2020-12-31","v" : -2.0973 }],
 //"eps" : [{"period":"2021-03-31", "v" : -12.1333}, { "period":"2020-12-31","v" : -0.9453 }],
 //"grossMargin" : [] ,
 //"longtermDebtTotalAsset" : [{"period":"2020-06-30", "v" : 0.3138}],
 //"longtermDebtTotalCapital" : [{"period":"2020-06-30", "v" : 0.3468}, { "period":"2020-03-31","v" : 0.4853 }],
 //"longtermDebtTotalEquity" : [{"period":"2020-06-30", "v" : 0.7884}, { "period":"2020-03-31","v" : 0.9763 }],
 //"netDebtToTotalCapital" : [{"period":"2021-03-31", "v" : -0.0843}, { "period":"2020-12-31","v" : -0.0745 }] ,
 //"netDebtToTotalEquity" : [{"period":"2021-03-31", "v" : -0.0843}, { "period":"2020-12-31","v" : -0.0745 }] ,
 //"netMargin" : [{"period":"2021-03-31", "v" : -2.9632}, { "period":"2020-12-31","v" : -0.2391 }],
 //"operatingMargin" : [{"period":"2021-03-31", "v" : -2.7551}, { "period":"2020-12-31","v" : -0.5305 }],
 //"pretaxMargin" : [{"period":"2021-03-31", "v" : -2.8522}, { "period":"2020-12-31","v" : -0.6159 }],
 //"salesPerShare" : [{"period":"2021-03-31", "v" : 4.0947}, { "period":"2020-12-31","v" : 3.9533 }],
 //"sgaToSale" : [] ,
 //"totalDebtToEquity" : [{"period":"2021-03-31", "v" : 0}, { "period":"2020-12-31","v" : 0 }] ,
 //"totalDebtToTotalAsset" : [{"period":"2021-03-31", "v" : 0}, { "period":"2020-12-31","v" : 0 }],
 //"totalDebtToTotalCapital" : [{"period":"2021-03-31", "v" : 0}, { "period":"2020-12-31","v" : 0 }] ,
 //"totalRatio" : [{"period":"2021-03-31", "v" : 1.4082}, { "period":"2020-12-31","v" : 1.5146 }] }},
//			"symbol":"AAPL"
// }
//
// </summary>
/// </summary>
/// <param name="pWebData"></param>
/// <param name="pStock"></param>
/// <returns></returns>

bool CProductFinnhubCompanyBasicFinancials::ParseFinnhubStockBasicFinancials(CWebDataPtr pWebData, CFinnhubStockBasicFinancialsPtr pBasicFinancials) {
	string s;
	shared_ptr<ptree> ppt;
	ptree ptMetric, ptSeries, ptAnnual, ptQuarterly, ptCashRatio;
	vector<strValue> vData;
	int year, month, day;

	ASSERT(pWebData->IsJSonContentType());
	if (pWebData->IsSucceedCreatePTree()) {
		if (pWebData->IsVoidJSon()) return false;
		ppt = pWebData->GetPTree();
		try {
			s = ppt->get<string>(_T("symbol"));
			pBasicFinancials->m_symbol = s.c_str();

			ptMetric = ppt->get_child(_T("metric"));
			pBasicFinancials->m_10DayAverageTradingVolume = ptreeGetDouble(ptMetric, _T("10DayAverageTradingVolume"));
			pBasicFinancials->m_13WeekPriceReturnDaily = ptreeGetDouble(ptMetric, _T("13WeekPriceReturnDaily"));
			pBasicFinancials->m_26WeekPriceReturnDaily = ptreeGetDouble(ptMetric, _T("26WeekPriceReturnDaily"));
			pBasicFinancials->m_3MonthAverageTradingVolume = ptreeGetDouble(ptMetric, _T("3MonthAverageTradingVolume"));
			pBasicFinancials->m_52WeekHigh = ptreeGetDouble(ptMetric, _T("52WeekHigh"));
			pBasicFinancials->m_52WeekLow = ptreeGetDouble(ptMetric, _T("52WeekLow"));
			s = ptMetric.get<string>(_T("52WeekHighDate"));
			sscanf_s(s.c_str(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pBasicFinancials->m_52WeekHighDate = year * 10000 + month * 100 + day;
			s = ptMetric.get<string>(_T("52WeekLowDate"));
			sscanf_s(s.c_str(), _T("%04d-%02d-%02d"), &year, &month, &day);
			pBasicFinancials->m_52WeekLowDate = year * 10000 + month * 100 + day;
			pBasicFinancials->m_52WeekPriceReturnDaily = ptreeGetDouble(ptMetric, _T("52WeekPriceReturnDaily"));
			pBasicFinancials->m_5DayPriceReturnDaily = ptreeGetDouble(ptMetric, _T("5DayPriceReturnDaily"));

			pBasicFinancials->m_assetTurnoverAnnual = ptreeGetDouble(ptMetric, _T("assetTurnoverAnnual"));
			pBasicFinancials->m_assetTurnoverTTM = ptreeGetDouble(ptMetric, _T("assetTurnoverTTM"));

			pBasicFinancials->m_beta = ptreeGetDouble(ptMetric, _T("beta"));
			pBasicFinancials->m_bookValuePerShareAnnual = ptreeGetDouble(ptMetric, _T("bookValuePerShareAnnual"));
			pBasicFinancials->m_bookValuePerShareQuarterly = ptreeGetDouble(ptMetric, _T("bookValuePerShareQuarterly"));
			pBasicFinancials->m_bookValueShareGrowth5Y = ptreeGetDouble(ptMetric, _T("bookValueShareGrowth5Y"));

			pBasicFinancials->m_capitalSpendingGrowth5Y = ptreeGetDouble(ptMetric, _T("capitalSpendingGrowth5Y"));
			pBasicFinancials->m_cashFlowPerShareAnnual = ptreeGetDouble(ptMetric, _T("cashFlowPerShareAnnual"));
			pBasicFinancials->m_cashFlowPerShareTTM = ptreeGetDouble(ptMetric, _T("cashFlowPerShareTTM"));
			pBasicFinancials->m_cashPerSharePerShareAnnual = ptreeGetDouble(ptMetric, _T("cashPerSharePerShareAnnual"));
			pBasicFinancials->m_cashPerSharePerShareQuarterly = ptreeGetDouble(ptMetric, _T("cashPerSharePerShareQuarterly"));
			pBasicFinancials->m_currentEV_freeCashFlowAnnual = ptreeGetDouble(ptMetric, _T("currentEv/freeCashFlowAnnual"));
			pBasicFinancials->m_currentEV_freeCashFlowTTM = ptreeGetDouble(ptMetric, _T("currentEv/freeCashFlowTTM"));
			pBasicFinancials->m_currentDividendYieldTTM = ptreeGetDouble(ptMetric, _T("currentDividendYieldTTM"));
			pBasicFinancials->m_currentRatioAnnual = ptreeGetDouble(ptMetric, _T("currentRatioAnnual"));
			pBasicFinancials->m_currentRatioQuarterly = ptreeGetDouble(ptMetric, _T("currentRatioQuarterly"));

			pBasicFinancials->m_dividendGrowthRate5Y = ptreeGetDouble(ptMetric, _T("dividendGrowthRate5Y"));
			pBasicFinancials->m_dividendPerShare5Y = ptreeGetDouble(ptMetric, _T("dividendPerShare5Y"));
			pBasicFinancials->m_dividendPerShareAnnual = ptreeGetDouble(ptMetric, _T("dividendPerShareAnnual"));
			pBasicFinancials->m_dividendsPerShareTTM = ptreeGetDouble(ptMetric, _T("dividendsPerShareTTM"));
			pBasicFinancials->m_dividendYield5Y = ptreeGetDouble(ptMetric, _T("dividendYield5Y"));
			pBasicFinancials->m_dividendYieldIndicatedAnnual = ptreeGetDouble(ptMetric, _T("dividendYieldIndicatedAnnual"));

			pBasicFinancials->m_ebitdaCagr5Y = ptreeGetDouble(ptMetric, _T("ebitdaCagr5Y"));
			pBasicFinancials->m_ebitdaInterimCagr5Y = ptreeGetDouble(ptMetric, _T("ebitdaInterimCagr5Y"));
			pBasicFinancials->m_ebitdPerShareTTM = ptreeGetDouble(ptMetric, _T("ebitdPerShareTTM"));
			pBasicFinancials->m_epsBasicExclExtraItemsAnnual = ptreeGetDouble(ptMetric, _T("epsBasicExclExtraItemsAnnual"));
			pBasicFinancials->m_epsBasicExclExtraItemsTTM = ptreeGetDouble(ptMetric, _T("epsBasicExclExtraItemsTTM"));
			pBasicFinancials->m_epsExclExtraItemsAnnual = ptreeGetDouble(ptMetric, _T("epsExclExtraItemsAnnual"));
			pBasicFinancials->m_epsExclExtraItemsTTM = ptreeGetDouble(ptMetric, _T("epsExclExtraItemsTTM"));
			pBasicFinancials->m_epsGrowth3Y = ptreeGetDouble(ptMetric, _T("epsGrowth3Y"));
			pBasicFinancials->m_epsGrowth5Y = ptreeGetDouble(ptMetric, _T("epsGrowth5Y"));
			pBasicFinancials->m_epsGrowthQuarterlyYoy = ptreeGetDouble(ptMetric, _T("epsGrowthQuarterlyYoy"));
			pBasicFinancials->m_epsGrowthTTMYoy = ptreeGetDouble(ptMetric, _T("epsGrowthTTMYoy"));
			pBasicFinancials->m_epsInclExtraItemsAnnual = ptreeGetDouble(ptMetric, _T("epsInclExtraItemsAnnual"));
			pBasicFinancials->m_epsInclExtraItemsTTM = ptreeGetDouble(ptMetric, _T("epsInclExtraItemsTTM"));
			pBasicFinancials->m_epsNormalizedAnnual = ptreeGetDouble(ptMetric, _T("epsNormalizedAnnual"));

			pBasicFinancials->m_focfCagr5Y = ptreeGetDouble(ptMetric, _T("focfCagr5Y"));
			pBasicFinancials->m_freeCashFlowAnnual = ptreeGetDouble(ptMetric, _T("freeCashFlowAnnual"));
			pBasicFinancials->m_freeCashFlowPerShareTTM = ptreeGetDouble(ptMetric, _T("freeCashFlowPerShareTTM"));
			pBasicFinancials->m_freeCashFlowTTM = ptreeGetDouble(ptMetric, _T("freeCashFlowTTM"));
			pBasicFinancials->m_freeOperatingCashFlow_revenue5Y = ptreeGetDouble(ptMetric, _T("freeOperatingCashFlow/revenue5Y"));
			pBasicFinancials->m_freeOperatingCashFlow_revenueTTM = ptreeGetDouble(ptMetric, _T("freeOperatingCashFlow/revenueTTM"));

			pBasicFinancials->m_grossMargin5Y = ptreeGetDouble(ptMetric, _T("grossMargin5Y"));
			pBasicFinancials->m_grossMarginAnnual = ptreeGetDouble(ptMetric, _T("grossMarginAnnual"));
			pBasicFinancials->m_grossMarginTTM = ptreeGetDouble(ptMetric, _T("grossMarginTTM"));

			pBasicFinancials->m_inventoryTurnoverAnnual = ptreeGetDouble(ptMetric, _T("inventoryTurnoverAnnual"));
			pBasicFinancials->m_inventoryTurnoverTTM = ptreeGetDouble(ptMetric, _T("inventoryTurnoverTTM"));

			pBasicFinancials->m_longTermDebt_equityAnnual = ptreeGetDouble(ptMetric, _T("longTermDebt/equityAnnual"));
			pBasicFinancials->m_longTermDebt_equityQuarterly = ptreeGetDouble(ptMetric, _T("longTermDebt/equityQuarterly"));

			pBasicFinancials->m_marketCapitalization = ptreeGetDouble(ptMetric, _T("marketCapitalization"));
			pBasicFinancials->m_monthToDatePriceReturnDaily = ptreeGetDouble(ptMetric, _T("monthToDatePriceReturnDaily"));

			pBasicFinancials->m_netDebtAnnual = ptreeGetDouble(ptMetric, _T("netDebtAnnual"));
			pBasicFinancials->m_netDebtInterim = ptreeGetDouble(ptMetric, _T("netDebtInterim"));
			pBasicFinancials->m_netIncomeEmployeeAnnual = ptreeGetDouble(ptMetric, _T("netIncomeEmployeeAnnual"));
			pBasicFinancials->m_netIncomeEmployeeTTM = ptreeGetDouble(ptMetric, _T("netIncomeEmployeeTTM"));
			pBasicFinancials->m_netInterestCoverageAnnual = ptreeGetDouble(ptMetric, _T("netInterestCoverageAnnual"));
			pBasicFinancials->m_netInterestCoverageTTM = ptreeGetDouble(ptMetric, _T("netInterestCoverageTTM"));
			pBasicFinancials->m_netMarginGrowth5Y = ptreeGetDouble(ptMetric, _T("netMarginGrowth5Y"));
			pBasicFinancials->m_netProfitMargin5Y = ptreeGetDouble(ptMetric, _T("netProfitMargin5Y"));
			pBasicFinancials->m_netProfitMarginAnnual = ptreeGetDouble(ptMetric, _T("netProfitMarginAnnual"));
			pBasicFinancials->m_netProfitMarginTTM = ptreeGetDouble(ptMetric, _T("netProfitMarginTTM"));

			pBasicFinancials->m_payoutRatioAnnual = ptreeGetDouble(ptMetric, _T("payoutRatioAnnual"));
			pBasicFinancials->m_payoutRatioTTM = ptreeGetDouble(ptMetric, _T("payoutRatioTTM"));
			pBasicFinancials->m_pbAnnual = ptreeGetDouble(ptMetric, _T("pbAnnual"));
			pBasicFinancials->m_pbQuarterly = ptreeGetDouble(ptMetric, _T("pbQuarterly"));
			pBasicFinancials->m_pcfShareTTM = ptreeGetDouble(ptMetric, _T("pcfShareTTM"));
			pBasicFinancials->m_peBasicExclExtraTTM = ptreeGetDouble(ptMetric, _T("peBasicExclExtraTTM"));
			pBasicFinancials->m_peExclExtraAnnual = ptreeGetDouble(ptMetric, _T("peExclExtraAnnual"));
			pBasicFinancials->m_peExclExtraHighTTM = ptreeGetDouble(ptMetric, _T("peExclExtraHighTTM"));
			pBasicFinancials->m_peExclExtraTTM = ptreeGetDouble(ptMetric, _T("peExclExtraTTM"));
			pBasicFinancials->m_peExclLowTTM = ptreeGetDouble(ptMetric, _T("peExclLowTTM"));
			pBasicFinancials->m_peInclExtraTTM = ptreeGetDouble(ptMetric, _T("peInclExtraTTM"));
			pBasicFinancials->m_peNormalizedAnnual = ptreeGetDouble(ptMetric, _T("peNormalizedAnnual"));
			pBasicFinancials->m_pfcfShareAnnual = ptreeGetDouble(ptMetric, _T("pfcfShareAnnual"));
			pBasicFinancials->m_pfcfShareTTM = ptreeGetDouble(ptMetric, _T("pfcfShareTTM"));
			pBasicFinancials->m_pretaxMargin5Y = ptreeGetDouble(ptMetric, _T("pretaxMargin5Y"));
			pBasicFinancials->m_pretaxMarginAnnual = ptreeGetDouble(ptMetric, _T("pretaxMarginAnnual"));
			pBasicFinancials->m_pretaxMarginTTM = ptreeGetDouble(ptMetric, _T("pretaxMarginTTM"));
			pBasicFinancials->m_priceRelativeToSP50013Week = ptreeGetDouble(ptMetric, _T("priceRelativeToS&P50013Week"));
			pBasicFinancials->m_priceRelativeToSP50026Week = ptreeGetDouble(ptMetric, _T("priceRelativeToS&P50026Week"));
			pBasicFinancials->m_priceRelativeToSP5004Week = ptreeGetDouble(ptMetric, _T("priceRelativeToS&P5004Week"));
			pBasicFinancials->m_priceRelativeToSP50052Week = ptreeGetDouble(ptMetric, _T("priceRelativeToS&P50052Week"));
			pBasicFinancials->m_priceRelativeToSP500Ytd = ptreeGetDouble(ptMetric, _T("priceRelativeToS&P500Ytd"));
			pBasicFinancials->m_psAnnual = ptreeGetDouble(ptMetric, _T("psAnnual"));
			pBasicFinancials->m_psTTM = ptreeGetDouble(ptMetric, _T("psTTM"));
			pBasicFinancials->m_ptbvAnnual = ptreeGetDouble(ptMetric, _T("ptbvAnnual"));
			pBasicFinancials->m_ptbvQuarterly = ptreeGetDouble(ptMetric, _T("ptbvQuarterly"));

			pBasicFinancials->m_quickRatioAnnual = ptreeGetDouble(ptMetric, _T("quickRatioAnnual"));
			pBasicFinancials->m_quickRatioQuarterly = ptreeGetDouble(ptMetric, _T("quickRatioQuarterly"));

			pBasicFinancials->m_receivablesTurnoverAnnual = ptreeGetDouble(ptMetric, _T("receivablesTurnoverAnnual"));
			pBasicFinancials->m_receivablesTurnoverTTM = ptreeGetDouble(ptMetric, _T("receivablesTurnoverTTM"));
			pBasicFinancials->m_revenueEmployeeAnnual = ptreeGetDouble(ptMetric, _T("revenueEmployeeAnnual"));
			pBasicFinancials->m_revenueEmployeeTTM = ptreeGetDouble(ptMetric, _T("revenueEmployeeTTM"));
			pBasicFinancials->m_revenueGrowth3Y = ptreeGetDouble(ptMetric, _T("revenueGrowth3Y"));
			pBasicFinancials->m_revenueGrowth5Y = ptreeGetDouble(ptMetric, _T("revenueGrowth5Y"));
			pBasicFinancials->m_revenueGrowthQuarterlyYoy = ptreeGetDouble(ptMetric, _T("revenueGrowthQuarterlyYoy"));
			pBasicFinancials->m_revenueGrowthTTMYoy = ptreeGetDouble(ptMetric, _T("revenueGrowthTTMYoy"));
			pBasicFinancials->m_revenuePerShareAnnual = ptreeGetDouble(ptMetric, _T("revenuePerShareAnnual"));
			pBasicFinancials->m_revenuePerShareTTM = ptreeGetDouble(ptMetric, _T("revenuePerShareTTM"));
			pBasicFinancials->m_revenueShareGrowth5Y = ptreeGetDouble(ptMetric, _T("revenueShareGrowth5Y"));
			pBasicFinancials->m_roaa5Y = ptreeGetDouble(ptMetric, _T("roaa5Y"));
			pBasicFinancials->m_roae5Y = ptreeGetDouble(ptMetric, _T("roae5Y"));
			pBasicFinancials->m_roaeTTM = ptreeGetDouble(ptMetric, _T("roaeTTM"));
			pBasicFinancials->m_roaRfy = ptreeGetDouble(ptMetric, _T("roaRfy"));
			pBasicFinancials->m_roeRfy = ptreeGetDouble(ptMetric, _T("roeRfy"));
			pBasicFinancials->m_roeTTM = ptreeGetDouble(ptMetric, _T("roeTTM"));
			pBasicFinancials->m_roi5Y = ptreeGetDouble(ptMetric, _T("roi5Y"));
			pBasicFinancials->m_roiAnnual = ptreeGetDouble(ptMetric, _T("roiAnnual"));
			pBasicFinancials->m_roiTTM = ptreeGetDouble(ptMetric, _T("roiTTM"));

			pBasicFinancials->m_tangibleBookValuePerShareAnnual = ptreeGetDouble(ptMetric, _T("tangibleBookValuePerShareAnnual"));
			pBasicFinancials->m_tangibleBookValuePerShareQuarterly = ptreeGetDouble(ptMetric, _T("tangibleBookValuePerShareQuarterly"));
			pBasicFinancials->m_tbvCagr5Y = ptreeGetDouble(ptMetric, _T("tbvCagr5Y"));
			pBasicFinancials->m_totalDebtCagr5Y = ptreeGetDouble(ptMetric, _T("totalDebtCagr5Y"));
			pBasicFinancials->m_totalDebt_totalEquityAnnual = ptreeGetDouble(ptMetric, _T("totalDebt/totalEquityAnnual"));
			pBasicFinancials->m_totalDebt_totalEquityQuarterly = ptreeGetDouble(ptMetric, _T("totalDebt/totalEquityQuarterly"));

			pBasicFinancials->m_yearToDatePriceReturnDaily = ptreeGetDouble(ptMetric, _T("yearToDatePriceReturnDaily"));

			s = ppt->get<string>(_T("metricType"));
			ASSERT(s.compare(_T("all")) == 0);

			ptSeries = ppt->get_child(_T("series"));
			ptAnnual = ptSeries.get_child(_T("annual"));
			ptQuarterly = ptSeries.get_child(_T("quarterly"));

			ptCashRatio = ptAnnual.get_child(_T("cashRatio"));
			ParseVector(ptCashRatio, vData);
			for (int i = 0; i < vData.size(); i++) {
				pBasicFinancials->m_annual.m_cashRatio.push_back(vData[i]);
			}
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_cashRatio, _T("cashRatio"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_currentRatio, _T("currentRatio"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_ebitPerShare, _T("ebitPerShare"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_eps, _T("eps"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_grossMargin, _T("grossMargin"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_longtermDebtTotalAsset, _T("longtermDebtTotalAsset"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_longtermDebtTotalCapital, _T("longtermDebtTotalCapital"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_longtermDebtTotalEquity, _T("longtermDebtTotalEquity"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_netDebtToTotalCapital, _T("netDebtToTotalCapital"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_netDebtToTotalEquity, _T("netDebtToTotalEquity"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_netMargin, _T("netMargin"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_operatingMargin, _T("operatingMargin"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_pretaxMargin, _T("pretaxMargin"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_salesPerShare, _T("salesPerShare"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_sgaToSale, _T("sgaToSale"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_totalDebtToEquity, _T("totalDebtToEquity"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_totalDebtToTotalAsset, _T("totalDebtToTotalAsset"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_totalDebtToTotalCapital, _T("totalDebtToTotalCapital"));
			GetSeasonData(ptAnnual, pBasicFinancials->m_annual.m_totalRatio, _T("totalRatio"));

			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_cashRatio, _T("cashRatio"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_currentRatio, _T("currentRatio"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_ebitPerShare, _T("ebitPerShare"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_eps, _T("eps"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_grossMargin, _T("grossMargin"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_longtermDebtTotalAsset, _T("longtermDebtTotalAsset"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_longtermDebtTotalCapital, _T("longtermDebtTotalCapital"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_longtermDebtTotalEquity, _T("longtermDebtTotalEquity"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_netDebtToTotalCapital, _T("netDebtToTotalCapital"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_netDebtToTotalEquity, _T("netDebtToTotalEquity"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_netMargin, _T("netMargin"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_operatingMargin, _T("operatingMargin"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_pretaxMargin, _T("pretaxMargin"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_salesPerShare, _T("salesPerShare"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_sgaToSale, _T("sgaToSale"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_totalDebtToEquity, _T("totalDebtToEquity"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_totalDebtToTotalAsset, _T("totalDebtToTotalAsset"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_totalDebtToTotalCapital, _T("totalDebtToTotalCapital"));
			GetSeasonData(ptQuarterly, pBasicFinancials->m_quarterly.m_totalRatio, _T("totalRatio"));
		}
		catch (ptree_error& e) {
			ReportJSonErrorToSystemMessage(_T("Finnhub Stock basic financials "), e);
			return false;
		}
		return true;
	}
	return true;
}

bool CProductFinnhubCompanyBasicFinancials::GetSeasonData(ptree& pt, vector<strValue>& vData, const char* szMsg)
{
	vector<strValue> vDataTemp;
	ptree ptChild = pt.get_child(szMsg);
	ParseVector(ptChild, vDataTemp);
	for (int i = 0; i < vDataTemp.size(); i++) {
		vData.push_back(vDataTemp[i]);
	}
	return true;
}

bool CProductFinnhubCompanyBasicFinancials::ParseVector(ptree& ptData, vector<strValue>& vecData) {
	ptree pt2;

	for (ptree::iterator it = ptData.begin(); it != ptData.end(); ++it) {
		strValue sv{ 0, 0 };
		string sDate;
		int year, month, day;
		pt2 = it->second;
		sDate = pt2.get<string>(_T("period"));
		sscanf_s(sDate.c_str(), "%04d-%02d-%02d", &year, &month, &day);
		sv.m_period = year * 10000 + month * 100 + day;
		sv.m_value = ptreeGetDouble(pt2, _T("v"));
		vecData.push_back(sv);
	}

	return true;
}