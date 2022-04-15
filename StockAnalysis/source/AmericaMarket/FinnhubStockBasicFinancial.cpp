#include"pch.h"
#include"FinnhubStockBasicFinancial.h"

CFinnhubStockBasicFinancial::CFinnhubStockBasicFinancial() : CObject() {
	m_symbol = _T("");
	m_10DayAverageTradingVolume = 0.0;
	m_52WeekHigh = 0.0;
	m_52WeekHighDate = 19800101;
	m_52WeekLow = 0.0;
	m_52WeekLowDate = 19800101;
	m_52WeekPriceReturnDaily = 0.0;
	m_13WeekPriceReturnDaily = 0.0;
	m_26WeekPriceReturnDaily = 0.0;
	m_3MonthAverageTradingVolume = 0.0;
	m_5DayPriceReturnDaily = 0.0;

	m_assetTurnoverAnnual = 0.0;
	m_assetTurnoverTTM = 0.0;

	m_beta = 0.0;
	m_bookValuePerShareAnnual = 0.0;
	m_bookValuePerShareQuarterly = 0.0;
	m_bookValueShareGrowth5Y = 0.0;

	m_capitalSpendingGrowth5Y = 0.0;
	m_cashFlowPerShareAnnual = 0.0;
	m_cashFlowPerShareTTM = 0.0;
	m_cashPerSharePerShareAnnual = 0.0;
	m_cashPerSharePerShareQuarterly = 0.0;
	m_currentDividendYieldTTM = 0.0;
	m_currentEV_freeCashFlowAnnual = 0.0;
	m_currentEV_freeCashFlowTTM = 0.0;
	m_currentRatioAnnual = 0.0;
	m_currentRatioQuarterly = 0.0;

	m_dividendGrowthRate5Y = 0.0;
	m_dividendPerShare5Y = 0.0;
	m_dividendPerShareAnnual = 0.0;
	m_dividendYield5Y = 0.0;
	m_dividendYieldIndicatedAnnual = 0.0;
	m_dividendsPerShareTTM = 0.0;

	m_ebitdaCagr5Y = 0.0;
	m_ebitdaInterimCagr5Y = 0.0;
	m_ebitdPerShareTTM = 0.0;
	m_epsBasicExclExtraItemsAnnual = 0.0;
	m_epsBasicExclExtraItemsTTM = 0.0;
	m_epsExclExtraItemsAnnual = 0.0;
	m_epsExclExtraItemsTTM = 0.0;
	m_epsGrowth3Y = 0.0;
	m_epsGrowth5Y = 0.0;
	m_epsGrowthQuarterlyYoy = 0.0;
	m_epsGrowthTTMYoy = 0.0;
	m_epsInclExtraItemsAnnual = 0.0;
	m_epsInclExtraItemsTTM = 0.0;
	m_epsNormalizedAnnual = 0.0;

	m_focfCagr5Y = 0.0;
	m_freeCashFlowAnnual = 0.0;
	m_freeCashFlowPerShareTTM = 0.0;
	m_freeCashFlowTTM = 0.0;
	m_freeOperatingCashFlow_revenue5Y = 0.0;
	m_freeOperatingCashFlow_revenueTTM = 0.0;

	m_grossMargin5Y = 0.0;
	m_grossMarginAnnual = 0.0;
	m_grossMarginTTM = 0.0;

	m_inventoryTurnoverAnnual = 0.0;
	m_inventoryTurnoverTTM = 0.0;

	m_longTermDebt_equityAnnual = 0.0;
	m_longTermDebt_equityQuarterly = 0.0;

	m_marketCapitalization = 0.0;
	m_monthToDatePriceReturnDaily = 0.0;

	m_netDebtAnnual = 0.0;
	m_netDebtInterim = 0.0;
	m_netIncomeEmployeeAnnual = 0.0;
	m_netIncomeEmployeeTTM = 0.0;
	m_netInterestCoverageAnnual = 0.0;
	m_netInterestCoverageTTM = 0.0;
	m_netMarginGrowth5Y = 0.0;
	m_netProfitMargin5Y = 0.0;
	m_netProfitMarginAnnual = 0.0;
	m_netProfitMarginTTM = 0.0;

	m_operatingMargin5Y = 0.0;
	m_operatingMarginAnnual = 0.0;
	m_operatingMarginTTM = 0.0;

	m_payoutRatioAnnual = 0.0;
	m_payoutRatioTTM = 0.0;
	m_pbAnnual = 0.0;
	m_pbQuarterly = 0.0;
	m_pcfShareTTM = 0.0;
	m_peBasicExclExtraTTM = 0.0;
	m_peExclExtraAnnual = 0.0;
	m_peExclExtraHighTTM = 0.0;
	m_peExclExtraTTM = 0.0;
	m_peExclLowTTM = 0.0;
	m_peInclExtraTTM = 0.0;
	m_peNormalizedAnnual = 0.0;
	m_pfcfShareTTM = 0.0;
	m_pfcfShareAnnual = 0.0;
	m_pretaxMargin5Y = 0.0;
	m_pretaxMarginAnnual = 0.0;
	m_pretaxMarginTTM = 0.0;
	m_priceRelativeToSP50013Week = 0.0;
	m_priceRelativeToSP50026Week = 0.0;
	m_priceRelativeToSP5004Week = 0.0;
	m_priceRelativeToSP50052Week = 0.0;
	m_priceRelativeToSP500Ytd = 0.0;
	m_psAnnual = 0.0;
	m_psTTM = 0.0;
	m_ptbvAnnual = 0.0;
	m_ptbvQuarterly = 0.0;

	m_quickRatioAnnual = 0.0;
	m_quickRatioQuarterly = 0.0;

	m_receivablesTurnoverAnnual = 0.0;
	m_receivablesTurnoverTTM = 0.0;
	m_revenueEmployeeAnnual = 0.0;
	m_revenueEmployeeTTM = 0.0;
	m_revenueGrowth5Y = 0.0;
	m_revenueGrowth3Y = 0.0;
	m_revenueGrowthQuarterlyYoy = 0.0;
	m_revenueGrowthTTMYoy = 0.0;
	m_revenuePerShareAnnual = 0.0;
	m_revenuePerShareTTM = 0.0;
	m_revenueShareGrowth5Y = 0.0;
	m_roaRfy = 0.0;
	m_roaa5Y = 0.0;
	m_roae5Y = 0.0;
	m_roaeTTM = 0.0;
	m_roeRfy = 0.0;
	m_roeTTM = 0.0;
	m_roi5Y = 0.0;
	m_roiAnnual = 0.0;
	m_roiTTM = 0.0;

	m_tangibleBookValuePerShareAnnual = 0.0;
	m_tangibleBookValuePerShareQuarterly = 0.0;
	m_tbvCagr5Y = 0.0;
	m_totalDebt_totalEquityAnnual = 0.0;
	m_totalDebt_totalEquityQuarterly = 0.0;
	m_totalDebtCagr5Y = 0.0;

	m_yearToDatePriceReturnDaily = 0.0;
}

