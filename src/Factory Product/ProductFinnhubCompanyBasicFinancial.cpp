#include "pch.h"

#include"SystemMessage.h"

#include"jsonParse.h"
import FireBird.Accessory.JsonGetValue;

#include"worldMarket.h"

#include"FinnhubStockBasicFinancial.h"
#include "ProductFinnhubCompanyBasicFinancial.h"

import FireBird.Accessory.TimeConvert;

std::set<string> s_setMetricType;// 目前共五种类型："all", "perShare", "marketCapitalization","metric","eps"

CProductFinnhubCompanyBasicFinancial::CProductFinnhubCompanyBasicFinancial() {
	s_setMetricType.clear();
	s_setMetricType.insert(_T("all"));
	s_setMetricType.insert(_T("perShare"));
	s_setMetricType.insert(_T("marketCapitalization"));
	s_setMetricType.insert(_T("metric"));
	s_setMetricType.insert(_T("eps"));

	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/metric?symbol=");
}

CString CProductFinnhubCompanyBasicFinancial::CreateMessage() {
	CString strMessage;
	const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();

	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol() + _T("&metric=all");
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
	const CFinnhubStockPtr pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);
	pFinnhubStockBasicFinancial = ParseFinnhubStockBasicFinancial(pWebData);
	if (pFinnhubStockBasicFinancial != nullptr) {
		// 因为接收到的股票代码是本土代码，可能与pStock中的不同（外国的ADR)，所以需要更新股票代码.
		// 例如申请BVDRF的金融数据，回复的股票代码为MBWS.PA
		pFinnhubStockBasicFinancial->m_symbol = pStock->GetSymbol();
		pStock->UpdateBasicFinancial(pFinnhubStockBasicFinancial);
		pStock->SetUpdateBasicFinancialDB(true);
	}
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
	nlohmann::ordered_json ptMetric, ptSeries, ptAnnual, ptQuarterly;
	vector<CValueOfPeriod> vData;
	int year, month, day;
	CFinnhubStockBasicFinancialPtr pBasicFinancial = nullptr;
	nlohmann::ordered_json js;

	ptQuarterly.clear();
	ptAnnual.clear();
	ptMetric.clear();
	ptSeries.clear();

	if (!pWebData->CreateJson(js)) return pBasicFinancial;
	if (!IsValidData(pWebData)) return pBasicFinancial;

	pBasicFinancial = std::make_shared<CFinnhubStockBasicFinancial>();
	try {
		s = js.at(_T("symbol"));
		pBasicFinancial->m_symbol = s.c_str();

		ptMetric = jsonGetChild(js, _T("metric"));
		try {
			pBasicFinancial->m_10DayAverageTradingVolume = jsonGetDouble(ptMetric, _T("10DayAverageTradingVolume"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_13WeekPriceReturnDaily = jsonGetDouble(ptMetric, _T("13WeekPriceReturnDaily"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_26WeekPriceReturnDaily = jsonGetDouble(ptMetric, _T("26WeekPriceReturnDaily"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_3MonthAverageTradingVolume = jsonGetDouble(ptMetric, _T("3MonthAverageTradingVolume"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_52WeekHigh = jsonGetDouble(ptMetric, _T("52WeekHigh"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_52WeekLow = jsonGetDouble(ptMetric, _T("52WeekLow"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			s = jsonGetString(ptMetric, _T("52WeekHighDate"));
			if (!s.empty()) {
				static_cast<void>(sscanf_s(s.c_str(), _T("%04d-%02d-%02d"), &year, &month, &day));
				pBasicFinancial->m_52WeekHighDate = XferYearMonthDayToYYYYMMDD(year, month, day);
			}
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			s = jsonGetString(ptMetric, _T("52WeekLowDate"));
			if (!s.empty()) {
				static_cast<void>(sscanf_s(s.c_str(), _T("%04d-%02d-%02d"), &year, &month, &day));
				pBasicFinancial->m_52WeekLowDate = XferYearMonthDayToYYYYMMDD(year, month, day);
			}
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_52WeekPriceReturnDaily = jsonGetDouble(ptMetric, _T("52WeekPriceReturnDaily"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_5DayPriceReturnDaily = jsonGetDouble(ptMetric, _T("5DayPriceReturnDaily"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_assetTurnoverAnnual = jsonGetDouble(ptMetric, _T("assetTurnoverAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_assetTurnoverTTM = jsonGetDouble(ptMetric, _T("assetTurnoverTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_beta = jsonGetDouble(ptMetric, _T("beta"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_bookValuePerShareAnnual = jsonGetDouble(ptMetric, _T("bookValuePerShareAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_bookValuePerShareQuarterly = jsonGetDouble(ptMetric, _T("bookValuePerShareQuarterly"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_bookValueShareGrowth5Y = jsonGetDouble(ptMetric, _T("bookValueShareGrowth5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_capitalSpendingGrowth5Y = jsonGetDouble(ptMetric, _T("capitalSpendingGrowth5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_cashFlowPerShareAnnual = jsonGetDouble(ptMetric, _T("cashFlowPerShareAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_cashFlowPerShareTTM = jsonGetDouble(ptMetric, _T("cashFlowPerShareTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_cashPerSharePerShareAnnual = jsonGetDouble(ptMetric, _T("cashPerSharePerShareAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_cashPerSharePerShareQuarterly = jsonGetDouble(ptMetric, _T("cashPerSharePerShareQuarterly"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_currentEV_freeCashFlowAnnual = jsonGetDouble(ptMetric, _T("currentEv/freeCashFlowAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_currentEV_freeCashFlowTTM = jsonGetDouble(ptMetric, _T("currentEv/freeCashFlowTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_currentDividendYieldTTM = jsonGetDouble(ptMetric, _T("currentDividendYieldTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_currentRatioAnnual = jsonGetDouble(ptMetric, _T("currentRatioAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_currentRatioQuarterly = jsonGetDouble(ptMetric, _T("currentRatioQuarterly"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_dividendGrowthRate5Y = jsonGetDouble(ptMetric, _T("dividendGrowthRate5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_dividendPerShare5Y = jsonGetDouble(ptMetric, _T("dividendPerShare5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_dividendPerShareAnnual = jsonGetDouble(ptMetric, _T("dividendPerShareAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_dividendsPerShareTTM = jsonGetDouble(ptMetric, _T("dividendsPerShareTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_dividendYield5Y = jsonGetDouble(ptMetric, _T("dividendYield5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_dividendYieldIndicatedAnnual = jsonGetDouble(ptMetric, _T("dividendYieldIndicatedAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_ebitdaCagr5Y = jsonGetDouble(ptMetric, _T("ebitdaCagr5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_ebitdaInterimCagr5Y = jsonGetDouble(ptMetric, _T("ebitdaInterimCagr5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_ebitdPerShareTTM = jsonGetDouble(ptMetric, _T("ebitdPerShareTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_epsBasicExclExtraItemsAnnual = jsonGetDouble(ptMetric, _T("epsBasicExclExtraItemsAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_epsBasicExclExtraItemsTTM = jsonGetDouble(ptMetric, _T("epsBasicExclExtraItemsTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_epsExclExtraItemsAnnual = jsonGetDouble(ptMetric, _T("epsExclExtraItemsAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_epsExclExtraItemsTTM = jsonGetDouble(ptMetric, _T("epsExclExtraItemsTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_epsGrowth3Y = jsonGetDouble(ptMetric, _T("epsGrowth3Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_epsGrowth5Y = jsonGetDouble(ptMetric, _T("epsGrowth5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_epsGrowthQuarterlyYoy = jsonGetDouble(ptMetric, _T("epsGrowthQuarterlyYoy"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_epsGrowthTTMYoy = jsonGetDouble(ptMetric, _T("epsGrowthTTMYoy"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_epsInclExtraItemsAnnual = jsonGetDouble(ptMetric, _T("epsInclExtraItemsAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_epsInclExtraItemsTTM = jsonGetDouble(ptMetric, _T("epsInclExtraItemsTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_epsNormalizedAnnual = jsonGetDouble(ptMetric, _T("epsNormalizedAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_focfCagr5Y = jsonGetDouble(ptMetric, _T("focfCagr5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_freeCashFlowAnnual = jsonGetDouble(ptMetric, _T("freeCashFlowAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_freeCashFlowPerShareTTM = jsonGetDouble(ptMetric, _T("freeCashFlowPerShareTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_freeCashFlowTTM = jsonGetDouble(ptMetric, _T("freeCashFlowTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_freeOperatingCashFlow_revenue5Y = jsonGetDouble(ptMetric, _T("freeOperatingCashFlow/revenue5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_freeOperatingCashFlow_revenueTTM = jsonGetDouble(ptMetric, _T("freeOperatingCashFlow/revenueTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_grossMargin5Y = jsonGetDouble(ptMetric, _T("grossMargin5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_grossMarginAnnual = jsonGetDouble(ptMetric, _T("grossMarginAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_grossMarginTTM = jsonGetDouble(ptMetric, _T("grossMarginTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_inventoryTurnoverAnnual = jsonGetDouble(ptMetric, _T("inventoryTurnoverAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_inventoryTurnoverTTM = jsonGetDouble(ptMetric, _T("inventoryTurnoverTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_longTermDebt_equityAnnual = jsonGetDouble(ptMetric, _T("longTermDebt/equityAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_longTermDebt_equityQuarterly = jsonGetDouble(ptMetric, _T("longTermDebt/equityQuarterly"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_marketCapitalization = jsonGetDouble(ptMetric, _T("marketCapitalization"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_monthToDatePriceReturnDaily = jsonGetDouble(ptMetric, _T("monthToDatePriceReturnDaily"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_netDebtAnnual = jsonGetDouble(ptMetric, _T("netDebtAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_netDebtInterim = jsonGetDouble(ptMetric, _T("netDebtInterim"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_netIncomeEmployeeAnnual = jsonGetDouble(ptMetric, _T("netIncomeEmployeeAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_netIncomeEmployeeTTM = jsonGetDouble(ptMetric, _T("netIncomeEmployeeTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_netInterestCoverageAnnual = jsonGetDouble(ptMetric, _T("netInterestCoverageAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_netInterestCoverageTTM = jsonGetDouble(ptMetric, _T("netInterestCoverageTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_netMarginGrowth5Y = jsonGetDouble(ptMetric, _T("netMarginGrowth5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_netProfitMargin5Y = jsonGetDouble(ptMetric, _T("netProfitMargin5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_netProfitMarginAnnual = jsonGetDouble(ptMetric, _T("netProfitMarginAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_netProfitMarginTTM = jsonGetDouble(ptMetric, _T("netProfitMarginTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_payoutRatioAnnual = jsonGetDouble(ptMetric, _T("payoutRatioAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_payoutRatioTTM = jsonGetDouble(ptMetric, _T("payoutRatioTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_pbAnnual = jsonGetDouble(ptMetric, _T("pbAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_pbQuarterly = jsonGetDouble(ptMetric, _T("pbQuarterly"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_pcfShareTTM = jsonGetDouble(ptMetric, _T("pcfShareTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_peBasicExclExtraTTM = jsonGetDouble(ptMetric, _T("peBasicExclExtraTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_peExclExtraAnnual = jsonGetDouble(ptMetric, _T("peExclExtraAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_peExclExtraHighTTM = jsonGetDouble(ptMetric, _T("peExclExtraHighTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_peExclExtraTTM = jsonGetDouble(ptMetric, _T("peExclExtraTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_peExclLowTTM = jsonGetDouble(ptMetric, _T("peExclLowTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_peInclExtraTTM = jsonGetDouble(ptMetric, _T("peInclExtraTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_peNormalizedAnnual = jsonGetDouble(ptMetric, _T("peNormalizedAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_pfcfShareAnnual = jsonGetDouble(ptMetric, _T("pfcfShareAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_pfcfShareTTM = jsonGetDouble(ptMetric, _T("pfcfShareTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_pretaxMargin5Y = jsonGetDouble(ptMetric, _T("pretaxMargin5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_pretaxMarginAnnual = jsonGetDouble(ptMetric, _T("pretaxMarginAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_pretaxMarginTTM = jsonGetDouble(ptMetric, _T("pretaxMarginTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_priceRelativeToSP50013Week = jsonGetDouble(ptMetric, _T("priceRelativeToS&P50013Week"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_priceRelativeToSP50026Week = jsonGetDouble(ptMetric, _T("priceRelativeToS&P50026Week"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_priceRelativeToSP5004Week = jsonGetDouble(ptMetric, _T("priceRelativeToS&P5004Week"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_priceRelativeToSP50052Week = jsonGetDouble(ptMetric, _T("priceRelativeToS&P50052Week"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_priceRelativeToSP500Ytd = jsonGetDouble(ptMetric, _T("priceRelativeToS&P500Ytd"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_psAnnual = jsonGetDouble(ptMetric, _T("psAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_psTTM = jsonGetDouble(ptMetric, _T("psTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_ptbvAnnual = jsonGetDouble(ptMetric, _T("ptbvAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_ptbvQuarterly = jsonGetDouble(ptMetric, _T("ptbvQuarterly"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_quickRatioAnnual = jsonGetDouble(ptMetric, _T("quickRatioAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_quickRatioQuarterly = jsonGetDouble(ptMetric, _T("quickRatioQuarterly"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_receivablesTurnoverAnnual = jsonGetDouble(ptMetric, _T("receivablesTurnoverAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_receivablesTurnoverTTM = jsonGetDouble(ptMetric, _T("receivablesTurnoverTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueEmployeeAnnual = jsonGetDouble(ptMetric, _T("revenueEmployeeAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueEmployeeTTM = jsonGetDouble(ptMetric, _T("revenueEmployeeTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueGrowth3Y = jsonGetDouble(ptMetric, _T("revenueGrowth3Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueGrowth5Y = jsonGetDouble(ptMetric, _T("revenueGrowth5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueGrowthQuarterlyYoy = jsonGetDouble(ptMetric, _T("revenueGrowthQuarterlyYoy"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueGrowthTTMYoy = jsonGetDouble(ptMetric, _T("revenueGrowthTTMYoy"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_revenuePerShareAnnual = jsonGetDouble(ptMetric, _T("revenuePerShareAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_revenuePerShareTTM = jsonGetDouble(ptMetric, _T("revenuePerShareTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_revenueShareGrowth5Y = jsonGetDouble(ptMetric, _T("revenueShareGrowth5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_roaa5Y = jsonGetDouble(ptMetric, _T("roaa5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_roae5Y = jsonGetDouble(ptMetric, _T("roae5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_roaeTTM = jsonGetDouble(ptMetric, _T("roaeTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_roaRfy = jsonGetDouble(ptMetric, _T("roaRfy"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_roeRfy = jsonGetDouble(ptMetric, _T("roeRfy"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_roeTTM = jsonGetDouble(ptMetric, _T("roeTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_roi5Y = jsonGetDouble(ptMetric, _T("roi5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_roiAnnual = jsonGetDouble(ptMetric, _T("roiAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_roiTTM = jsonGetDouble(ptMetric, _T("roiTTM"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_tangibleBookValuePerShareAnnual = jsonGetDouble(ptMetric, _T("tangibleBookValuePerShareAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_tangibleBookValuePerShareQuarterly = jsonGetDouble(ptMetric, _T("tangibleBookValuePerShareQuarterly"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_tbvCagr5Y = jsonGetDouble(ptMetric, _T("tbvCagr5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_totalDebtCagr5Y = jsonGetDouble(ptMetric, _T("totalDebtCagr5Y"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_totalDebt_totalEquityAnnual = jsonGetDouble(ptMetric, _T("totalDebt/totalEquityAnnual"));
		} catch (nlohmann::ordered_json::exception&) {
		}
		try {
			pBasicFinancial->m_totalDebt_totalEquityQuarterly = jsonGetDouble(ptMetric, _T("totalDebt/totalEquityQuarterly"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		try {
			pBasicFinancial->m_yearToDatePriceReturnDaily = jsonGetDouble(ptMetric, _T("yearToDatePriceReturnDaily"));
		} catch (nlohmann::ordered_json::exception&) {
		}

		s = js.at(_T("metricType")); // 目前共五种类型："all", "perShare", "marketCapitalization","metric","eps"
		if (!s_setMetricType.contains(s)) {
			CString str = _T(" metric type out of range: ");
			str += s.c_str();
			str += _T("   Inquiry string:  ");
			str += m_strInquiry;
			gl_systemMessage.PushInnerSystemInformationMessage(str);
		}

		ptSeries = jsonGetChild(js, _T("series"));
		if (!ptSeries.empty()) {
			bool fAnnualEmpty = false;
			bool fQuarterlyEmpty = false;
			try { ptAnnual = jsonGetChild(&ptSeries, _T("annual")); } catch (nlohmann::ordered_json::exception&) { fAnnualEmpty = true; }
			if (!fAnnualEmpty && !ptAnnual.empty()) {
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_cashRatio, _T("cashRatio"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_currentRatio, _T("currentRatio"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_ebitPerShare, _T("ebitPerShare"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_eps, _T("eps"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_grossMargin, _T("grossMargin"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_longtermDebtTotalAsset, _T("longtermDebtTotalAsset"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_longtermDebtTotalCapital, _T("longtermDebtTotalCapital"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_longtermDebtTotalEquity, _T("longtermDebtTotalEquity"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_netDebtToTotalCapital, _T("netDebtToTotalCapital"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_netDebtToTotalEquity, _T("netDebtToTotalEquity"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_netMargin, _T("netMargin"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_operatingMargin, _T("operatingMargin"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_pretaxMargin, _T("pretaxMargin"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_salesPerShare, _T("salesPerShare"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_sgaToSale, _T("sgaToSale"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_totalDebtToEquity, _T("totalDebtToEquity"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_totalDebtToTotalAsset, _T("totalDebtToTotalAsset"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_totalDebtToTotalCapital, _T("totalDebtToTotalCapital"));
				GetSeasonData(&ptAnnual, pBasicFinancial->m_annual.m_totalRatio, _T("totalRatio"));
			}
			try { ptQuarterly = jsonGetChild(&ptSeries, _T("quarterly")); } catch (nlohmann::ordered_json::exception&) { fQuarterlyEmpty = true; }
			if (!fQuarterlyEmpty && !ptQuarterly.empty()) {
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_cashRatio, _T("cashRatio"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_currentRatio, _T("currentRatio"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_ebitPerShare, _T("ebitPerShare"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_eps, _T("eps"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_grossMargin, _T("grossMargin"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_longtermDebtTotalAsset, _T("longtermDebtTotalAsset"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_longtermDebtTotalCapital,_T("longtermDebtTotalCapital"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_longtermDebtTotalEquity, _T("longtermDebtTotalEquity"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_netDebtToTotalCapital, _T("netDebtToTotalCapital"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_netDebtToTotalEquity, _T("netDebtToTotalEquity"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_netMargin, _T("netMargin"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_operatingMargin, _T("operatingMargin"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_pretaxMargin, _T("pretaxMargin"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_salesPerShare, _T("salesPerShare"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_sgaToSale, _T("sgaToSale"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_totalDebtToEquity, _T("totalDebtToEquity"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_totalDebtToTotalAsset, _T("totalDebtToTotalAsset"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_totalDebtToTotalCapital, _T("totalDebtToTotalCapital"));
				GetSeasonData(&ptQuarterly, pBasicFinancial->m_quarter.m_totalRatio, _T("totalRatio"));
			}
		}
	} catch (nlohmann::ordered_json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock basic financials "), e.what());
		return nullptr;
	}
	return pBasicFinancial;
}

void CProductFinnhubCompanyBasicFinancial::GetSeasonData(nlohmann::ordered_json* pjs, vector<CValueOfPeriod>& vData, const char* szMsg) {
	try {
		nlohmann::ordered_json ptChild;
		ptChild = jsonGetChild(pjs, szMsg);
		vector<CValueOfPeriod> vDataTemp;
		Parse(&ptChild, vDataTemp);
		for (size_t i = 0; i < vDataTemp.size(); i++) {
			vData.push_back(vDataTemp[i]);
		}
	} catch (nlohmann::ordered_json::exception&) {
		// do nothing
	}
}

//
// [{"period":"2021-03-31", "v" : -2.8522}, { "period":"2020-12-31","v" : -0.6159 }]
//
void CProductFinnhubCompanyBasicFinancial::Parse(nlohmann::ordered_json* pjs, vector<CValueOfPeriod>& vecData) {
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			CValueOfPeriod sv{ 0, 0 };
			string sDate;
			int year{ 0 }, month{ 0 }, day{ 0 };
			sDate = jsonGetString(it, _T("period"));
			if (!sDate.empty()) {
				static_cast<void>(sscanf_s(sDate.c_str(), "%04d-%02d-%02d", &year, &month, &day));
				sv.m_period = XferYearMonthDayToYYYYMMDD(year, month, +day);
				sv.m_value = jsonGetDouble(it, _T("v"));
				vecData.push_back(sv);
			}
		}
	} catch (nlohmann::ordered_json::exception&) {
		// just skip
	}
}
