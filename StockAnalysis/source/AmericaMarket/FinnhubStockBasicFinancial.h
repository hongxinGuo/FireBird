#pragma once

#include"SetFinnhubStockBasicFinancialMetric.h"
#include"SetFinnhubStockBasicFinancialQuarter.h"
#include"SetFinnhubStockBasicFinancialAnnual.h"

using namespace std;
#include<memory>

class CValueOfPeriod {
public:
	int m_period;
	double m_value;
};

class CBasicFinancialOfSeason {
public:
	vector<CValueOfPeriod> m_cashRatio;
	vector<CValueOfPeriod> m_currentRatio;
	vector<CValueOfPeriod> m_ebitPerShare;
	vector<CValueOfPeriod> m_eps;
	vector<CValueOfPeriod> m_grossMargin;
	vector<CValueOfPeriod> m_longtermDebtTotalAsset;
	vector<CValueOfPeriod> m_longtermDebtTotalCapital;
	vector<CValueOfPeriod> m_longtermDebtTotalEquity;
	vector<CValueOfPeriod> m_netDebtToTotalCapital;
	vector<CValueOfPeriod> m_netDebtToTotalEquity;
	vector<CValueOfPeriod> m_netMargin;
	vector<CValueOfPeriod> m_operatingMargin;
	vector<CValueOfPeriod> m_pretaxMargin;
	vector<CValueOfPeriod> m_salesPerShare;
	vector<CValueOfPeriod> m_sgaToSale;
	vector<CValueOfPeriod> m_totalDebtToEquity;
	vector<CValueOfPeriod> m_totalDebtToTotalAsset;
	vector<CValueOfPeriod> m_totalDebtToTotalCapital;
	vector<CValueOfPeriod> m_totalRatio;
};

class CItemOfBasicFinancialSeasonData {
public:
	CString m_symbol;
	CString m_type;
	int m_date;
	double m_value;
};

/// <summary>
///
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
//		},
//		"metricType": "all",
	//
/// </summary>

class CFinnhubStockBasicFinancial : public CObject {
public:
	CFinnhubStockBasicFinancial();

	void AppendMetric(CSetFinnhubStockBasicFinancialMetric& setMetric);
	void UpdateMetric(CSetFinnhubStockBasicFinancialMetric& setMetric);
	void SaveMetric(CSetFinnhubStockBasicFinancialMetric& setMetric);
	void LoadMetric(CSetFinnhubStockBasicFinancialMetric& setMetric);

	void AppendQuarterData(CSetFinnhubStockBasicFinancialQuarter& setMetric);
	void AppendAnnualData(CSetFinnhubStockBasicFinancialAnnual& setMetric);

	void SaveAllQuarterData(CSetFinnhubStockBasicFinancialQuarter& SetQuarterData, vector<CItemOfBasicFinancialSeasonData>& vDBData);
	void SaveAllAnnualData(CSetFinnhubStockBasicFinancialAnnual& setAnnualData, vector<CItemOfBasicFinancialSeasonData>& vDBData);

	void SaveQuarterData(CSetFinnhubStockBasicFinancialQuarter& SetQuarterData, vector<CValueOfPeriod>& vData, CString typeName, vector<CItemOfBasicFinancialSeasonData>& vDBData);
	void LoadQuarterData(CSetFinnhubStockBasicFinancialQuarter& SetQuarterData);

	void SaveAnnualData(CSetFinnhubStockBasicFinancialAnnual& setAnnualData, vector<CValueOfPeriod>& vData, CString typeName, vector <CItemOfBasicFinancialSeasonData>& vDBData);
	void LoadAnnualData(CSetFinnhubStockBasicFinancialAnnual& setAnnualData);

	bool IsNewData(CString type, CValueOfPeriod vData, vector<CItemOfBasicFinancialSeasonData>& vDBData);

public:
	CString m_symbol;

	// Metric financials data
	double m_10DayAverageTradingVolume;
	double m_13WeekPriceReturnDaily;
	double m_26WeekPriceReturnDaily;
	double m_3MonthAverageTradingVolume;
	double m_52WeekHigh;
	int m_52WeekHighDate;
	double m_52WeekLow;
	int m_52WeekLowDate;
	double m_52WeekPriceReturnDaily;
	double m_5DayPriceReturnDaily;

	double m_assetTurnoverAnnual;
	double m_assetTurnoverTTM;

	double m_beta;
	double m_bookValuePerShareAnnual;
	double m_bookValuePerShareQuarterly;
	double m_bookValueShareGrowth5Y;

	double m_capitalSpendingGrowth5Y;
	double m_cashFlowPerShareAnnual;
	double m_cashFlowPerShareTTM;
	double m_cashPerSharePerShareAnnual;
	double m_cashPerSharePerShareQuarterly;
	double m_currentDividendYieldTTM;
	double m_currentEV_freeCashFlowAnnual;
	double m_currentEV_freeCashFlowTTM;
	double m_currentRatioAnnual;
	double m_currentRatioQuarterly;