void CFinnhubStockBasicFinancial::LoadMetric(CSetFinnhubStockBasicFinancialMetric& setMetric) {
	ASSERT(setMetric.IsOpen());

	m_symbol = setMetric.m_symbol;

	m_10DayAverageTradingVolume = setMetric.m_10DayAverageTradingVolume;
	m_13WeekPriceReturnDaily = setMetric.m_13WeekPriceReturnDaily;
	m_26WeekPriceReturnDaily = setMetric.m_26WeekPriceReturnDaily;
	m_3MonthAverageTradingVolume = setMetric.m_3MonthAverageTradingVolume;
	m_52WeekHigh = setMetric.m_52WeekHigh;
	m_52WeekHighDate = setMetric.m_52WeekHighDate;
	m_52WeekLow = setMetric.m_52WeekLow;
	m_52WeekLowDate = setMetric.m_52WeekLowDate;
	m_52WeekPriceReturnDaily = setMetric.m_52WeekPriceReturnDaily;
	m_5DayPriceReturnDaily = setMetric.m_5DayPriceReturnDaily;

	m_assetTurnoverAnnual = setMetric.m_assetTurnoverAnnual;
	m_assetTurnoverTTM = setMetric.m_assetTurnoverTTM;

	m_beta = setMetric.m_beta;
	m_bookValuePerShareAnnual = setMetric.m_bookValuePerShareAnnual;
	m_bookValuePerShareQuarterly = setMetric.m_bookValuePerShareQuarterly;
	m_bookValueShareGrowth5Y = setMetric.m_bookValueShareGrowth5Y;

	m_capitalSpendingGrowth5Y = setMetric.m_capitalSpendingGrowth5Y;
	m_cashFlowPerShareAnnual = setMetric.m_cashFlowPerShareAnnual;
	m_cashFlowPerShareTTM = setMetric.m_cashFlowPerShareTTM;
	m_cashPerSharePerShareAnnual = setMetric.m_cashPerSharePerShareAnnual;
	m_cashPerSharePerShareQuarterly = setMetric.m_cashPerSharePerShareQuarterly;
	m_currentEV_freeCashFlowAnnual = setMetric.m_currentEV_freeCashFlowAnnual;
	m_currentEV_freeCashFlowTTM = setMetric.m_currentEV_freeCashFlowTTM;
	m_currentDividendYieldTTM = setMetric.m_currentDividendYieldTTM;
	m_currentRatioAnnual = setMetric.m_currentRatioAnnual;
	m_currentRatioQuarterly = setMetric.m_currentRatioQuarterly;

	m_dividendGrowthRate5Y = setMetric.m_dividendGrowthRate5Y;
	m_dividendPerShare5Y = setMetric.m_dividendPerShare5Y;
	m_dividendPerShareAnnual = setMetric.m_dividendPerShareAnnual;
	m_dividendYield5Y = setMetric.m_dividendYield5Y;
	m_dividendYieldIndicatedAnnual = setMetric.m_dividendYieldIndicatedAnnual;
	m_dividendsPerShareTTM = setMetric.m_dividendsPerShareTTM;

	m_ebitdaCagr5Y = setMetric.m_ebitdaCagr5Y;
	m_ebitdaInterimCagr5Y = setMetric.m_ebitdaInterimCagr5Y;
	m_ebitdPerShareTTM = setMetric.m_ebitdPerShareTTM;
	m_epsBasicExclExtraItemsAnnual = setMetric.m_epsBasicExclExtraItemsAnnual;
	m_epsBasicExclExtraItemsTTM = setMetric.m_epsBasicExclExtraItemsTTM;
	m_epsExclExtraItemsAnnual = setMetric.m_epsExclExtraItemsAnnual;
	m_epsExclExtraItemsTTM = setMetric.m_epsExclExtraItemsTTM;
	m_epsGrowth3Y = setMetric.m_epsGrowth3Y;
	m_epsGrowth5Y = setMetric.m_epsGrowth5Y;
	m_epsGrowthQuarterlyYoy = setMetric.m_epsGrowthQuarterlyYoy;
	m_epsGrowthTTMYoy = setMetric.m_epsGrowthTTMYoy;
	m_epsInclExtraItemsAnnual = setMetric.m_epsInclExtraItemsAnnual;
	m_epsInclExtraItemsTTM = setMetric.m_epsInclExtraItemsTTM;
	m_epsNormalizedAnnual = setMetric.m_epsNormalizedAnnual;

	m_focfCagr5Y = setMetric.m_focfCagr5Y;
	m_freeCashFlowAnnual = setMetric.m_freeCashFlowAnnual;
	m_freeCashFlowPerShareTTM = setMetric.m_freeCashFlowPerShareTTM;
	m_freeCashFlowTTM = setMetric.m_freeCashFlowTTM;
	m_freeOperatingCashFlow_revenue5Y = setMetric.m_freeOperatingCashFlow_revenue5Y;
	m_freeOperatingCashFlow_revenueTTM = setMetric.m_freeOperatingCashFlow_revenueTTM;

	m_grossMargin5Y = setMetric.m_grossMargin5Y;
	m_grossMarginAnnual = setMetric.m_grossMarginAnnual;
	m_grossMarginTTM = setMetric.m_grossMarginTTM;

	m_inventoryTurnoverAnnual = setMetric.m_inventoryTurnoverAnnual;
	m_inventoryTurnoverTTM = setMetric.m_inventoryTurnoverTTM;

	m_longTermDebt_equityAnnual = setMetric.m_longTermDebt_equityAnnual;
	m_longTermDebt_equityQuarterly = setMetric.m_longTermDebt_equityQuarterly;

	m_marketCapitalization = setMetric.m_marketCapitalization;
	m_monthToDatePriceReturnDaily = setMetric.m_monthToDatePriceReturnDaily;

	m_netDebtAnnual = setMetric.m_netDebtAnnual;
	m_netDebtInterim = setMetric.m_netDebtInterim;
	m_netIncomeEmployeeAnnual = setMetric.m_netIncomeEmployeeAnnual;
	m_netIncomeEmployeeTTM = setMetric.m_netIncomeEmployeeTTM;
	m_netInterestCoverageAnnual = setMetric.m_netInterestCoverageAnnual;
	m_netInterestCoverageTTM = setMetric.m_netInterestCoverageTTM;
	m_netMarginGrowth5Y = setMetric.m_netMarginGrowth5Y;
	m_netProfitMargin5Y = setMetric.m_netProfitMargin5Y;
	m_netProfitMarginAnnual = setMetric.m_netProfitMarginAnnual;
	m_netProfitMarginTTM = setMetric.m_netProfitMarginTTM;

	m_operatingMargin5Y = setMetric.m_operatingMargin5Y;
	m_operatingMarginAnnual = setMetric.m_operatingMarginAnnual;
	m_operatingMarginTTM = setMetric.m_operatingMarginTTM;

	m_payoutRatioAnnual = setMetric.m_payoutRatioAnnual;
	m_payoutRatioTTM = setMetric.m_payoutRatioTTM;
	m_pbAnnual = setMetric.m_pbAnnual;
	m_pbQuarterly = setMetric.m_pbQuarterly;
	m_pcfShareTTM = setMetric.m_pcfShareTTM;
	m_peBasicExclExtraTTM = setMetric.m_peBasicExclExtraTTM;
	m_peExclExtraAnnual = setMetric.m_peExclExtraAnnual;
	m_peExclExtraHighTTM = setMetric.m_peExclExtraHighTTM;
	m_peExclExtraTTM = setMetric.m_peExclExtraTTM;
	m_peExclLowTTM = setMetric.m_peExclLowTTM;
	m_peInclExtraTTM = setMetric.m_peInclExtraTTM;
	m_peNormalizedAnnual = setMetric.m_peNormalizedAnnual;
	m_pfcfShareTTM = setMetric.m_pfcfShareTTM;
	m_pfcfShareAnnual = setMetric.m_pfcfShareAnnual;
	m_pretaxMargin5Y = setMetric.m_pretaxMargin5Y;
	m_pretaxMarginAnnual = setMetric.m_pretaxMarginAnnual;
	m_pretaxMarginTTM = setMetric.m_pretaxMarginTTM;
	m_priceRelativeToSP50013Week = setMetric.m_priceRelativeToSP50013Week;
	m_priceRelativeToSP50026Week = setMetric.m_priceRelativeToSP50026Week;
	m_priceRelativeToSP5004Week = setMetric.m_priceRelativeToSP5004Week;
	m_priceRelativeToSP50052Week = setMetric.m_priceRelativeToSP50052Week;
	m_priceRelativeToSP500Ytd = setMetric.m_priceRelativeToSP500Ytd;
	m_psAnnual = setMetric.m_psAnnual;
	m_psTTM = setMetric.m_psTTM;
	m_ptbvAnnual = setMetric.m_ptbvAnnual;
	m_ptbvQuarterly = setMetric.m_ptbvQuarterly;

	m_quickRatioAnnual = setMetric.m_quickRatioAnnual;
	m_quickRatioQuarterly = setMetric.m_quickRatioQuarterly;

	m_receivablesTurnoverAnnual = setMetric.m_receivablesTurnoverAnnual;
	m_receivablesTurnoverTTM = setMetric.m_receivablesTurnoverTTM;
	m_revenueEmployeeAnnual = setMetric.m_revenueEmployeeAnnual;
	m_revenueEmployeeTTM = setMetric.m_revenueEmployeeTTM;
	m_revenueGrowth5Y = setMetric.m_revenueGrowth5Y;
	m_revenueGrowth3Y = setMetric.m_revenueGrowth3Y;
	m_revenueGrowthQuarterlyYoy = setMetric.m_revenueGrowthQuarterlyYoy;
	m_revenueGrowthTTMYoy = setMetric.m_revenueGrowthTTMYoy;
	m_revenuePerShareAnnual = setMetric.m_revenuePerShareAnnual;
	m_revenuePerShareTTM = setMetric.m_revenuePerShareTTM;
	m_revenueShareGrowth5Y = setMetric.m_revenueShareGrowth5Y;
	m_roaRfy = setMetric.m_roaRfy;
	m_roaa5Y = setMetric.m_roaa5Y;
	m_roae5Y = setMetric.m_roae5Y;
	m_roaeTTM = setMetric.m_roaeTTM;
	m_roeRfy = setMetric.m_roeRfy;
	m_roeTTM = setMetric.m_roeTTM;
	m_roi5Y = setMetric.m_roi5Y;
	m_roiAnnual = setMetric.m_roiAnnual;
	m_roiTTM = setMetric.m_roiTTM;

	m_tangibleBookValuePerShareAnnual = setMetric.m_tangibleBookValuePerShareAnnual;
	m_tangibleBookValuePerShareQuarterly = setMetric.m_tangibleBookValuePerShareQuarterly;
	m_tbvCagr5Y = setMetric.m_tbvCagr5Y;
	m_totalDebt_totalEquityAnnual = setMetric.m_totalDebt_totalEquityAnnual;
	m_totalDebt_totalEquityQuarterly = setMetric.m_totalDebt_totalEquityQuarterly;
	m_totalDebtCagr5Y = setMetric.m_totalDebtCagr5Y;

	m_yearToDatePriceReturnDaily = setMetric.m_yearToDatePriceReturnDaily;
}

