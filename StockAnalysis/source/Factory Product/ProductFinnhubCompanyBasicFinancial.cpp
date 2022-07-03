#include "pch.h"

#include"accessory.h"
#include"jsonParse.h"
#include"worldMarket.h"

#include"FinnhubStockBasicFinancial.h"
#include "ProductFinnhubCompanyBasicFinancial.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCompanyBasicFinancial, CProductWebSourceData)

CProductFinnhubCompanyBasicFinancial::CProductFinnhubCompanyBasicFinancial() {
	m_strClassName = _T("Finnhub company basic financials");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/metric?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCompanyBasicFinancial::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMessage;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	strMessage = m_strInquiringStr + pStock->GetSymbol() + _T("&metric=all");
	pStock->SetBasicFinancialUpdated(true);

	return strMessage;
}

/// <summary>
/// Basic financials最新日期的更新，需要判断此基本金融数据是否存在。如果不存在的话，则每次查询后即自动更新日期，这样能够减少无效查询次数；
/// 如果存在有效数据的话，则为了能够及时提供最新数据，需要检查是否有新数据出现，有的话才更新日期，否则留待明日继续查询。
///
/// 查询时如果是外国的ADR，则返回的股票代码为其本土代码，与ADR不同，需要注意。
///
///
bool CProductFinnhubCompanyBasicFinancial::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CFinnhubStockBasicFinancialPtr pFinnhubStockBasicFinancial = nullptr;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	if (ParseFinnhubStockBasicFinancial(pFinnhubStockBasicFinancial, pWebData)) {
		// 因为接收到的股票代码是本土代码，可能与pStock中的不同（外国的ADR)，所以需要更新股票代码.
		// 例如申请BVDRF的金融数据，回复的股票代码为MBWS.PA
		pFinnhubStockBasicFinancial->m_symbol = pStock->GetSymbol();
		pStock->UpdateBasicFinancial(pFinnhubStockBasicFinancial);
		pStock->SetUpdateBasicFinancialDB(true);
	}
	pStock->SetBasicFinancialUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
	pStock->SetUpdateProfileDB(true);

	return true;
}

