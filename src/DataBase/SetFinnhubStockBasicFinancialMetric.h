// SetFinnhubStockBasicFinancialMetric.h : SetFinnhubStockBasicFinancialMetric 的声明

#pragma once

#include"VirtualRecordset.h"

/// <summary>
/// </summary>
// "metric": {
// "10DayAverageTradingVolume": 0.43212,
// "13WeekPriceReturnDaily" : 56.53409,
// "26WeekPriceReturnDaily" : 39.84772,
// "3MonthAverageTradingVolume" : 4.08133,
// "52WeekHigh" : 5.5699,
// "52WeekHighDate" : "2022-04-08",
// "52WeekLow" : 3.0699,
// "52WeekLowDate" : "2021-08-20",
// "52WeekPriceReturnDaily" : 30.87886,
// "5DayPriceReturnDaily" : 20.04357,
//
// "assetTurnoverAnnual" : 0.26072,
// "assetTurnoverTTM" : 0.26072,
//
// "beta" : 0.60752,
// "bookValuePerShareAnnual" : 9.57393,
// "bookValuePerShareQuarterly" : 9.57393,
// "bookValueShareGrowth5Y" : -4.75925,
//
// "capitalSpendingGrowth5Y" : -56.49411,
// "cashFlowPerShareAnnual" : -0.17905,
// "cashFlowPerShareTTM" : -0.17906,
// "cashPerSharePerShareAnnual" : 1.61358,
// "cashPerSharePerShareQuarterly" : 1.61358,
// "currentDividendYieldTTM" : 0,
// "currentEv/freeCashFlowAnnual" : null,
// "currentEv/freeCashFlowTTM" : null,
// "currentRatioAnnual" : 1.61055,
// "currentRatioQuarterly" : 1.61055,
//
// "dividendGrowthRate5Y" : null,
// "dividendPerShare5Y" : null,
// "dividendPerShareAnnual" : null,
//"dividendYield5Y" : null,
//"dividendYieldIndicatedAnnual" : null,
//"dividendsPerShareTTM" : 0,
//
//"ebitdPerShareTTM" : 0.34761,
//"ebitdaCagr5Y" : -26.28051,
//"ebitdaInterimCagr5Y" : -26.28051,
//"epsBasicExclExtraItemsAnnual" : -1.12407,
//"epsBasicExclExtraItemsTTM" : -1.12246,
//"epsExclExtraItemsAnnual" : -1.12407,
//"epsExclExtraItemsTTM" : -1.12246,
//"epsGrowth3Y" : null,
//"epsGrowth5Y" : null,
//"epsGrowthQuarterlyYoy" : 57.6156,
//"epsGrowthTTMYoy" : -511.5282,
//"epsInclExtraItemsAnnual" : -1.12407,
//"epsInclExtraItemsTTM" : -1.12246,
//"epsNormalizedAnnual" : -1.12407,
//
//"focfCagr5Y" : null,
//"freeCashFlowAnnual" : -6.40426,
//"freeCashFlowPerShareTTM" : -0.18903,
//"freeCashFlowTTM" : -6.40426,
//"freeOperatingCashFlow/revenue5Y" : 4.2309,
//"freeOperatingCashFlow/revenueTTM" : -3.32716,
//
//"grossMargin5Y" : 28.23462,
//"grossMarginAnnual" : 22.37743,
//"grossMarginTTM" : 22.37744,
//
//"inventoryTurnoverAnnual" : 13.98368,
//"inventoryTurnoverTTM" : 13.98368,
//
//"longTermDebt/equityAnnual" : 101.9366,
//"longTermDebt/equityQuarterly" : 101.9366,
//
//"marketCapitalization" : 189.345,
//"monthToDatePriceReturnDaily" : 22.44444,
//
//"netDebtAnnual" : 316.1072,
//"netDebtInterim" : 316.1072,
//"netIncomeEmployeeAnnual" : -776933,
//"netIncomeEmployeeTTM" : -776933.2,
//"netInterestCoverageAnnual" : -1.21208,
//"netInterestCoverageTTM" : -1.21208,
//"netMarginGrowth5Y" : null,
//"netProfitMargin5Y" : -11.52406,
//"netProfitMarginAnnual" : -18.97082,
//"netProfitMarginTTM" : -18.97083,
//
//"operatingMargin5Y" : 1.32556,
//"operatingMarginAnnual" : -10.2087,
//"operatingMarginTTM" : -10.2087,
//
//"payoutRatioAnnual" : null,
//"payoutRatioTTM" : null,
//"pbAnnual" : 0.64854,
//"pbQuarterly" : 0.64854,
//"pcfShareTTM" : null,
//"peBasicExclExtraTTM" : null,
//"peExclExtraAnnual" : null,
//"peExclExtraHighTTM" : 8.87272,
//"peExclExtraTTM" : null,
//"peExclLowTTM" : 7.69796,
//"peInclExtraTTM" : null,
//"peNormalizedAnnual" : null,
//"pfcfShareAnnual" : null,
//"pfcfShareTTM" : null,
//"pretaxMargin5Y" : -11.46396,
//"pretaxMarginAnnual" : -18.8931,
//"pretaxMarginTTM" : -18.89311,
//"priceRelativeToS&P50013Week" : null,
//"priceRelativeToS&P50026Week" : null,
//"priceRelativeToS&P5004Week" : null,
//"priceRelativeToS&P50052Week" : null,
//"priceRelativeToS&P500Ytd" : null,
//"psAnnual" : 0.98369,
//"psTTM" : 0.98369,
//"ptbvAnnual" : 0.57552,
//"ptbvQuarterly" : 0.57552,
//
//"quickRatioAnnual" : 1.42089,
//"quickRatioQuarterly" : 1.42089,
//
//"receivablesTurnoverAnnual" : 10.10098,
//"receivablesTurnoverTTM" : 10.10098,
//"revenueEmployeeAnnual" : 4095411,
//"revenueEmployeeTTM" : 4095411,
//"revenueGrowth3Y" : -2.88897,
//"revenueGrowth5Y" : 3.20402,
//"revenueGrowthQuarterlyYoy" : 25.72476,
//"revenueGrowthTTMYoy" : -12.53004,
//"revenuePerShareAnnual" : 5.68086,
//"revenuePerShareTTM" : 5.68136,
//"revenueShareGrowth5Y" : 0.81717,
//"roaRfy" : -4.94614,
//"roaa5Y" : -3.00716,
//"roae5Y" : -7.10913,
//"roaeTTM" : -12.44075,
//"roeRfy" : -12.44075,
//"roeTTM" : -4.94614,
//"roi5Y" : -3.26368,
//"roiAnnual" : -5.38924,
//"roiTTM" : -5.38924,
//
//"tangibleBookValuePerShareAnnual" : 9.57393,
//"tangibleBookValuePerShareQuarterly" : 9.57393,
//"tbvCagr5Y" : -4.0369,
//"totalDebt/totalEquityAnnual" : 112.9358,
//"totalDebt/totalEquityQuarterly" : 112.9358,
//"totalDebtCagr5Y" : -4.27798,
//
//"yearToDatePriceReturnDaily" : 63.01775
//		}
//
///
class CSetFinnhubStockBasicFinancialMetric : public CVirtualRecordset {
public:
	CSetFinnhubStockBasicFinancialMetric(const CString& strSchema = _T("WorldMarket"), const CString& strTable = _T("finnhub_stock_basic_financials_metric"), CDatabase* pDatabase = nullptr);