void CFinnhubStockBasicFinancial::AppendQuarterData(CSetFinnhubStockBasicFinancialQuarter& setMetric) {
	CSetFinnhubStockBasicFinancialQuarter setQuarterly;
	vector<strSeasonDBData> vData;
	strSeasonDBData data;

	ASSERT(setMetric.IsOpen());
	setQuarterly.m_strFilter = _T("[Symbol] = '") + m_symbol + _T("'");
	setQuarterly.Open();
	while (!setQuarterly.IsEOF()) {
		data.m_symbol = setQuarterly.m_symbol;
		data.m_date = setQuarterly.m_date;
		data.m_type = setQuarterly.m_type;
		data.m_value = setQuarterly.m_value;
		vData.push_back(data);
		setQuarterly.MoveNext();
	}
	setQuarterly.Close();

	SaveAllQuarterData(setMetric, vData);
}

void CFinnhubStockBasicFinancial::AppendAnnualData(CSetFinnhubStockBasicFinancialAnnual& setMetric) {
	CSetFinnhubStockBasicFinancialAnnual setAnnual;
	vector<strSeasonDBData> vData;
	strSeasonDBData data;

	ASSERT(setMetric.IsOpen());
	setAnnual.m_strFilter = _T("[Symbol] = '") + m_symbol + _T("'");
	setAnnual.Open();
	while (!setAnnual.IsEOF())
	{
		data.m_symbol = setAnnual.m_symbol;
		data.m_date = setAnnual.m_date;
		data.m_type = setAnnual.m_type;
		data.m_value = setAnnual.m_value;
		vData.push_back(data);
		setAnnual.MoveNext();
	}
	setAnnual.Close();

	SaveAllAnnualData(setMetric, vData);
}