	double m_dividendGrowthRate5Y;
	double m_dividendPerShare5Y;
	double m_dividendPerShareAnnual;
	double m_dividendYield5Y;
	double m_dividendYieldIndicatedAnnual;
	double m_dividendsPerShareTTM;

	double m_ebitdaCagr5Y;
	double m_ebitdaInterimCagr5Y;
	double m_ebitdPerShareTTM;
	double m_epsBasicExclExtraItemsAnnual;
	double m_epsBasicExclExtraItemsTTM;
	double m_epsExclExtraItemsAnnual;
	double m_epsExclExtraItemsTTM;
	double m_epsGrowth3Y;
	double m_epsGrowth5Y;
	double m_epsGrowthQuarterlyYoy;
	double m_epsGrowthTTMYoy;
	double m_epsInclExtraItemsAnnual;
	double m_epsInclExtraItemsTTM;
	double m_epsNormalizedAnnual;

	double m_focfCagr5Y;
	double m_freeCashFlowAnnual;
	double m_freeCashFlowPerShareTTM;
	double m_freeCashFlowTTM;
	double m_freeOperatingCashFlow_revenue5Y;
	double m_freeOperatingCashFlow_revenueTTM;

	double m_grossMargin5Y;
	double m_grossMarginAnnual;
	double m_grossMarginTTM;

	double m_inventoryTurnoverAnnual;
	double m_inventoryTurnoverTTM;

	double m_longTermDebt_equityAnnual;
	double m_longTermDebt_equityQuarterly;

	double m_marketCapitalization;
	double m_monthToDatePriceReturnDaily;

	double m_netDebtAnnual;
	double m_netDebtInterim;
	double m_netIncomeEmployeeAnnual;
	double m_netIncomeEmployeeTTM;
	double m_netInterestCoverageAnnual;
	double m_netInterestCoverageTTM;
	double m_netMarginGrowth5Y;
	double m_netProfitMargin5Y;
	double m_netProfitMarginAnnual;
	double m_netProfitMarginTTM;

	double m_operatingMargin5Y;
	double m_operatingMarginAnnual;
	double m_operatingMarginTTM;

	double m_payoutRatioAnnual;
	double m_payoutRatioTTM;
	double m_pbAnnual;
	double m_pbQuarterly;
	double m_pcfShareTTM;
	double m_peBasicExclExtraTTM;
	double m_peExclExtraAnnual;
	double m_peExclExtraHighTTM;
	double m_peExclExtraTTM;
	double m_peExclLowTTM;
	double m_peInclExtraTTM;
	double m_peNormalizedAnnual;
	double m_pfcfShareTTM;
	double m_pfcfShareAnnual;
	double m_pretaxMargin5Y;
	double m_pretaxMarginAnnual;
	double m_pretaxMarginTTM;
	double m_priceRelativeToSP50013Week;
	double m_priceRelativeToSP50026Week;
	double m_priceRelativeToSP5004Week;
	double m_priceRelativeToSP50052Week;
	double m_priceRelativeToSP500Ytd;
	double m_psAnnual;
	double m_psTTM;
	double m_ptbvAnnual;
	double m_ptbvQuarterly;

	double m_quickRatioAnnual;
	double m_quickRatioQuarterly;

	double m_receivablesTurnoverAnnual;
	double m_receivablesTurnoverTTM;
	double m_revenueEmployeeAnnual;
	double m_revenueEmployeeTTM;
	double m_revenueGrowth5Y;
	double m_revenueGrowth3Y;
	double m_revenueGrowthQuarterlyYoy;
	double m_revenueGrowthTTMYoy;
	double m_revenuePerShareAnnual;
	double m_revenuePerShareTTM;
	double m_revenueShareGrowth5Y;
	double m_roaRfy;
	double m_roaa5Y;
	double m_roae5Y;
	double m_roaeTTM;
	double m_roeRfy;
	double m_roeTTM;
	double m_roi5Y;
	double m_roiAnnual;
	double m_roiTTM;

	double m_tangibleBookValuePerShareAnnual;
	double m_tangibleBookValuePerShareQuarterly;
	double m_tbvCagr5Y;
	double m_totalDebt_totalEquityAnnual;
	double m_totalDebt_totalEquityQuarterly;
	double m_totalDebtCagr5Y;

	double m_yearToDatePriceReturnDaily;

	// seasonal Financial data
	CBasicFinancialOfSeason m_annual;
	CBasicFinancialOfSeason m_quarter;
};

typedef shared_ptr<CFinnhubStockBasicFinancial> CFinnhubStockBasicFinancialPtr;
typedef shared_ptr<vector<CFinnhubStockBasicFinancialPtr>> CFinnhubStockBasicFinancialVectorPtr;