	// 字段/参数数据

	// 以下字符串类型(如果存在)反映数据库字段(ANSI 数据类型的 CStringA 和 Unicode
	// 数据类型的 CStringW)的实际数据类型。
	//  这是为防止 ODBC 驱动程序执行可能
	// 不必要的转换。如果希望，可以将这些成员更改为
	// CString 类型，ODBC 驱动程序将执行所有必要的转换。
	// (注意: 必须使用 3.5 版或更高版本的 ODBC 驱动程序
	// 以同时支持 Unicode 和这些转换)。

	long m_ID{0};  // 索引用，自动生成。
	CString m_symbol{_T("")};

	double m_10DayAverageTradingVolume{0.0};
	double m_13WeekPriceReturnDaily{0.0};
	double m_26WeekPriceReturnDaily{0.0};
	double m_3MonthAverageTradingVolume{0.0};
	double m_52WeekHigh{0.0};
	int m_52WeekHighDate{0};
	double m_52WeekLow{0.0};
	int m_52WeekLowDate{0};
	double m_52WeekPriceReturnDaily{0.0};
	double m_5DayPriceReturnDaily{0.0};

	double m_assetTurnoverAnnual{0.0};
	double m_assetTurnoverTTM{0.0};

	double m_beta{0.0};
	double m_bookValuePerShareAnnual{0.0};
	double m_bookValuePerShareQuarterly{0.0};
	double m_bookValueShareGrowth5Y{0.0};

	double m_capitalSpendingGrowth5Y{0.0};
	double m_cashFlowPerShareAnnual{0.0};
	double m_cashFlowPerShareTTM{0.0};
	double m_cashPerSharePerShareAnnual{0.0};
	double m_cashPerSharePerShareQuarterly{0.0};
	double m_currentDividendYieldTTM{0.0};
	double m_currentEV_freeCashFlowAnnual{0.0};
	double m_currentEV_freeCashFlowTTM{0.0};
	double m_currentRatioAnnual{0.0};
	double m_currentRatioQuarterly{0.0};

	double m_dividendGrowthRate5Y{0.0};
	double m_dividendPerShare5Y{0.0};
	double m_dividendPerShareAnnual{0.0};
	double m_dividendYield5Y{0.0};
	double m_dividendYieldIndicatedAnnual{0.0};
	double m_dividendsPerShareTTM{0.0};