void CFinnhubStockBasicFinancial::SaveAllQuarterData(CSetFinnhubStockBasicFinancialQuarter& SetQuarterData, vector<strSeasonDBData>& vDBData) {
	SaveQuarterData(SetQuarterData, m_quarter.m_cashRatio, _T("cashRatio"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_currentRatio, _T("currentRatio"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_ebitPerShare, _T("ebitPerShare"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_eps, _T("eps"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_grossMargin, _T("grossMargin"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_longtermDebtTotalAsset, _T("longtermDebtTotalAsset"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_longtermDebtTotalCapital, _T("longtermDebtTotalCapital"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_longtermDebtTotalEquity, _T("longtermDebtTotalEquity"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_netDebtToTotalCapital, _T("netDebtToTotalCapital"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_netDebtToTotalEquity, _T("netDebtToTotalEquity"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_netMargin, _T("netMargin"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_operatingMargin, _T("operatingMargin"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_pretaxMargin, _T("pretaxMargin"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_salesPerShare, _T("salesPerShare"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_sgaToSale, _T("sgaToSale"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_totalDebtToEquity, _T("totalDebtToEquity"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_totalDebtToTotalAsset, _T("totalDebtToTotalAsset"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_totalDebtToTotalCapital, _T("totalDebtToTotalCapital"), vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_totalRatio, _T("totalRatio"), vDBData);
}

void CFinnhubStockBasicFinancial::SaveQuarterData(CSetFinnhubStockBasicFinancialQuarter& SetQuarterData, vector<strValue>& vData, CString typeName, vector<strSeasonDBData>& vDBData) {
	ASSERT(SetQuarterData.IsOpen());
	for (int i = 0; i < vData.size(); i++) {
		if (IsNewData(typeName, vData.at(i), vDBData)) {
			SetQuarterData.AddNew();
			SetQuarterData.m_symbol = m_symbol;
			SetQuarterData.m_type = typeName;
			SetQuarterData.m_date = m_quarter.m_cashRatio.at(i).m_period;
			SetQuarterData.m_value = m_quarter.m_cashRatio.at(i).m_value;
			SetQuarterData.Update();
		}
	}
}

void CFinnhubStockBasicFinancial::LoadQuarterData(CSetFinnhubStockBasicFinancialQuarter& SetQuarterData) {
}

void CFinnhubStockBasicFinancial::SaveAllAnnualData(CSetFinnhubStockBasicFinancialAnnual& setAnnualData, vector<strSeasonDBData>& vDBData) {
	SaveAnnualData(setAnnualData, m_annual.m_cashRatio, _T("cashRatio"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_currentRatio, _T("currentRatio"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_ebitPerShare, _T("ebitPerShare"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_eps, _T("eps"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_grossMargin, _T("grossMargin"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_longtermDebtTotalAsset, _T("longtermDebtTotalAsset"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_longtermDebtTotalCapital, _T("longtermDebtTotalCapital"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_longtermDebtTotalEquity, _T("longtermDebtTotalEquity"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_netDebtToTotalCapital, _T("netDebtToTotalCapital"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_netDebtToTotalEquity, _T("netDebtToTotalEquity"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_netMargin, _T("netMargin"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_operatingMargin, _T("operatingMargin"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_pretaxMargin, _T("pretaxMargin"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_salesPerShare, _T("salesPerShare"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_sgaToSale, _T("sgaToSale"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_totalDebtToEquity, _T("totalDebtToEquity"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_totalDebtToTotalAsset, _T("totalDebtToTotalAsset"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_totalDebtToTotalCapital, _T("totalDebtToTotalCapital"), vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_totalRatio, _T("totalRatio"), vDBData);
}

void CFinnhubStockBasicFinancial::SaveAnnualData(CSetFinnhubStockBasicFinancialAnnual& setAnnualData, vector<strValue>& vData, CString typeName, vector<strSeasonDBData>& vDBData) {
	ASSERT(setAnnualData.IsOpen());
	for (int i = 0; i < vData.size(); i++) {
		if (IsNewData(typeName, vData.at(i), vDBData)) {
			setAnnualData.AddNew();
			setAnnualData.m_symbol = m_symbol;
			setAnnualData.m_type = typeName;
			setAnnualData.m_date = m_annual.m_cashRatio.at(i).m_period;
			setAnnualData.m_value = m_annual.m_cashRatio.at(i).m_value;
			setAnnualData.Update();
		}
	}
}

void CFinnhubStockBasicFinancial::LoadAnnualData(CSetFinnhubStockBasicFinancialAnnual& setAnnualData) {
}

bool CFinnhubStockBasicFinancial::IsNewData(CString type, strValue vData, vector<strSeasonDBData>& vDBData) {
	for (int i = 0; i < vDBData.size(); i++) {
		if (vDBData.at(i).m_type == type && vDBData.at(i).m_date == vData.m_period) {
			return false;
		}
	}
	return true;
}

void CFinnhubStockBasicFinancial::AppendMetric(CSetFinnhubStockBasicFinancialMetric& setMetric) {
	ASSERT(setMetric.IsOpen());
	setMetric.AddNew();
	SaveMetric(setMetric);
	setMetric.Update();
}

void CFinnhubStockBasicFinancial::UpdateMetric(CSetFinnhubStockBasicFinancialMetric& setMetric) {
	ASSERT(setMetric.IsOpen());
	setMetric.Edit();
	SaveMetric(setMetric);
	setMetric.Update();
}

void CFinnhubStockBasicFinancial::SaveMetric(CSetFinnhubStockBasicFinancialMetric& setMetric) {
	setMetric.m_symbol = m_symbol;
	setMetric.m_10DayAverageTradingVolume = m_10DayAverageTradingVolume;
	setMetric.m_13WeekPriceReturnDaily = m_13WeekPriceReturnDaily;
	setMetric.m_26WeekPriceReturnDaily = m_26WeekPriceReturnDaily;
	setMetric.m_3MonthAverageTradingVolume = m_3MonthAverageTradingVolume;
	setMetric.m_52WeekHigh = m_52WeekHigh;
	setMetric.m_52WeekHighDate = m_52WeekHighDate;
	setMetric.m_52WeekLow = m_52WeekLow;
	setMetric.m_52WeekLowDate = m_52WeekLowDate;
	setMetric.m_52WeekPriceReturnDaily = m_52WeekPriceReturnDaily;
	setMetric.m_5DayPriceReturnDaily = m_5DayPriceReturnDaily;

	setMetric.m_assetTurnoverAnnual = m_assetTurnoverAnnual;
	setMetric.m_assetTurnoverTTM = m_assetTurnoverTTM;

	setMetric.m_beta = m_beta;
	setMetric.m_bookValuePerShareAnnual = m_bookValuePerShareAnnual;
	setMetric.m_bookValuePerShareQuarterly = m_bookValuePerShareQuarterly;
	setMetric.m_bookValueShareGrowth5Y = m_bookValueShareGrowth5Y;

	setMetric.m_capitalSpendingGrowth5Y = m_capitalSpendingGrowth5Y;
	setMetric.m_cashFlowPerShareAnnual = m_cashFlowPerShareAnnual;
	setMetric.m_cashFlowPerShareTTM = m_cashFlowPerShareTTM;
	setMetric.m_cashPerSharePerShareAnnual = m_cashPerSharePerShareAnnual;
	setMetric.m_cashPerSharePerShareQuarterly = m_cashPerSharePerShareQuarterly;
	setMetric.m_currentEV_freeCashFlowAnnual = m_currentEV_freeCashFlowAnnual;
	setMetric.m_currentEV_freeCashFlowTTM = m_currentEV_freeCashFlowTTM;
	setMetric.m_currentDividendYieldTTM = m_currentDividendYieldTTM;
	setMetric.m_currentRatioAnnual = m_currentRatioAnnual;
	setMetric.m_currentRatioQuarterly = m_currentRatioQuarterly;

	setMetric.m_dividendGrowthRate5Y = m_dividendGrowthRate5Y;
	setMetric.m_dividendPerShare5Y = m_dividendPerShare5Y;
	setMetric.m_dividendPerShareAnnual = m_dividendPerShareAnnual;
	setMetric.m_dividendsPerShareTTM = m_dividendsPerShareTTM;
	setMetric.m_dividendYield5Y = m_dividendYield5Y;
	setMetric.m_dividendYieldIndicatedAnnual = m_dividendYieldIndicatedAnnual;

	setMetric.m_ebitdPerShareTTM = m_ebitdPerShareTTM;
	setMetric.m_ebitdaCagr5Y = m_ebitdaCagr5Y;
	setMetric.m_ebitdaInterimCagr5Y = m_ebitdaInterimCagr5Y;
	setMetric.m_epsBasicExclExtraItemsAnnual = m_epsBasicExclExtraItemsAnnual;
	setMetric.m_epsBasicExclExtraItemsTTM = m_epsBasicExclExtraItemsTTM;
	setMetric.m_epsExclExtraItemsAnnual = m_epsExclExtraItemsAnnual;
	setMetric.m_epsExclExtraItemsTTM = m_epsExclExtraItemsTTM;
	setMetric.m_epsGrowth3Y = m_epsGrowth3Y;
	setMetric.m_epsGrowth5Y = m_epsGrowth5Y;
	setMetric.m_epsGrowthQuarterlyYoy = m_epsGrowthQuarterlyYoy;
	setMetric.m_epsGrowthTTMYoy = m_epsGrowthTTMYoy;
	setMetric.m_epsInclExtraItemsAnnual = m_epsInclExtraItemsAnnual;
	setMetric.m_epsInclExtraItemsTTM = m_epsInclExtraItemsTTM;
	setMetric.m_epsNormalizedAnnual = m_epsNormalizedAnnual;

	setMetric.m_focfCagr5Y = m_focfCagr5Y;
	setMetric.m_freeCashFlowAnnual = m_freeCashFlowAnnual;
	setMetric.m_freeCashFlowPerShareTTM = m_freeCashFlowPerShareTTM;
	setMetric.m_freeCashFlowTTM = m_freeCashFlowTTM;
	setMetric.m_freeOperatingCashFlow_revenue5Y = m_freeOperatingCashFlow_revenue5Y;
	setMetric.m_freeOperatingCashFlow_revenueTTM = m_freeOperatingCashFlow_revenueTTM;

	setMetric.m_grossMargin5Y = m_grossMargin5Y;
	setMetric.m_grossMarginAnnual = m_grossMarginAnnual;
	setMetric.m_grossMarginTTM = m_grossMarginTTM;

	setMetric.m_inventoryTurnoverAnnual = m_inventoryTurnoverAnnual;
	setMetric.m_inventoryTurnoverTTM = m_inventoryTurnoverTTM;

	setMetric.m_longTermDebt_equityAnnual = m_longTermDebt_equityAnnual;
	setMetric.m_longTermDebt_equityQuarterly = m_longTermDebt_equityQuarterly;

	setMetric.m_marketCapitalization = m_marketCapitalization;
	setMetric.m_monthToDatePriceReturnDaily = m_monthToDatePriceReturnDaily;

	setMetric.m_netDebtAnnual = m_netDebtAnnual;
	setMetric.m_netDebtInterim = m_netDebtInterim;
	setMetric.m_netIncomeEmployeeAnnual = m_netIncomeEmployeeAnnual;
	setMetric.m_netIncomeEmployeeTTM = m_netIncomeEmployeeTTM;
	setMetric.m_netInterestCoverageAnnual = m_netInterestCoverageAnnual;
	setMetric.m_netInterestCoverageTTM = m_netInterestCoverageTTM;
	setMetric.m_netMarginGrowth5Y = m_netMarginGrowth5Y;
	setMetric.m_netProfitMargin5Y = m_netProfitMargin5Y;
	setMetric.m_netProfitMarginAnnual = m_netProfitMarginAnnual;
	setMetric.m_netProfitMarginTTM = m_netProfitMarginTTM;

	setMetric.m_operatingMargin5Y = m_operatingMargin5Y;
	setMetric.m_operatingMarginAnnual = m_operatingMarginAnnual;
	setMetric.m_operatingMarginTTM = m_operatingMarginTTM;

	setMetric.m_payoutRatioAnnual = m_payoutRatioAnnual;
	setMetric.m_payoutRatioTTM = m_payoutRatioTTM;
	setMetric.m_pbAnnual = m_pbAnnual;
	setMetric.m_pbQuarterly = m_pbQuarterly;
	setMetric.m_pcfShareTTM = m_pcfShareTTM;
	setMetric.m_peBasicExclExtraTTM = m_peBasicExclExtraTTM;
	setMetric.m_peExclExtraAnnual = m_peExclExtraAnnual;
	setMetric.m_peExclExtraHighTTM = m_peExclExtraHighTTM;
	setMetric.m_peExclExtraTTM = m_peExclExtraTTM;
	setMetric.m_peExclLowTTM = m_peExclLowTTM;
	setMetric.m_peInclExtraTTM = m_peInclExtraTTM;
	setMetric.m_peNormalizedAnnual = m_peNormalizedAnnual;
	setMetric.m_pfcfShareAnnual = m_pfcfShareAnnual;
	setMetric.m_pfcfShareTTM = m_pfcfShareTTM;
	setMetric.m_pretaxMargin5Y = m_pretaxMargin5Y;
	setMetric.m_pretaxMarginAnnual = m_pretaxMarginAnnual;
	setMetric.m_pretaxMarginTTM = m_pretaxMarginTTM;
	setMetric.m_priceRelativeToSP50013Week = m_priceRelativeToSP50013Week;
	setMetric.m_priceRelativeToSP50026Week = m_priceRelativeToSP50026Week;
	setMetric.m_priceRelativeToSP5004Week = m_priceRelativeToSP5004Week;
	setMetric.m_priceRelativeToSP50052Week = m_priceRelativeToSP50052Week;
	setMetric.m_priceRelativeToSP500Ytd = m_priceRelativeToSP500Ytd;
	setMetric.m_psAnnual = m_psAnnual;
	setMetric.m_psTTM = m_psTTM;
	setMetric.m_ptbvAnnual = m_ptbvAnnual;
	setMetric.m_ptbvQuarterly = m_ptbvQuarterly;

	setMetric.m_quickRatioAnnual = m_quickRatioAnnual;
	setMetric.m_quickRatioQuarterly = m_quickRatioQuarterly;

	setMetric.m_receivablesTurnoverAnnual = m_receivablesTurnoverAnnual;
	setMetric.m_receivablesTurnoverTTM = m_receivablesTurnoverTTM;
	setMetric.m_revenueEmployeeAnnual = m_revenueEmployeeAnnual;
	setMetric.m_revenueEmployeeTTM = m_revenueEmployeeTTM;
	setMetric.m_revenueGrowth3Y = m_revenueGrowth3Y;
	setMetric.m_revenueGrowth5Y = m_revenueGrowth5Y;
	setMetric.m_revenueGrowthQuarterlyYoy = m_revenueGrowthQuarterlyYoy;
	setMetric.m_revenueGrowthTTMYoy = m_revenueGrowthTTMYoy;
	setMetric.m_revenuePerShareAnnual = m_revenuePerShareAnnual;
	setMetric.m_revenuePerShareTTM = m_revenuePerShareTTM;
	setMetric.m_revenueShareGrowth5Y = m_revenueShareGrowth5Y;
	setMetric.m_roaa5Y = m_roaa5Y;
	setMetric.m_roae5Y = m_roae5Y;
	setMetric.m_roaeTTM = m_roaeTTM;
	setMetric.m_roaRfy = m_roaRfy;
	setMetric.m_roeRfy = m_roeRfy;
	setMetric.m_roeTTM = m_roeTTM;
	setMetric.m_roi5Y = m_roi5Y;
	setMetric.m_roiAnnual = m_roiAnnual;
	setMetric.m_roiTTM = m_roiTTM;

	setMetric.m_tangibleBookValuePerShareAnnual = m_tangibleBookValuePerShareAnnual;
	setMetric.m_tangibleBookValuePerShareQuarterly = m_tangibleBookValuePerShareQuarterly;
	setMetric.m_tbvCagr5Y = m_tbvCagr5Y;
	setMetric.m_totalDebtCagr5Y = m_totalDebtCagr5Y;
	setMetric.m_totalDebt_totalEquityAnnual = m_totalDebt_totalEquityAnnual;
	setMetric.m_totalDebt_totalEquityQuarterly = m_totalDebt_totalEquityQuarterly;
	setMetric.m_yearToDatePriceReturnDaily = m_yearToDatePriceReturnDaily;
}