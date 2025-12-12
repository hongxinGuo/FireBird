#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"worldMarket.h"

#include"FinnhubStockBasicFinancial.h"
#include "ProductFinnhubCompanyBasicFinancial.h"

#include "TimeConvert.h"
#include "WebData.h"

std::set<string> s_setMetricType;// 目前共五种类型："all", "perShare", "marketCapitalization","metric","eps"

CProductFinnhubCompanyBasicFinancial::CProductFinnhubCompanyBasicFinancial() {
	s_setMetricType.clear();
	s_setMetricType.insert("all");
	s_setMetricType.insert("perShare");
	s_setMetricType.insert("marketCapitalization");
	s_setMetricType.insert("metric");
	s_setMetricType.insert("eps");

	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/metric?symbol=";
}

string CProductFinnhubCompanyBasicFinancial::CreateMessage() {
	const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();

	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol() + "&metric=all";
	return m_strInquiry;
}

// <summary>
// Basic financials最新日期的更新，需要判断此基本金融数据是否存在。如果不存在的话，则每次查询后即自动更新日期，这样能够减少无效查询次数；
// 如果存在有效数据的话，则为了能够及时提供最新数据，需要检查是否有新数据出现，有的话才更新日期，否则留待明日继续查询。
//
// 查询时如果是外国的ADR，则返回的股票代码为其本土代码，与ADR不同，需要注意。如申请BVDRF,回复的股票代码为MBWS.PA。
//
//
void CProductFinnhubCompanyBasicFinancial::ParseAndStoreWebData(CWebDataPtr pWebData) {
	CFinnhubStockBasicFinancialPtr pFinnhubStockBasicFinancial = nullptr;
	const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	pFinnhubStockBasicFinancial = ParseFinnhubStockBasicFinancial(pWebData);
	if (pFinnhubStockBasicFinancial != nullptr) {
		// 因为接收到的股票代码是本土代码，可能与pStock中的不同（外国的ADR)，所以需要更新股票代码.
		// 例如申请BVDRF的金融数据，回复的股票代码为MBWS.PA
		pFinnhubStockBasicFinancial->m_symbol = pStock->GetSymbol(); // 使用pStock中的股票代码
		pStock->UpdateBasicFinancial(pFinnhubStockBasicFinancial);
		pStock->SetUpdateBasicFinancialDB(true);
	}
}

void CProductFinnhubCompanyBasicFinancial::UpdateSystemStatus() {
	const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	pStock->SetBasicFinancialUpdateDate(GetMarket()->GetMarketDate());
	pStock->SetUpdateBasicFinancial(false);
	pStock->SetUpdateProfileDB(true);
}