	double m_ebitdPerShareTTM{0.0};
	double m_ebitdaCagr5Y{0.0};
	double m_ebitdaInterimCagr5Y{0.0};
	double m_epsBasicExclExtraItemsAnnual{0.0};
	double m_epsBasicExclExtraItemsTTM{0.0};
	double m_epsExclExtraItemsAnnual{0.0};
	double m_epsExclExtraItemsTTM{0.0};
	double m_epsGrowth3Y{0.0};
	double m_epsGrowth5Y{0.0};
	double m_epsGrowthQuarterlyYoy{0.0};
	double m_epsGrowthTTMYoy{0.0};
	double m_epsInclExtraItemsAnnual{0.0};
	double m_epsInclExtraItemsTTM{0.0};
	double m_epsNormalizedAnnual{0.0};

	double m_focfCagr5Y{0.0};
	double m_freeCashFlowAnnual{0.0};
	double m_freeCashFlowPerShareTTM{0.0};
	double m_freeCashFlowTTM{0.0};
	double m_freeOperatingCashFlow_revenue5Y{0.0};
	double m_freeOperatingCashFlow_revenueTTM{0.0};

	double m_grossMargin5Y{0.0};
	double m_grossMarginAnnual{0.0};
	double m_grossMarginTTM{0.0};

	double m_inventoryTurnoverAnnual{0.0};
	double m_inventoryTurnoverTTM{0.0};

	double m_longTermDebt_equityAnnual{0.0};
	double m_longTermDebt_equityQuarterly{0.0};

	double m_marketCapitalization{0.0};
	double m_monthToDatePriceReturnDaily{0.0};

	double m_netDebtAnnual{0.0};
	double m_netDebtInterim{0.0};
	double m_netIncomeEmployeeAnnual{0.0};
	double m_netIncomeEmployeeTTM{0.0};
	double m_netInterestCoverageAnnual{0.0};
	double m_netInterestCoverageTTM{0.0};
	double m_netMarginGrowth5Y{0.0};
	double m_netProfitMargin5Y{0.0};
	double m_netProfitMarginAnnual{0.0};
	double m_netProfitMarginTTM{0.0};

	double m_operatingMargin5Y{0.0};
	double m_operatingMarginAnnual{0.0};
	double m_operatingMarginTTM{0.0};

	double m_payoutRatioAnnual{0.0};
	double m_payoutRatioTTM{0.0};
	double m_pbAnnual{0.0};
	double m_pbQuarterly{0.0};
	double m_pcfShareTTM{0.0};
	double m_peBasicExclExtraTTM{0.0};
	double m_peExclExtraAnnual{0.0};
	double m_peExclExtraHighTTM{0.0};
	double m_peExclExtraTTM{0.0};
	double m_peExclLowTTM{0.0};
	double m_peInclExtraTTM{0.0};
	double m_peNormalizedAnnual{0.0};
	double m_pfcfShareTTM{0.0};
	double m_pfcfShareAnnual{0.0};
	double m_pretaxMargin5Y{0.0};
	double m_pretaxMarginAnnual{0.0};
	double m_pretaxMarginTTM{0.0};
	double m_priceRelativeToSP50013Week{0.0};
	double m_priceRelativeToSP50026Week{0.0};
	double m_priceRelativeToSP5004Week{0.0};
	double m_priceRelativeToSP50052Week{0.0};
	double m_priceRelativeToSP500Ytd{0.0};
	double m_psAnnual{0.0};
	double m_psTTM{0.0};
	double m_ptbvAnnual{0.0};
	double m_ptbvQuarterly{0.0};

	double m_quickRatioAnnual{0.0};
	double m_quickRatioQuarterly{0.0};

	double m_receivablesTurnoverAnnual{0.0};
	double m_receivablesTurnoverTTM{0.0};
	double m_revenueEmployeeAnnual{0.0};
	double m_revenueEmployeeTTM{0.0};
	double m_revenueGrowth3Y{0.0};
	double m_revenueGrowth5Y{0.0};
	double m_revenueGrowthQuarterlyYoy{0.0};
	double m_revenueGrowthTTMYoy{0.0};
	double m_revenuePerShareAnnual{0.0};
	double m_revenuePerShareTTM{0.0};
	double m_revenueShareGrowth5Y{0.0};
	double m_roaRfy{0.0};
	double m_roaa5Y{0.0};
	double m_roae5Y{0.0};
	double m_roaeTTM{0.0};
	double m_roeRfy{0.0};
	double m_roeTTM{0.0};
	double m_roi5Y{0.0};
	double m_roiAnnual{0.0};
	double m_roiTTM{0.0};

	double m_tangibleBookValuePerShareAnnual{0.0};
	double m_tangibleBookValuePerShareQuarterly{0.0};
	double m_tbvCagr5Y{0.0};
	double m_totalDebt_totalEquityAnnual{0.0};
	double m_totalDebt_totalEquityQuarterly{0.0};
	double m_totalDebtCagr5Y{0.0};

	double m_yearToDatePriceReturnDaily{0.0};

	// 重写
	// 向导生成的虚函数重写
public:
	void DoFieldExchange(CFieldExchange* pFX) override;	// RFX 支持
};