/// <summary>
/// Processes the web data.格式：
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
//    "metricType": "PerShare",
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
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CProductFinnhubCompanyBasicFinancial::ParseFinnhubStockBasicFinancial(CFinnhubStockBasicFinancialPtr& pBasicFinancial, CWebDataPtr pWebData) {
	string s;
	shared_ptr<ptree> ppt;
	ptree ptMetric, ptSeries, ptAnnual, ptQuarterly;
	vector<strValue> vData;
	int year, month, day;

	ASSERT(pWebData->IsJSonContentType());
	pBasicFinancial = std::make_shared<CFinnhubStockBasicFinancial>();
	if (pWebData->IsParsed()) {
		if (pWebData->IsVoidJSon()) return false;
		ppt = pWebData->GetPTree();
		try {
			s = ptreeGetString(*ppt, _T("symbol"));
			pBasicFinancial->m_symbol = s.c_str();

			if (ptreeGetChild(*ppt, _T("metric"), &ptMetric)) {
				pBasicFinancial->m_10DayAverageTradingVolume = ptreeGetDouble(ptMetric, _T("10DayAverageTradingVolume"));
				pBasicFinancial->m_13WeekPriceReturnDaily = ptreeGetDouble(ptMetric, _T("13WeekPriceReturnDaily"));
				pBasicFinancial->m_26WeekPriceReturnDaily = ptreeGetDouble(ptMetric, _T("26WeekPriceReturnDaily"));
				pBasicFinancial->m_3MonthAverageTradingVolume = ptreeGetDouble(ptMetric, _T("3MonthAverageTradingVolume"));
				pBasicFinancial->m_52WeekHigh = ptreeGetDouble(ptMetric, _T("52WeekHigh"));
				pBasicFinancial->m_52WeekLow = ptreeGetDouble(ptMetric, _T("52WeekLow"));
				s = ptreeGetString(ptMetric, _T("52WeekHighDate"));
				if (s.size() > 0) {
					sscanf_s(s.c_str(), _T("%04d-%02d-%02d"), &year, &month, &day);
					pBasicFinancial->m_52WeekHighDate = year * 10000 + month * 100 + day;
				}
				s = ptreeGetString(ptMetric, _T("52WeekLowDate"));
				if (s.size() > 0) {
					sscanf_s(s.c_str(), _T("%04d-%02d-%02d"), &year, &month, &day);
					pBasicFinancial->m_52WeekLowDate = year * 10000 + month * 100 + day;
				}
				pBasicFinancial->m_52WeekPriceReturnDaily = ptreeGetDouble(ptMetric, _T("52WeekPriceReturnDaily"));
				pBasicFinancial->m_5DayPriceReturnDaily = ptreeGetDouble(ptMetric, _T("5DayPriceReturnDaily"));

				pBasicFinancial->m_assetTurnoverAnnual = ptreeGetDouble(ptMetric, _T("assetTurnoverAnnual"));
				pBasicFinancial->m_assetTurnoverTTM = ptreeGetDouble(ptMetric, _T("assetTurnoverTTM"));

				pBasicFinancial->m_beta = ptreeGetDouble(ptMetric, _T("beta"));
				pBasicFinancial->m_bookValuePerShareAnnual = ptreeGetDouble(ptMetric, _T("bookValuePerShareAnnual"));
				pBasicFinancial->m_bookValuePerShareQuarterly = ptreeGetDouble(ptMetric, _T("bookValuePerShareQuarterly"));
				pBasicFinancial->m_bookValueShareGrowth5Y = ptreeGetDouble(ptMetric, _T("bookValueShareGrowth5Y"));

				pBasicFinancial->m_capitalSpendingGrowth5Y = ptreeGetDouble(ptMetric, _T("capitalSpendingGrowth5Y"));
				pBasicFinancial->m_cashFlowPerShareAnnual = ptreeGetDouble(ptMetric, _T("cashFlowPerShareAnnual"));
				pBasicFinancial->m_cashFlowPerShareTTM = ptreeGetDouble(ptMetric, _T("cashFlowPerShareTTM"));
				pBasicFinancial->m_cashPerSharePerShareAnnual = ptreeGetDouble(ptMetric, _T("cashPerSharePerShareAnnual"));
				pBasicFinancial->m_cashPerSharePerShareQuarterly = ptreeGetDouble(ptMetric, _T("cashPerSharePerShareQuarterly"));
				pBasicFinancial->m_currentEV_freeCashFlowAnnual = ptreeGetDouble(ptMetric, _T("currentEv/freeCashFlowAnnual"));
				pBasicFinancial->m_currentEV_freeCashFlowTTM = ptreeGetDouble(ptMetric, _T("currentEv/freeCashFlowTTM"));
				pBasicFinancial->m_currentDividendYieldTTM = ptreeGetDouble(ptMetric, _T("currentDividendYieldTTM"));
				pBasicFinancial->m_currentRatioAnnual = ptreeGetDouble(ptMetric, _T("currentRatioAnnual"));
				pBasicFinancial->m_currentRatioQuarterly = ptreeGetDouble(ptMetric, _T("currentRatioQuarterly"));

				pBasicFinancial->m_dividendGrowthRate5Y = ptreeGetDouble(ptMetric, _T("dividendGrowthRate5Y"));
				pBasicFinancial->m_dividendPerShare5Y = ptreeGetDouble(ptMetric, _T("dividendPerShare5Y"));
				pBasicFinancial->m_dividendPerShareAnnual = ptreeGetDouble(ptMetric, _T("dividendPerShareAnnual"));
				pBasicFinancial->m_dividendsPerShareTTM = ptreeGetDouble(ptMetric, _T("dividendsPerShareTTM"));
				pBasicFinancial->m_dividendYield5Y = ptreeGetDouble(ptMetric, _T("dividendYield5Y"));
				pBasicFinancial->m_dividendYieldIndicatedAnnual = ptreeGetDouble(ptMetric, _T("dividendYieldIndicatedAnnual"));

				pBasicFinancial->m_ebitdaCagr5Y = ptreeGetDouble(ptMetric, _T("ebitdaCagr5Y"));
				pBasicFinancial->m_ebitdaInterimCagr5Y = ptreeGetDouble(ptMetric, _T("ebitdaInterimCagr5Y"));
				pBasicFinancial->m_ebitdPerShareTTM = ptreeGetDouble(ptMetric, _T("ebitdPerShareTTM"));
				pBasicFinancial->m_epsBasicExclExtraItemsAnnual = ptreeGetDouble(ptMetric, _T("epsBasicExclExtraItemsAnnual"));
				pBasicFinancial->m_epsBasicExclExtraItemsTTM = ptreeGetDouble(ptMetric, _T("epsBasicExclExtraItemsTTM"));
				pBasicFinancial->m_epsExclExtraItemsAnnual = ptreeGetDouble(ptMetric, _T("epsExclExtraItemsAnnual"));
				pBasicFinancial->m_epsExclExtraItemsTTM = ptreeGetDouble(ptMetric, _T("epsExclExtraItemsTTM"));
				pBasicFinancial->m_epsGrowth3Y = ptreeGetDouble(ptMetric, _T("epsGrowth3Y"));
				pBasicFinancial->m_epsGrowth5Y = ptreeGetDouble(ptMetric, _T("epsGrowth5Y"));
				pBasicFinancial->m_epsGrowthQuarterlyYoy = ptreeGetDouble(ptMetric, _T("epsGrowthQuarterlyYoy"));
				pBasicFinancial->m_epsGrowthTTMYoy = ptreeGetDouble(ptMetric, _T("epsGrowthTTMYoy"));
				pBasicFinancial->m_epsInclExtraItemsAnnual = ptreeGetDouble(ptMetric, _T("epsInclExtraItemsAnnual"));
				pBasicFinancial->m_epsInclExtraItemsTTM = ptreeGetDouble(ptMetric, _T("epsInclExtraItemsTTM"));
				pBasicFinancial->m_epsNormalizedAnnual = ptreeGetDouble(ptMetric, _T("epsNormalizedAnnual"));

				pBasicFinancial->m_focfCagr5Y = ptreeGetDouble(ptMetric, _T("focfCagr5Y"));
				pBasicFinancial->m_freeCashFlowAnnual = ptreeGetDouble(ptMetric, _T("freeCashFlowAnnual"));
				pBasicFinancial->m_freeCashFlowPerShareTTM = ptreeGetDouble(ptMetric, _T("freeCashFlowPerShareTTM"));
				pBasicFinancial->m_freeCashFlowTTM = ptreeGetDouble(ptMetric, _T("freeCashFlowTTM"));
				pBasicFinancial->m_freeOperatingCashFlow_revenue5Y = ptreeGetDouble(ptMetric, _T("freeOperatingCashFlow/revenue5Y"));
				pBasicFinancial->m_freeOperatingCashFlow_revenueTTM = ptreeGetDouble(ptMetric, _T("freeOperatingCashFlow/revenueTTM"));

				pBasicFinancial->m_grossMargin5Y = ptreeGetDouble(ptMetric, _T("grossMargin5Y"));
				pBasicFinancial->m_grossMarginAnnual = ptreeGetDouble(ptMetric, _T("grossMarginAnnual"));
				pBasicFinancial->m_grossMarginTTM = ptreeGetDouble(ptMetric, _T("grossMarginTTM"));

				pBasicFinancial->m_inventoryTurnoverAnnual = ptreeGetDouble(ptMetric, _T("inventoryTurnoverAnnual"));
				pBasicFinancial->m_inventoryTurnoverTTM = ptreeGetDouble(ptMetric, _T("inventoryTurnoverTTM"));

				pBasicFinancial->m_longTermDebt_equityAnnual = ptreeGetDouble(ptMetric, _T("longTermDebt/equityAnnual"));
				pBasicFinancial->m_longTermDebt_equityQuarterly = ptreeGetDouble(ptMetric, _T("longTermDebt/equityQuarterly"));

				pBasicFinancial->m_marketCapitalization = ptreeGetDouble(ptMetric, _T("marketCapitalization"));
				pBasicFinancial->m_monthToDatePriceReturnDaily = ptreeGetDouble(ptMetric, _T("monthToDatePriceReturnDaily"));

				pBasicFinancial->m_netDebtAnnual = ptreeGetDouble(ptMetric, _T("netDebtAnnual"));
				pBasicFinancial->m_netDebtInterim = ptreeGetDouble(ptMetric, _T("netDebtInterim"));
				pBasicFinancial->m_netIncomeEmployeeAnnual = ptreeGetDouble(ptMetric, _T("netIncomeEmployeeAnnual"));
				pBasicFinancial->m_netIncomeEmployeeTTM = ptreeGetDouble(ptMetric, _T("netIncomeEmployeeTTM"));
				pBasicFinancial->m_netInterestCoverageAnnual = ptreeGetDouble(ptMetric, _T("netInterestCoverageAnnual"));
				pBasicFinancial->m_netInterestCoverageTTM = ptreeGetDouble(ptMetric, _T("netInterestCoverageTTM"));
				pBasicFinancial->m_netMarginGrowth5Y = ptreeGetDouble(ptMetric, _T("netMarginGrowth5Y"));
				pBasicFinancial->m_netProfitMargin5Y = ptreeGetDouble(ptMetric, _T("netProfitMargin5Y"));
				pBasicFinancial->m_netProfitMarginAnnual = ptreeGetDouble(ptMetric, _T("netProfitMarginAnnual"));
				pBasicFinancial->m_netProfitMarginTTM = ptreeGetDouble(ptMetric, _T("netProfitMarginTTM"));

				pBasicFinancial->m_payoutRatioAnnual = ptreeGetDouble(ptMetric, _T("payoutRatioAnnual"));
				pBasicFinancial->m_payoutRatioTTM = ptreeGetDouble(ptMetric, _T("payoutRatioTTM"));
				pBasicFinancial->m_pbAnnual = ptreeGetDouble(ptMetric, _T("pbAnnual"));
				pBasicFinancial->m_pbQuarterly = ptreeGetDouble(ptMetric, _T("pbQuarterly"));
				pBasicFinancial->m_pcfShareTTM = ptreeGetDouble(ptMetric, _T("pcfShareTTM"));
				pBasicFinancial->m_peBasicExclExtraTTM = ptreeGetDouble(ptMetric, _T("peBasicExclExtraTTM"));
				pBasicFinancial->m_peExclExtraAnnual = ptreeGetDouble(ptMetric, _T("peExclExtraAnnual"));
				pBasicFinancial->m_peExclExtraHighTTM = ptreeGetDouble(ptMetric, _T("peExclExtraHighTTM"));
				pBasicFinancial->m_peExclExtraTTM = ptreeGetDouble(ptMetric, _T("peExclExtraTTM"));
				pBasicFinancial->m_peExclLowTTM = ptreeGetDouble(ptMetric, _T("peExclLowTTM"));
				pBasicFinancial->m_peInclExtraTTM = ptreeGetDouble(ptMetric, _T("peInclExtraTTM"));
				pBasicFinancial->m_peNormalizedAnnual = ptreeGetDouble(ptMetric, _T("peNormalizedAnnual"));
				pBasicFinancial->m_pfcfShareAnnual = ptreeGetDouble(ptMetric, _T("pfcfShareAnnual"));
				pBasicFinancial->m_pfcfShareTTM = ptreeGetDouble(ptMetric, _T("pfcfShareTTM"));
				pBasicFinancial->m_pretaxMargin5Y = ptreeGetDouble(ptMetric, _T("pretaxMargin5Y"));
				pBasicFinancial->m_pretaxMarginAnnual = ptreeGetDouble(ptMetric, _T("pretaxMarginAnnual"));
				pBasicFinancial->m_pretaxMarginTTM = ptreeGetDouble(ptMetric, _T("pretaxMarginTTM"));
				pBasicFinancial->m_priceRelativeToSP50013Week = ptreeGetDouble(ptMetric, _T("priceRelativeToS&P50013Week"));
				pBasicFinancial->m_priceRelativeToSP50026Week = ptreeGetDouble(ptMetric, _T("priceRelativeToS&P50026Week"));
				pBasicFinancial->m_priceRelativeToSP5004Week = ptreeGetDouble(ptMetric, _T("priceRelativeToS&P5004Week"));
				pBasicFinancial->m_priceRelativeToSP50052Week = ptreeGetDouble(ptMetric, _T("priceRelativeToS&P50052Week"));
				pBasicFinancial->m_priceRelativeToSP500Ytd = ptreeGetDouble(ptMetric, _T("priceRelativeToS&P500Ytd"));
				pBasicFinancial->m_psAnnual = ptreeGetDouble(ptMetric, _T("psAnnual"));
				pBasicFinancial->m_psTTM = ptreeGetDouble(ptMetric, _T("psTTM"));
				pBasicFinancial->m_ptbvAnnual = ptreeGetDouble(ptMetric, _T("ptbvAnnual"));
				pBasicFinancial->m_ptbvQuarterly = ptreeGetDouble(ptMetric, _T("ptbvQuarterly"));

				pBasicFinancial->m_quickRatioAnnual = ptreeGetDouble(ptMetric, _T("quickRatioAnnual"));
				pBasicFinancial->m_quickRatioQuarterly = ptreeGetDouble(ptMetric, _T("quickRatioQuarterly"));

				pBasicFinancial->m_receivablesTurnoverAnnual = ptreeGetDouble(ptMetric, _T("receivablesTurnoverAnnual"));
				pBasicFinancial->m_receivablesTurnoverTTM = ptreeGetDouble(ptMetric, _T("receivablesTurnoverTTM"));
				pBasicFinancial->m_revenueEmployeeAnnual = ptreeGetDouble(ptMetric, _T("revenueEmployeeAnnual"));
				pBasicFinancial->m_revenueEmployeeTTM = ptreeGetDouble(ptMetric, _T("revenueEmployeeTTM"));
				pBasicFinancial->m_revenueGrowth3Y = ptreeGetDouble(ptMetric, _T("revenueGrowth3Y"));
				pBasicFinancial->m_revenueGrowth5Y = ptreeGetDouble(ptMetric, _T("revenueGrowth5Y"));
				pBasicFinancial->m_revenueGrowthQuarterlyYoy = ptreeGetDouble(ptMetric, _T("revenueGrowthQuarterlyYoy"));
				pBasicFinancial->m_revenueGrowthTTMYoy = ptreeGetDouble(ptMetric, _T("revenueGrowthTTMYoy"));
				pBasicFinancial->m_revenuePerShareAnnual = ptreeGetDouble(ptMetric, _T("revenuePerShareAnnual"));
				pBasicFinancial->m_revenuePerShareTTM = ptreeGetDouble(ptMetric, _T("revenuePerShareTTM"));
				pBasicFinancial->m_revenueShareGrowth5Y = ptreeGetDouble(ptMetric, _T("revenueShareGrowth5Y"));
				pBasicFinancial->m_roaa5Y = ptreeGetDouble(ptMetric, _T("roaa5Y"));
				pBasicFinancial->m_roae5Y = ptreeGetDouble(ptMetric, _T("roae5Y"));
				pBasicFinancial->m_roaeTTM = ptreeGetDouble(ptMetric, _T("roaeTTM"));
				pBasicFinancial->m_roaRfy = ptreeGetDouble(ptMetric, _T("roaRfy"));
				pBasicFinancial->m_roeRfy = ptreeGetDouble(ptMetric, _T("roeRfy"));
				pBasicFinancial->m_roeTTM = ptreeGetDouble(ptMetric, _T("roeTTM"));
				pBasicFinancial->m_roi5Y = ptreeGetDouble(ptMetric, _T("roi5Y"));
				pBasicFinancial->m_roiAnnual = ptreeGetDouble(ptMetric, _T("roiAnnual"));
				pBasicFinancial->m_roiTTM = ptreeGetDouble(ptMetric, _T("roiTTM"));

				pBasicFinancial->m_tangibleBookValuePerShareAnnual = ptreeGetDouble(ptMetric, _T("tangibleBookValuePerShareAnnual"));
				pBasicFinancial->m_tangibleBookValuePerShareQuarterly = ptreeGetDouble(ptMetric, _T("tangibleBookValuePerShareQuarterly"));
				pBasicFinancial->m_tbvCagr5Y = ptreeGetDouble(ptMetric, _T("tbvCagr5Y"));
				pBasicFinancial->m_totalDebtCagr5Y = ptreeGetDouble(ptMetric, _T("totalDebtCagr5Y"));
				pBasicFinancial->m_totalDebt_totalEquityAnnual = ptreeGetDouble(ptMetric, _T("totalDebt/totalEquityAnnual"));
				pBasicFinancial->m_totalDebt_totalEquityQuarterly = ptreeGetDouble(ptMetric, _T("totalDebt/totalEquityQuarterly"));

				pBasicFinancial->m_yearToDatePriceReturnDaily = ptreeGetDouble(ptMetric, _T("yearToDatePriceReturnDaily"));
			} // metric
			s = ptreeGetString(*ppt, _T("metricType"));
			ASSERT((s.compare(_T("all")) == 0) || (s.compare(_T("perShare")) == 0)); // 例子中返回的是all，但实际返回的是perShare

			if (ptreeGetChild(*ppt, _T("series"), &ptSeries)) {
				if (ptreeGetChild(ptSeries, _T("annual"), &ptAnnual)) {
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_cashRatio, _T("cashRatio"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_currentRatio, _T("currentRatio"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_ebitPerShare, _T("ebitPerShare"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_eps, _T("eps"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_grossMargin, _T("grossMargin"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_longtermDebtTotalAsset, _T("longtermDebtTotalAsset"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_longtermDebtTotalCapital, _T("longtermDebtTotalCapital"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_longtermDebtTotalEquity, _T("longtermDebtTotalEquity"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_netDebtToTotalCapital, _T("netDebtToTotalCapital"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_netDebtToTotalEquity, _T("netDebtToTotalEquity"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_netMargin, _T("netMargin"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_operatingMargin, _T("operatingMargin"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_pretaxMargin, _T("pretaxMargin"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_salesPerShare, _T("salesPerShare"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_sgaToSale, _T("sgaToSale"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_totalDebtToEquity, _T("totalDebtToEquity"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_totalDebtToTotalAsset, _T("totalDebtToTotalAsset"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_totalDebtToTotalCapital, _T("totalDebtToTotalCapital"));
					GetSeasonData(ptAnnual, pBasicFinancial->m_annual.m_totalRatio, _T("totalRatio"));
				} // annual

				if (ptreeGetChild(ptSeries, _T("quarterly"), &ptQuarterly)) {
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_cashRatio, _T("cashRatio"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_currentRatio, _T("currentRatio"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_ebitPerShare, _T("ebitPerShare"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_eps, _T("eps"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_grossMargin, _T("grossMargin"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_longtermDebtTotalAsset, _T("longtermDebtTotalAsset"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_longtermDebtTotalCapital, _T("longtermDebtTotalCapital"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_longtermDebtTotalEquity, _T("longtermDebtTotalEquity"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_netDebtToTotalCapital, _T("netDebtToTotalCapital"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_netDebtToTotalEquity, _T("netDebtToTotalEquity"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_netMargin, _T("netMargin"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_operatingMargin, _T("operatingMargin"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_pretaxMargin, _T("pretaxMargin"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_salesPerShare, _T("salesPerShare"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_sgaToSale, _T("sgaToSale"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_totalDebtToEquity, _T("totalDebtToEquity"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_totalDebtToTotalAsset, _T("totalDebtToTotalAsset"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_totalDebtToTotalCapital, _T("totalDebtToTotalCapital"));
					GetSeasonData(ptQuarterly, pBasicFinancial->m_quarter.m_totalRatio, _T("totalRatio"));
				} // quarterly
			} // series
		}
		catch (ptree_error& e) {
			ReportJSonErrorToSystemMessage(_T("Finnhub Stock basic financials "), e);
			return false;
		}
		return true;
	}
	return false;
}

bool CProductFinnhubCompanyBasicFinancial::GetSeasonData(ptree& pt, vector<strValue>& vData, const char* szMsg)
{
	vector<strValue> vDataTemp;
	try {
		ptree ptChild;
		if (ptreeGetChild(pt, szMsg, &ptChild)) {
			ParseVector(ptChild, vDataTemp);
			for (int i = 0; i < vDataTemp.size(); i++) {
				vData.push_back(vDataTemp[i]);
			}
		}
	}
	catch (ptree_error&) {
		// do nothing
	}
	return true;
}

bool CProductFinnhubCompanyBasicFinancial::ParseVector(ptree& ptData, vector<strValue>& vecData) {
	ptree pt2;

	try {
		for (ptree::iterator it = ptData.begin(); it != ptData.end(); ++it) {
			strValue sv{ 0, 0 };
			string sDate;
			int year{ 0 }, month{ 0 }, day{ 0 };
			pt2 = it->second;
			if ((sDate = ptreeGetString(pt2, _T("period"))) != _T("")) {
				sscanf_s(sDate.c_str(), "%04d-%02d-%02d", &year, &month, &day);
				sv.m_period = year * 10000 + month * 100 + day;
				sv.m_value = ptreeGetDouble(pt2, _T("v"));
				vecData.push_back(sv);
			}
		}
	}
	catch (ptree_error&) {
		// just skip
	}

	return true;
}