// <summary>
// Processes the web data.格式
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
CFinnhubStockBasicFinancialPtr CProductFinnhubCompanyBasicFinancial::ParseFinnhubStockBasicFinancial(CWebDataPtr pWebData) {
	string s;
	json ptMetric, ptSeries, ptAnnual, ptQuarterly;
	vector<CValueOfPeriod> vData;
	CFinnhubStockBasicFinancialPtr pBasicFinancial = nullptr;
	json js;

	ptQuarterly.clear();
	ptAnnual.clear();
	ptMetric.clear();
	ptSeries.clear();

	if (!pWebData->CreateJson(js)) return pBasicFinancial;
	if (!IsValidData(pWebData)) return pBasicFinancial;

	pBasicFinancial = std::make_shared<CFinnhubStockBasicFinancial>();
	try {
		s = js.at("symbol");
		pBasicFinancial->m_symbol = s;

		ptMetric = jsonGetChild(js, "metric");
		try {
			pBasicFinancial->m_10DayAverageTradingVolume = jsonGetDouble(ptMetric, "10DayAverageTradingVolume");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_13WeekPriceReturnDaily = jsonGetDouble(ptMetric, "13WeekPriceReturnDaily");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_26WeekPriceReturnDaily = jsonGetDouble(ptMetric, "26WeekPriceReturnDaily");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_3MonthAverageTradingVolume = jsonGetDouble(ptMetric, "3MonthAverageTradingVolume");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_52WeekHigh = jsonGetDouble(ptMetric, "52WeekHigh");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_52WeekLow = jsonGetDouble(ptMetric, "52WeekLow");
		} catch (json::exception&) {
		}
		try {
			s = jsonGetString(ptMetric, "52WeekLowDate");
			if (!s.empty()) {
				pBasicFinancial->m_52WeekLowDate = XferToYYYYMMDD(s);
			}
		} catch (json::exception&) {
		}
		try {
			s = jsonGetString(ptMetric, "52WeekHighDate");
			if (!s.empty()) {
				pBasicFinancial->m_52WeekHighDate = XferToYYYYMMDD(s);
			}
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_52WeekPriceReturnDaily = jsonGetDouble(ptMetric, "52WeekPriceReturnDaily");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_5DayPriceReturnDaily = jsonGetDouble(ptMetric, "5DayPriceReturnDaily");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_assetTurnoverAnnual = jsonGetDouble(ptMetric, "assetTurnoverAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_assetTurnoverTTM = jsonGetDouble(ptMetric, "assetTurnoverTTM");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_beta = jsonGetDouble(ptMetric, "beta");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_bookValuePerShareAnnual = jsonGetDouble(ptMetric, "bookValuePerShareAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_bookValuePerShareQuarterly = jsonGetDouble(ptMetric, "bookValuePerShareQuarterly");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_bookValueShareGrowth5Y = jsonGetDouble(ptMetric, "bookValueShareGrowth5Y");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_capitalSpendingGrowth5Y = jsonGetDouble(ptMetric, "capitalSpendingGrowth5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_cashFlowPerShareAnnual = jsonGetDouble(ptMetric, "cashFlowPerShareAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_cashFlowPerShareTTM = jsonGetDouble(ptMetric, "cashFlowPerShareTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_cashPerSharePerShareAnnual = jsonGetDouble(ptMetric, "cashPerSharePerShareAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_cashPerSharePerShareQuarterly = jsonGetDouble(ptMetric, "cashPerSharePerShareQuarterly");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_currentEV_freeCashFlowAnnual = jsonGetDouble(ptMetric, "currentEv/freeCashFlowAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_currentEV_freeCashFlowTTM = jsonGetDouble(ptMetric, "currentEv/freeCashFlowTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_currentDividendYieldTTM = jsonGetDouble(ptMetric, "currentDividendYieldTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_currentRatioAnnual = jsonGetDouble(ptMetric, "currentRatioAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_currentRatioQuarterly = jsonGetDouble(ptMetric, "currentRatioQuarterly");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_dividendGrowthRate5Y = jsonGetDouble(ptMetric, "dividendGrowthRate5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_dividendPerShare5Y = jsonGetDouble(ptMetric, "dividendPerShare5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_dividendPerShareAnnual = jsonGetDouble(ptMetric, "dividendPerShareAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_dividendsPerShareTTM = jsonGetDouble(ptMetric, "dividendsPerShareTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_dividendYield5Y = jsonGetDouble(ptMetric, "dividendYield5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_dividendYieldIndicatedAnnual = jsonGetDouble(ptMetric, "dividendYieldIndicatedAnnual");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_ebitdaCagr5Y = jsonGetDouble(ptMetric, "ebitdaCagr5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_ebitdaInterimCagr5Y = jsonGetDouble(ptMetric, "ebitdaInterimCagr5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_ebitdPerShareTTM = jsonGetDouble(ptMetric, "ebitdPerShareTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_epsBasicExclExtraItemsAnnual = jsonGetDouble(ptMetric, "epsBasicExclExtraItemsAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_epsBasicExclExtraItemsTTM = jsonGetDouble(ptMetric, "epsBasicExclExtraItemsTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_epsExclExtraItemsAnnual = jsonGetDouble(ptMetric, "epsExclExtraItemsAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_epsExclExtraItemsTTM = jsonGetDouble(ptMetric, "epsExclExtraItemsTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_epsGrowth3Y = jsonGetDouble(ptMetric, "epsGrowth3Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_epsGrowth5Y = jsonGetDouble(ptMetric, "epsGrowth5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_epsGrowthQuarterlyYoy = jsonGetDouble(ptMetric, "epsGrowthQuarterlyYoy");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_epsGrowthTTMYoy = jsonGetDouble(ptMetric, "epsGrowthTTMYoy");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_epsInclExtraItemsAnnual = jsonGetDouble(ptMetric, "epsInclExtraItemsAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_epsInclExtraItemsTTM = jsonGetDouble(ptMetric, "epsInclExtraItemsTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_epsNormalizedAnnual = jsonGetDouble(ptMetric, "epsNormalizedAnnual");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_focfCagr5Y = jsonGetDouble(ptMetric, "focfCagr5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_freeCashFlowAnnual = jsonGetDouble(ptMetric, "freeCashFlowAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_freeCashFlowPerShareTTM = jsonGetDouble(ptMetric, "freeCashFlowPerShareTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_freeCashFlowTTM = jsonGetDouble(ptMetric, "freeCashFlowTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_freeOperatingCashFlow_revenue5Y = jsonGetDouble(ptMetric, "freeOperatingCashFlow/revenue5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_freeOperatingCashFlow_revenueTTM = jsonGetDouble(ptMetric, "freeOperatingCashFlow/revenueTTM");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_grossMargin5Y = jsonGetDouble(ptMetric, "grossMargin5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_grossMarginAnnual = jsonGetDouble(ptMetric, "grossMarginAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_grossMarginTTM = jsonGetDouble(ptMetric, "grossMarginTTM");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_inventoryTurnoverAnnual = jsonGetDouble(ptMetric, "inventoryTurnoverAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_inventoryTurnoverTTM = jsonGetDouble(ptMetric, "inventoryTurnoverTTM");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_longTermDebt_equityAnnual = jsonGetDouble(ptMetric, "longTermDebt/equityAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_longTermDebt_equityQuarterly = jsonGetDouble(ptMetric, "longTermDebt/equityQuarterly");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_marketCapitalization = jsonGetDouble(ptMetric, "marketCapitalization");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_monthToDatePriceReturnDaily = jsonGetDouble(ptMetric, "monthToDatePriceReturnDaily");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_netDebtAnnual = jsonGetDouble(ptMetric, "netDebtAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_netDebtInterim = jsonGetDouble(ptMetric, "netDebtInterim");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_netIncomeEmployeeAnnual = jsonGetDouble(ptMetric, "netIncomeEmployeeAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_netIncomeEmployeeTTM = jsonGetDouble(ptMetric, "netIncomeEmployeeTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_netInterestCoverageAnnual = jsonGetDouble(ptMetric, "netInterestCoverageAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_netInterestCoverageTTM = jsonGetDouble(ptMetric, "netInterestCoverageTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_netMarginGrowth5Y = jsonGetDouble(ptMetric, "netMarginGrowth5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_netProfitMargin5Y = jsonGetDouble(ptMetric, "netProfitMargin5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_netProfitMarginAnnual = jsonGetDouble(ptMetric, "netProfitMarginAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_netProfitMarginTTM = jsonGetDouble(ptMetric, "netProfitMarginTTM");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_payoutRatioAnnual = jsonGetDouble(ptMetric, "payoutRatioAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_payoutRatioTTM = jsonGetDouble(ptMetric, "payoutRatioTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_pbAnnual = jsonGetDouble(ptMetric, "pbAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_pbQuarterly = jsonGetDouble(ptMetric, "pbQuarterly");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_pcfShareTTM = jsonGetDouble(ptMetric, "pcfShareTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_peBasicExclExtraTTM = jsonGetDouble(ptMetric, "peBasicExclExtraTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_peExclExtraAnnual = jsonGetDouble(ptMetric, "peExclExtraAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_peExclExtraHighTTM = jsonGetDouble(ptMetric, "peExclExtraHighTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_peExclExtraTTM = jsonGetDouble(ptMetric, "peExclExtraTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_peExclLowTTM = jsonGetDouble(ptMetric, "peExclLowTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_peInclExtraTTM = jsonGetDouble(ptMetric, "peInclExtraTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_peNormalizedAnnual = jsonGetDouble(ptMetric, "peNormalizedAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_pfcfShareAnnual = jsonGetDouble(ptMetric, "pfcfShareAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_pfcfShareTTM = jsonGetDouble(ptMetric, "pfcfShareTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_pretaxMargin5Y = jsonGetDouble(ptMetric, "pretaxMargin5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_pretaxMarginAnnual = jsonGetDouble(ptMetric, "pretaxMarginAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_pretaxMarginTTM = jsonGetDouble(ptMetric, "pretaxMarginTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_priceRelativeToSP50013Week = jsonGetDouble(ptMetric, "priceRelativeToS&P50013Week");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_priceRelativeToSP50026Week = jsonGetDouble(ptMetric, "priceRelativeToS&P50026Week");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_priceRelativeToSP5004Week = jsonGetDouble(ptMetric, "priceRelativeToS&P5004Week");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_priceRelativeToSP50052Week = jsonGetDouble(ptMetric, "priceRelativeToS&P50052Week");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_priceRelativeToSP500Ytd = jsonGetDouble(ptMetric, "priceRelativeToS&P500Ytd");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_psAnnual = jsonGetDouble(ptMetric, "psAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_psTTM = jsonGetDouble(ptMetric, "psTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_ptbvAnnual = jsonGetDouble(ptMetric, "ptbvAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_ptbvQuarterly = jsonGetDouble(ptMetric, "ptbvQuarterly");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_quickRatioAnnual = jsonGetDouble(ptMetric, "quickRatioAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_quickRatioQuarterly = jsonGetDouble(ptMetric, "quickRatioQuarterly");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_receivablesTurnoverAnnual = jsonGetDouble(ptMetric, "receivablesTurnoverAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_receivablesTurnoverTTM = jsonGetDouble(ptMetric, "receivablesTurnoverTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueEmployeeAnnual = jsonGetDouble(ptMetric, "revenueEmployeeAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueEmployeeTTM = jsonGetDouble(ptMetric, "revenueEmployeeTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueGrowth3Y = jsonGetDouble(ptMetric, "revenueGrowth3Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueGrowth5Y = jsonGetDouble(ptMetric, "revenueGrowth5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueGrowthQuarterlyYoy = jsonGetDouble(ptMetric, "revenueGrowthQuarterlyYoy");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueGrowthTTMYoy = jsonGetDouble(ptMetric, "revenueGrowthTTMYoy");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_revenuePerShareAnnual = jsonGetDouble(ptMetric, "revenuePerShareAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_revenuePerShareTTM = jsonGetDouble(ptMetric, "revenuePerShareTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueShareGrowth5Y = jsonGetDouble(ptMetric, "revenueShareGrowth5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_roaa5Y = jsonGetDouble(ptMetric, "roaa5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_roae5Y = jsonGetDouble(ptMetric, "roae5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_roaeTTM = jsonGetDouble(ptMetric, "roaeTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_roaRfy = jsonGetDouble(ptMetric, "roaRfy");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_roeRfy = jsonGetDouble(ptMetric, "roeRfy");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_roeTTM = jsonGetDouble(ptMetric, "roeTTM");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_roi5Y = jsonGetDouble(ptMetric, "roi5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_roiAnnual = jsonGetDouble(ptMetric, "roiAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_roiTTM = jsonGetDouble(ptMetric, "roiTTM");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_tangibleBookValuePerShareAnnual = jsonGetDouble(ptMetric, "tangibleBookValuePerShareAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_tangibleBookValuePerShareQuarterly = jsonGetDouble(ptMetric, "tangibleBookValuePerShareQuarterly");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_tbvCagr5Y = jsonGetDouble(ptMetric, "tbvCagr5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_totalDebtCagr5Y = jsonGetDouble(ptMetric, "totalDebtCagr5Y");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_totalDebt_totalEquityAnnual = jsonGetDouble(ptMetric, "totalDebt/totalEquityAnnual");
		} catch (json::exception&) {
		}
		try {
			pBasicFinancial->m_totalDebt_totalEquityQuarterly = jsonGetDouble(ptMetric, "totalDebt/totalEquityQuarterly");
		} catch (json::exception&) {
		}

		try {
			pBasicFinancial->m_yearToDatePriceReturnDaily = jsonGetDouble(ptMetric, "yearToDatePriceReturnDaily");
		} catch (json::exception&) {
		}

		s = js.at("metricType"); // 目前共五种类型："all", "perShare", "marketCapitalization","metric","eps"
		if (!s_setMetricType.contains(s)) {
			string str = " metric type out of range: ";
			str += s;
			str += "   Inquiry string:  ";
			str += m_strInquiry;
			gl_systemMessage.PushInnerSystemInformationMessage(str);
		}

		ptSeries = jsonGetChild(js, "series");
		if (!ptSeries.empty()) {
			bool fAnnualEmpty = false;
			bool fQuarterlyEmpty = false;
			try { ptAnnual = jsonGetChild(&ptSeries, "annual"); } catch (json::exception&) { fAnnualEmpty = true; }
			if (!fAnnualEmpty && !ptAnnual.empty()) {
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_cashRatio, "cashRatio");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_currentRatio, "currentRatio");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_ebitPerShare, "ebitPerShare");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_eps, "eps");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_grossMargin, "grossMargin");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_longtermDebtTotalAsset, "longtermDebtTotalAsset");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_longtermDebtTotalCapital, "longtermDebtTotalCapital");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_longtermDebtTotalEquity, "longtermDebtTotalEquity");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_netDebtToTotalCapital, "netDebtToTotalCapital");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_netDebtToTotalEquity, "netDebtToTotalEquity");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_netMargin, "netMargin");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_operatingMargin, "operatingMargin");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_pretaxMargin, "pretaxMargin");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_salesPerShare, "salesPerShare");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_sgaToSale, "sgaToSale");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_totalDebtToEquity, "totalDebtToEquity");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_totalDebtToTotalAsset, "totalDebtToTotalAsset");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_totalDebtToTotalCapital, "totalDebtToTotalCapital");
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_totalRatio, "totalRatio");
			}
			try { ptQuarterly = jsonGetChild(&ptSeries, "quarterly"); } catch (json::exception&) { fQuarterlyEmpty = true; }
			if (!fQuarterlyEmpty && !ptQuarterly.empty()) {
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_cashRatio, "cashRatio");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_currentRatio, "currentRatio");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_ebitPerShare, "ebitPerShare");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_eps, "eps");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_grossMargin, "grossMargin");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_longtermDebtTotalAsset, "longtermDebtTotalAsset");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_longtermDebtTotalCapital, "longtermDebtTotalCapital");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_longtermDebtTotalEquity, "longtermDebtTotalEquity");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_netDebtToTotalCapital, "netDebtToTotalCapital");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_netDebtToTotalEquity, "netDebtToTotalEquity");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_netMargin, "netMargin");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_operatingMargin, "operatingMargin");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_pretaxMargin, "pretaxMargin");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_salesPerShare, "salesPerShare");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_sgaToSale, "sgaToSale");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_totalDebtToEquity, "totalDebtToEquity");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_totalDebtToTotalAsset, "totalDebtToTotalAsset");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_totalDebtToTotalCapital, "totalDebtToTotalCapital");
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_totalRatio, "totalRatio");
			}
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Stock basic financials ", e.what());
		return nullptr;
	}
	return pBasicFinancial;
}

void CProductFinnhubCompanyBasicFinancial::GetSeasonData(json* pjs, vector<CValueOfPeriod>& vData, const char* szMsg) {
	try {
		json ptChild;
		ptChild = jsonGetChild(pjs, szMsg);
		vector<CValueOfPeriod> vDataTemp;
		Parse(&ptChild, vDataTemp);
		for (size_t i = 0; i < vDataTemp.size(); i++) {
			vData.push_back(vDataTemp[i]);
		}
	} catch (json::exception&) {
		// do nothing
	}
}

//
// [{"period":"2021-03-31", "v" : -2.8522}, { "period":"2020-12-31","v" : -0.6159 }]
//
void CProductFinnhubCompanyBasicFinancial::Parse(json* pjs, vector<CValueOfPeriod>& vecData) {
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			CValueOfPeriod sv{ 0, 0 };
			string sDate;
			sDate = jsonGetString(it, "period");
			if (!sDate.empty()) {
				sv.m_period = XferToYYYYMMDD(sDate);
				sv.m_value = jsonGetDouble(it, "v");
				vecData.push_back(sv);
			}
		}
	} catch (json::exception&) {
		// just skip
	}
}
