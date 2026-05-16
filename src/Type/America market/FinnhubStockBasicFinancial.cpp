#include"pch.h"
#include"FinnhubStockBasicFinancial.h"

#include "CharSetTransfer.h"

CBasicFinancialOfSeason::CBasicFinancialOfSeason() {
	Reset();
}

CBasicFinancialOfSeason::CBasicFinancialOfSeason(const CBasicFinancialOfSeason& other) {
	m_cashRatio = other.m_cashRatio;
	m_currentRatio = other.m_currentRatio;
	m_ebitPerShare = other.m_ebitPerShare;
	m_eps = other.m_eps;
	m_grossMargin = other.m_grossMargin;
	m_longtermDebtTotalAsset = other.m_longtermDebtTotalAsset;
	m_longtermDebtTotalCapital = other.m_longtermDebtTotalCapital;
	m_longtermDebtTotalEquity = other.m_longtermDebtTotalEquity;
	m_netDebtToTotalCapital = other.m_netDebtToTotalCapital;
	m_netDebtToTotalEquity = other.m_netDebtToTotalEquity;
	m_netMargin = other.m_netMargin;
	m_operatingMargin = other.m_operatingMargin;
	m_pretaxMargin = other.m_pretaxMargin;
	m_salesPerShare = other.m_salesPerShare;
	m_sgaToSale = other.m_sgaToSale;
	m_totalDebtToEquity = other.m_totalDebtToEquity;
	m_totalDebtToTotalAsset = other.m_totalDebtToTotalAsset;
	m_totalDebtToTotalCapital = other.m_totalDebtToTotalCapital;
	m_totalRatio = other.m_totalRatio;
}

CBasicFinancialOfSeason::CBasicFinancialOfSeason(CBasicFinancialOfSeason&& other) noexcept {
	m_cashRatio = std::move(other.m_cashRatio);
	m_currentRatio = std::move(other.m_currentRatio);
	m_ebitPerShare = std::move(other.m_ebitPerShare);
	m_eps = std::move(other.m_eps);
	m_grossMargin = std::move(other.m_grossMargin);
	m_longtermDebtTotalAsset = std::move(other.m_longtermDebtTotalAsset);
	m_longtermDebtTotalCapital = std::move(other.m_longtermDebtTotalCapital);
	m_longtermDebtTotalEquity = std::move(other.m_longtermDebtTotalEquity);
	m_netDebtToTotalCapital = std::move(other.m_netDebtToTotalCapital);
	m_netDebtToTotalEquity = std::move(other.m_netDebtToTotalEquity);
	m_netMargin = std::move(other.m_netMargin);
	m_operatingMargin = std::move(other.m_operatingMargin);
	m_pretaxMargin = std::move(other.m_pretaxMargin);
	m_salesPerShare = std::move(other.m_salesPerShare);
	m_sgaToSale = std::move(other.m_sgaToSale);
	m_totalDebtToEquity = std::move(other.m_totalDebtToEquity);
	m_totalDebtToTotalAsset = std::move(other.m_totalDebtToTotalAsset);
	m_totalDebtToTotalCapital = std::move(other.m_totalDebtToTotalCapital);
	m_totalRatio = std::move(other.m_totalRatio);
}

CBasicFinancialOfSeason& CBasicFinancialOfSeason::operator=(CBasicFinancialOfSeason&& other) noexcept {
	m_cashRatio = std::move(other.m_cashRatio);
	m_currentRatio = std::move(other.m_currentRatio);
	m_ebitPerShare = std::move(other.m_ebitPerShare);
	m_eps = std::move(other.m_eps);
	m_grossMargin = std::move(other.m_grossMargin);
	m_longtermDebtTotalAsset = std::move(other.m_longtermDebtTotalAsset);
	m_longtermDebtTotalCapital = std::move(other.m_longtermDebtTotalCapital);
	m_longtermDebtTotalEquity = std::move(other.m_longtermDebtTotalEquity);
	m_netDebtToTotalCapital = std::move(other.m_netDebtToTotalCapital);
	m_netDebtToTotalEquity = std::move(other.m_netDebtToTotalEquity);
	m_netMargin = std::move(other.m_netMargin);
	m_operatingMargin = std::move(other.m_operatingMargin);
	m_pretaxMargin = std::move(other.m_pretaxMargin);
	m_salesPerShare = std::move(other.m_salesPerShare);
	m_sgaToSale = std::move(other.m_sgaToSale);
	m_totalDebtToEquity = std::move(other.m_totalDebtToEquity);
	m_totalDebtToTotalAsset = std::move(other.m_totalDebtToTotalAsset);
	m_totalDebtToTotalCapital = std::move(other.m_totalDebtToTotalCapital);
	m_totalRatio = std::move(other.m_totalRatio);
	return *this;
}
CBasicFinancialOfSeason::~CBasicFinancialOfSeason() = default;

void CBasicFinancialOfSeason::Reset() {
	m_cashRatio.clear();
	m_currentRatio.clear();
	m_ebitPerShare.clear();
	m_eps.clear();
	m_grossMargin.clear();
	m_longtermDebtTotalAsset.clear();
	m_longtermDebtTotalCapital.clear();
	m_longtermDebtTotalEquity.clear();
	m_netDebtToTotalCapital.clear();
	m_netDebtToTotalEquity.clear();
	m_netMargin.clear();
	m_operatingMargin.clear();
	m_pretaxMargin.clear();
	m_salesPerShare.clear();
	m_sgaToSale.clear();
	m_totalDebtToEquity.clear();
	m_totalDebtToTotalAsset.clear();
	m_totalDebtToTotalCapital.clear();
	m_totalRatio.clear();
}

CFinnhubStockBasicFinancial::CFinnhubStockBasicFinancial() {
	Reset();
}

CFinnhubStockBasicFinancial::CFinnhubStockBasicFinancial(const CFinnhubStockBasicFinancial& other) {
	m_symbol = other.m_symbol;
	m_AverageTradingVolume10Day = other.m_AverageTradingVolume10Day;
	m_High52Week = other.m_High52Week;
	m_HighDate52Week = other.m_HighDate52Week;
	m_Low52Week = other.m_Low52Week;
	m_LowDate52Week = other.m_LowDate52Week;
	m_PriceReturnDaily52Week = other.m_PriceReturnDaily52Week;
	m_PriceReturnDaily13Week = other.m_PriceReturnDaily13Week;
	m_PriceReturnDaily26Week = other.m_PriceReturnDaily26Week;
	m_AverageTradingVolume2Month = other.m_AverageTradingVolume2Month;
	m_PriceReturnDaily5Day = other.m_PriceReturnDaily5Day;
	m_assetTurnoverAnnual = other.m_assetTurnoverAnnual;
	m_assetTurnoverTTM = other.m_assetTurnoverTTM;
	m_beta = other.m_beta;
	m_bookValuePerShareAnnual = other.m_bookValuePerShareAnnual;
	m_bookValuePerShareQuarterly = other.m_bookValuePerShareQuarterly;
	m_bookValueShareGrowth5Y = other.m_bookValueShareGrowth5Y;
	m_capitalSpendingGrowth5Y = other.m_capitalSpendingGrowth5Y;
	m_cashFlowPerShareAnnual = other.m_cashFlowPerShareAnnual;
	m_cashFlowPerShareTTM = other.m_cashFlowPerShareTTM;
	m_cashPerSharePerShareAnnual = other.m_cashPerSharePerShareAnnual;
	m_cashPerSharePerShareQuarterly = other.m_cashPerSharePerShareQuarterly;
	m_currentDividendYieldTTM = other.m_currentDividendYieldTTM;
	m_currentEV_freeCashFlowAnnual = other.m_currentEV_freeCashFlowAnnual;
	m_currentEV_freeCashFlowTTM = other.m_currentEV_freeCashFlowTTM;
	m_currentRatioAnnual = other.m_currentRatioAnnual;
	m_currentRatioQuarterly = other.m_currentRatioQuarterly;
	m_dividendGrowthRate5Y = other.m_dividendGrowthRate5Y;
	m_dividendPerShare5Y = other.m_dividendPerShare5Y;
	m_dividendPerShareAnnual = other.m_dividendPerShareAnnual;
	m_dividendYield5Y = other.m_dividendYield5Y;
	m_dividendYieldIndicatedAnnual = other.m_dividendYieldIndicatedAnnual;
	m_dividendsPerShareTTM = other.m_dividendsPerShareTTM;
	m_ebitdaCagr5Y = other.m_ebitdaCagr5Y;
	m_ebitdaInterimCagr5Y = other.m_ebitdaInterimCagr5Y;
	m_ebitdPerShareTTM = other.m_ebitdPerShareTTM;
	m_epsBasicExclExtraItemsAnnual = other.m_epsBasicExclExtraItemsAnnual;
	m_epsBasicExclExtraItemsTTM = other.m_epsBasicExclExtraItemsTTM;
	m_epsExclExtraItemsAnnual = other.m_epsExclExtraItemsAnnual;
	m_epsExclExtraItemsTTM = other.m_epsExclExtraItemsTTM;
	m_epsGrowth3Y = other.m_epsGrowth3Y;
	m_epsGrowth5Y = other.m_epsGrowth5Y;
	m_epsGrowthQuarterlyYoy = other.m_epsGrowthQuarterlyYoy;
	m_epsGrowthTTMYoy = other.m_epsGrowthTTMYoy;
	m_epsInclExtraItemsAnnual = other.m_epsInclExtraItemsAnnual;
	m_epsInclExtraItemsTTM = other.m_epsInclExtraItemsTTM;
	m_epsNormalizedAnnual = other.m_epsNormalizedAnnual;
	m_focfCagr5Y = other.m_focfCagr5Y;
	m_freeCashFlowAnnual = other.m_freeCashFlowAnnual;
	m_freeCashFlowPerShareTTM = other.m_freeCashFlowPerShareTTM;
	m_freeCashFlowTTM = other.m_freeCashFlowTTM;
	m_freeOperatingCashFlow_revenue5Y = other.m_freeOperatingCashFlow_revenue5Y;
	m_freeOperatingCashFlow_revenueTTM = other.m_freeOperatingCashFlow_revenueTTM;

	m_grossMargin5Y = other.m_grossMargin5Y;
	m_grossMarginAnnual = other.m_grossMarginAnnual;
	m_grossMarginTTM = other.m_grossMarginTTM;

	m_inventoryTurnoverAnnual = other.m_inventoryTurnoverAnnual;
	m_inventoryTurnoverTTM = other.m_inventoryTurnoverTTM;

	m_longTermDebt_equityAnnual = other.m_longTermDebt_equityAnnual;
	m_longTermDebt_equityQuarterly = other.m_longTermDebt_equityQuarterly;

	m_marketCapitalization = other.m_marketCapitalization;
	m_monthToDatePriceReturnDaily = other.m_monthToDatePriceReturnDaily;

	m_netDebtAnnual = other.m_netDebtAnnual;
	m_netDebtInterim = other.m_netDebtInterim;
	m_netIncomeEmployeeAnnual = other.m_netIncomeEmployeeAnnual;
	m_netIncomeEmployeeTTM = other.m_netIncomeEmployeeTTM;
	m_netInterestCoverageAnnual = other.m_netInterestCoverageAnnual;
	m_netInterestCoverageTTM = other.m_netInterestCoverageTTM;
	m_netMarginGrowth5Y = other.m_netMarginGrowth5Y;
	m_netProfitMargin5Y = other.m_netProfitMargin5Y;
	m_netProfitMarginAnnual = other.m_netProfitMarginAnnual;
	m_netProfitMarginTTM = other.m_netProfitMarginTTM;

	m_operatingMargin5Y = other.m_operatingMargin5Y;
	m_operatingMarginAnnual = other.m_operatingMarginAnnual;
	m_operatingMarginTTM = other.m_operatingMarginTTM;

	m_payoutRatioAnnual = other.m_payoutRatioAnnual;
	m_payoutRatioTTM = other.m_payoutRatioTTM;
	m_pbAnnual = other.m_pbAnnual;
	m_pbQuarterly = other.m_pbQuarterly;
	m_pcfShareTTM = other.m_pcfShareTTM;
	m_peBasicExclExtraTTM = other.m_peBasicExclExtraTTM;
	m_peExclExtraAnnual = other.m_peExclExtraAnnual;
	m_peExclExtraHighTTM = other.m_peExclExtraHighTTM;
	m_peExclExtraTTM = other.m_peExclExtraTTM;
	m_peExclLowTTM = other.m_peExclLowTTM;
	m_peInclExtraTTM = other.m_peInclExtraTTM;
	m_peNormalizedAnnual = other.m_peNormalizedAnnual;
	m_pfcfShareTTM = other.m_pfcfShareTTM;
	m_pfcfShareAnnual = other.m_pfcfShareAnnual;
	m_pretaxMargin5Y = other.m_pretaxMargin5Y;
	m_pretaxMarginAnnual = other.m_pretaxMarginAnnual;
	m_pretaxMarginTTM = other.m_pretaxMarginTTM;
	m_priceRelativeToSP50013Week = other.m_priceRelativeToSP50013Week;
	m_priceRelativeToSP50026Week = other.m_priceRelativeToSP50026Week;
	m_priceRelativeToSP5004Week = other.m_priceRelativeToSP5004Week;
	m_priceRelativeToSP50052Week = other.m_priceRelativeToSP50052Week;
	m_priceRelativeToSP500Ytd = other.m_priceRelativeToSP500Ytd;
	m_psAnnual = other.m_psAnnual;
	m_psTTM = other.m_psTTM;
	m_ptbvAnnual = other.m_ptbvAnnual;
	m_ptbvQuarterly = other.m_ptbvQuarterly;

	m_quickRatioAnnual = other.m_quickRatioAnnual;
	m_quickRatioQuarterly = other.m_quickRatioQuarterly;
	m_receivablesTurnoverAnnual = other.m_receivablesTurnoverAnnual;
	m_receivablesTurnoverTTM = other.m_receivablesTurnoverTTM;
	m_revenueEmployeeAnnual = other.m_revenueEmployeeAnnual;
	m_revenueEmployeeTTM = other.m_revenueEmployeeTTM;
	m_revenueGrowth5Y = other.m_revenueGrowth5Y;
	m_revenueGrowth3Y = other.m_revenueGrowth3Y;
	m_revenueGrowthQuarterlyYoy = other.m_revenueGrowthQuarterlyYoy;
	m_revenueGrowthTTMYoy = other.m_revenueGrowthTTMYoy;
	m_revenuePerShareAnnual = other.m_revenuePerShareAnnual;
	m_revenuePerShareTTM = other.m_revenuePerShareTTM;
	m_revenueShareGrowth5Y = other.m_revenueShareGrowth5Y;
	m_roaRfy = other.m_roaRfy;
	m_roaa5Y = other.m_roaa5Y;
	m_roae5Y = other.m_roae5Y;
	m_roaeTTM = other.m_roaeTTM;
	m_roeRfy = other.m_roeRfy;
	m_roeTTM = other.m_roeTTM;
	m_roi5Y = other.m_roi5Y;
	m_roiAnnual = other.m_roiAnnual;
	m_roiTTM = other.m_roiTTM;

	m_tangibleBookValuePerShareAnnual = other.m_tangibleBookValuePerShareAnnual;
	m_tangibleBookValuePerShareQuarterly = other.m_tangibleBookValuePerShareQuarterly;
	m_tbvCagr5Y = other.m_tbvCagr5Y;
	m_totalDebt_totalEquityAnnual = other.m_totalDebt_totalEquityAnnual;
	m_totalDebt_totalEquityQuarterly = other.m_totalDebt_totalEquityQuarterly;
	m_totalDebtCagr5Y = other.m_totalDebtCagr5Y;

	m_yearToDatePriceReturnDaily = other.m_yearToDatePriceReturnDaily;

	m_annual.m_cashRatio = other.m_annual.m_cashRatio;
	m_annual.m_currentRatio = other.m_annual.m_currentRatio;
	m_annual.m_ebitPerShare = other.m_annual.m_ebitPerShare;
	m_annual.m_eps = other.m_annual.m_eps;
	m_annual.m_grossMargin = other.m_annual.m_grossMargin;
	m_annual.m_longtermDebtTotalAsset = other.m_annual.m_longtermDebtTotalAsset;
	m_annual.m_longtermDebtTotalCapital = other.m_annual.m_longtermDebtTotalCapital;
	m_annual.m_longtermDebtTotalEquity = other.m_annual.m_longtermDebtTotalEquity;
	m_annual.m_netDebtToTotalCapital = other.m_annual.m_netDebtToTotalCapital;
	m_annual.m_netDebtToTotalEquity = other.m_annual.m_netDebtToTotalEquity;
	m_annual.m_netMargin = other.m_annual.m_netMargin;
	m_annual.m_operatingMargin = other.m_annual.m_operatingMargin;
	m_annual.m_pretaxMargin = other.m_annual.m_pretaxMargin;
	m_annual.m_salesPerShare = other.m_annual.m_salesPerShare;
	m_annual.m_sgaToSale = other.m_annual.m_sgaToSale;
	m_annual.m_totalDebtToEquity = other.m_annual.m_totalDebtToEquity;
	m_annual.m_totalDebtToTotalAsset = other.m_annual.m_totalDebtToTotalAsset;
	m_annual.m_totalDebtToTotalCapital = other.m_annual.m_totalDebtToTotalCapital;
	m_annual.m_totalRatio = other.m_annual.m_totalRatio;

	m_quarter.m_cashRatio = other.m_quarter.m_cashRatio;
	m_quarter.m_currentRatio = other.m_quarter.m_currentRatio;
	m_quarter.m_ebitPerShare = other.m_quarter.m_ebitPerShare;
	m_quarter.m_eps = other.m_quarter.m_eps;
	m_quarter.m_grossMargin = other.m_quarter.m_grossMargin;
	m_quarter.m_longtermDebtTotalAsset = other.m_quarter.m_longtermDebtTotalAsset;
	m_quarter.m_longtermDebtTotalCapital = other.m_quarter.m_longtermDebtTotalCapital;
	m_quarter.m_longtermDebtTotalEquity = other.m_quarter.m_longtermDebtTotalEquity;
	m_quarter.m_netDebtToTotalCapital = other.m_quarter.m_netDebtToTotalCapital;
	m_quarter.m_netDebtToTotalEquity = other.m_quarter.m_netDebtToTotalEquity;
	m_quarter.m_netMargin = other.m_quarter.m_netMargin;
	m_quarter.m_operatingMargin = other.m_quarter.m_operatingMargin;
	m_quarter.m_pretaxMargin = other.m_quarter.m_pretaxMargin;
	m_quarter.m_salesPerShare = other.m_quarter.m_salesPerShare;
	m_quarter.m_sgaToSale = other.m_quarter.m_sgaToSale;
	m_quarter.m_totalDebtToEquity = other.m_quarter.m_totalDebtToEquity;
	m_quarter.m_totalDebtToTotalAsset = other.m_quarter.m_totalDebtToTotalAsset;
	m_quarter.m_totalDebtToTotalCapital = other.m_quarter.m_totalDebtToTotalCapital;
	m_quarter.m_totalRatio = other.m_quarter.m_totalRatio;
}
CFinnhubStockBasicFinancial& CFinnhubStockBasicFinancial::operator=(const CFinnhubStockBasicFinancial& other) {
	m_symbol = other.m_symbol;
	m_AverageTradingVolume10Day = other.m_AverageTradingVolume10Day;
	m_High52Week = other.m_High52Week;
	m_HighDate52Week = other.m_HighDate52Week;
	m_Low52Week = other.m_Low52Week;
	m_LowDate52Week = other.m_LowDate52Week;
	m_PriceReturnDaily52Week = other.m_PriceReturnDaily52Week;
	m_PriceReturnDaily13Week = other.m_PriceReturnDaily13Week;
	m_PriceReturnDaily26Week = other.m_PriceReturnDaily26Week;
	m_AverageTradingVolume2Month = other.m_AverageTradingVolume2Month;
	m_PriceReturnDaily5Day = other.m_PriceReturnDaily5Day;
	m_assetTurnoverAnnual = other.m_assetTurnoverAnnual;
	m_assetTurnoverTTM = other.m_assetTurnoverTTM;
	m_beta = other.m_beta;
	m_bookValuePerShareAnnual = other.m_bookValuePerShareAnnual;
	m_bookValuePerShareQuarterly = other.m_bookValuePerShareQuarterly;
	m_bookValueShareGrowth5Y = other.m_bookValueShareGrowth5Y;
	m_capitalSpendingGrowth5Y = other.m_capitalSpendingGrowth5Y;
	m_cashFlowPerShareAnnual = other.m_cashFlowPerShareAnnual;
	m_cashFlowPerShareTTM = other.m_cashFlowPerShareTTM;
	m_cashPerSharePerShareAnnual = other.m_cashPerSharePerShareAnnual;
	m_cashPerSharePerShareQuarterly = other.m_cashPerSharePerShareQuarterly;
	m_currentDividendYieldTTM = other.m_currentDividendYieldTTM;
	m_currentEV_freeCashFlowAnnual = other.m_currentEV_freeCashFlowAnnual;
	m_currentEV_freeCashFlowTTM = other.m_currentEV_freeCashFlowTTM;
	m_currentRatioAnnual = other.m_currentRatioAnnual;
	m_currentRatioQuarterly = other.m_currentRatioQuarterly;
	m_dividendGrowthRate5Y = other.m_dividendGrowthRate5Y;
	m_dividendPerShare5Y = other.m_dividendPerShare5Y;
	m_dividendPerShareAnnual = other.m_dividendPerShareAnnual;
	m_dividendYield5Y = other.m_dividendYield5Y;
	m_dividendYieldIndicatedAnnual = other.m_dividendYieldIndicatedAnnual;
	m_dividendsPerShareTTM = other.m_dividendsPerShareTTM;
	m_ebitdaCagr5Y = other.m_ebitdaCagr5Y;
	m_ebitdaInterimCagr5Y = other.m_ebitdaInterimCagr5Y;
	m_ebitdPerShareTTM = other.m_ebitdPerShareTTM;
	m_epsBasicExclExtraItemsAnnual = other.m_epsBasicExclExtraItemsAnnual;
	m_epsBasicExclExtraItemsTTM = other.m_epsBasicExclExtraItemsTTM;
	m_epsExclExtraItemsAnnual = other.m_epsExclExtraItemsAnnual;
	m_epsExclExtraItemsTTM = other.m_epsExclExtraItemsTTM;
	m_epsGrowth3Y = other.m_epsGrowth3Y;
	m_epsGrowth5Y = other.m_epsGrowth5Y;
	m_epsGrowthQuarterlyYoy = other.m_epsGrowthQuarterlyYoy;
	m_epsGrowthTTMYoy = other.m_epsGrowthTTMYoy;
	m_epsInclExtraItemsAnnual = other.m_epsInclExtraItemsAnnual;
	m_epsInclExtraItemsTTM = other.m_epsInclExtraItemsTTM;
	m_epsNormalizedAnnual = other.m_epsNormalizedAnnual;
	m_focfCagr5Y = other.m_focfCagr5Y;
	m_freeCashFlowAnnual = other.m_freeCashFlowAnnual;
	m_freeCashFlowPerShareTTM = other.m_freeCashFlowPerShareTTM;
	m_freeCashFlowTTM = other.m_freeCashFlowTTM;
	m_freeOperatingCashFlow_revenue5Y = other.m_freeOperatingCashFlow_revenue5Y;
	m_freeOperatingCashFlow_revenueTTM = other.m_freeOperatingCashFlow_revenueTTM;

	m_grossMargin5Y = other.m_grossMargin5Y;
	m_grossMarginAnnual = other.m_grossMarginAnnual;
	m_grossMarginTTM = other.m_grossMarginTTM;

	m_inventoryTurnoverAnnual = other.m_inventoryTurnoverAnnual;
	m_inventoryTurnoverTTM = other.m_inventoryTurnoverTTM;

	m_longTermDebt_equityAnnual = other.m_longTermDebt_equityAnnual;
	m_longTermDebt_equityQuarterly = other.m_longTermDebt_equityQuarterly;

	m_marketCapitalization = other.m_marketCapitalization;
	m_monthToDatePriceReturnDaily = other.m_monthToDatePriceReturnDaily;

	m_netDebtAnnual = other.m_netDebtAnnual;
	m_netDebtInterim = other.m_netDebtInterim;
	m_netIncomeEmployeeAnnual = other.m_netIncomeEmployeeAnnual;
	m_netIncomeEmployeeTTM = other.m_netIncomeEmployeeTTM;
	m_netInterestCoverageAnnual = other.m_netInterestCoverageAnnual;
	m_netInterestCoverageTTM = other.m_netInterestCoverageTTM;
	m_netMarginGrowth5Y = other.m_netMarginGrowth5Y;
	m_netProfitMargin5Y = other.m_netProfitMargin5Y;
	m_netProfitMarginAnnual = other.m_netProfitMarginAnnual;
	m_netProfitMarginTTM = other.m_netProfitMarginTTM;

	m_operatingMargin5Y = other.m_operatingMargin5Y;
	m_operatingMarginAnnual = other.m_operatingMarginAnnual;
	m_operatingMarginTTM = other.m_operatingMarginTTM;

	m_payoutRatioAnnual = other.m_payoutRatioAnnual;
	m_payoutRatioTTM = other.m_payoutRatioTTM;
	m_pbAnnual = other.m_pbAnnual;
	m_pbQuarterly = other.m_pbQuarterly;
	m_pcfShareTTM = other.m_pcfShareTTM;
	m_peBasicExclExtraTTM = other.m_peBasicExclExtraTTM;
	m_peExclExtraAnnual = other.m_peExclExtraAnnual;
	m_peExclExtraHighTTM = other.m_peExclExtraHighTTM;
	m_peExclExtraTTM = other.m_peExclExtraTTM;
	m_peExclLowTTM = other.m_peExclLowTTM;
	m_peInclExtraTTM = other.m_peInclExtraTTM;
	m_peNormalizedAnnual = other.m_peNormalizedAnnual;
	m_pfcfShareTTM = other.m_pfcfShareTTM;
	m_pfcfShareAnnual = other.m_pfcfShareAnnual;
	m_pretaxMargin5Y = other.m_pretaxMargin5Y;
	m_pretaxMarginAnnual = other.m_pretaxMarginAnnual;
	m_pretaxMarginTTM = other.m_pretaxMarginTTM;
	m_priceRelativeToSP50013Week = other.m_priceRelativeToSP50013Week;
	m_priceRelativeToSP50026Week = other.m_priceRelativeToSP50026Week;
	m_priceRelativeToSP5004Week = other.m_priceRelativeToSP5004Week;
	m_priceRelativeToSP50052Week = other.m_priceRelativeToSP50052Week;
	m_priceRelativeToSP500Ytd = other.m_priceRelativeToSP500Ytd;
	m_psAnnual = other.m_psAnnual;
	m_psTTM = other.m_psTTM;
	m_ptbvAnnual = other.m_ptbvAnnual;
	m_ptbvQuarterly = other.m_ptbvQuarterly;

	m_quickRatioAnnual = other.m_quickRatioAnnual;
	m_quickRatioQuarterly = other.m_quickRatioQuarterly;
	m_receivablesTurnoverAnnual = other.m_receivablesTurnoverAnnual;
	m_receivablesTurnoverTTM = other.m_receivablesTurnoverTTM;
	m_revenueEmployeeAnnual = other.m_revenueEmployeeAnnual;
	m_revenueEmployeeTTM = other.m_revenueEmployeeTTM;
	m_revenueGrowth5Y = other.m_revenueGrowth5Y;
	m_revenueGrowth3Y = other.m_revenueGrowth3Y;
	m_revenueGrowthQuarterlyYoy = other.m_revenueGrowthQuarterlyYoy;
	m_revenueGrowthTTMYoy = other.m_revenueGrowthTTMYoy;
	m_revenuePerShareAnnual = other.m_revenuePerShareAnnual;
	m_revenuePerShareTTM = other.m_revenuePerShareTTM;
	m_revenueShareGrowth5Y = other.m_revenueShareGrowth5Y;
	m_roaRfy = other.m_roaRfy;
	m_roaa5Y = other.m_roaa5Y;
	m_roae5Y = other.m_roae5Y;
	m_roaeTTM = other.m_roaeTTM;
	m_roeRfy = other.m_roeRfy;
	m_roeTTM = other.m_roeTTM;
	m_roi5Y = other.m_roi5Y;
	m_roiAnnual = other.m_roiAnnual;
	m_roiTTM = other.m_roiTTM;

	m_tangibleBookValuePerShareAnnual = other.m_tangibleBookValuePerShareAnnual;
	m_tangibleBookValuePerShareQuarterly = other.m_tangibleBookValuePerShareQuarterly;
	m_tbvCagr5Y = other.m_tbvCagr5Y;
	m_totalDebt_totalEquityAnnual = other.m_totalDebt_totalEquityAnnual;
	m_totalDebt_totalEquityQuarterly = other.m_totalDebt_totalEquityQuarterly;
	m_totalDebtCagr5Y = other.m_totalDebtCagr5Y;

	m_yearToDatePriceReturnDaily = other.m_yearToDatePriceReturnDaily;

	m_annual.m_cashRatio = other.m_annual.m_cashRatio;
	m_annual.m_currentRatio = other.m_annual.m_currentRatio;
	m_annual.m_ebitPerShare = other.m_annual.m_ebitPerShare;
	m_annual.m_eps = other.m_annual.m_eps;
	m_annual.m_grossMargin = other.m_annual.m_grossMargin;
	m_annual.m_longtermDebtTotalAsset = other.m_annual.m_longtermDebtTotalAsset;
	m_annual.m_longtermDebtTotalCapital = other.m_annual.m_longtermDebtTotalCapital;
	m_annual.m_longtermDebtTotalEquity = other.m_annual.m_longtermDebtTotalEquity;
	m_annual.m_netDebtToTotalCapital = other.m_annual.m_netDebtToTotalCapital;
	m_annual.m_netDebtToTotalEquity = other.m_annual.m_netDebtToTotalEquity;
	m_annual.m_netMargin = other.m_annual.m_netMargin;
	m_annual.m_operatingMargin = other.m_annual.m_operatingMargin;
	m_annual.m_pretaxMargin = other.m_annual.m_pretaxMargin;
	m_annual.m_salesPerShare = other.m_annual.m_salesPerShare;
	m_annual.m_sgaToSale = other.m_annual.m_sgaToSale;
	m_annual.m_totalDebtToEquity = other.m_annual.m_totalDebtToEquity;
	m_annual.m_totalDebtToTotalAsset = other.m_annual.m_totalDebtToTotalAsset;
	m_annual.m_totalDebtToTotalCapital = other.m_annual.m_totalDebtToTotalCapital;
	m_annual.m_totalRatio = other.m_annual.m_totalRatio;

	m_quarter.m_cashRatio = other.m_quarter.m_cashRatio;
	m_quarter.m_currentRatio = other.m_quarter.m_currentRatio;
	m_quarter.m_ebitPerShare = other.m_quarter.m_ebitPerShare;
	m_quarter.m_eps = other.m_quarter.m_eps;
	m_quarter.m_grossMargin = other.m_quarter.m_grossMargin;
	m_quarter.m_longtermDebtTotalAsset = other.m_quarter.m_longtermDebtTotalAsset;
	m_quarter.m_longtermDebtTotalCapital = other.m_quarter.m_longtermDebtTotalCapital;
	m_quarter.m_longtermDebtTotalEquity = other.m_quarter.m_longtermDebtTotalEquity;
	m_quarter.m_netDebtToTotalCapital = other.m_quarter.m_netDebtToTotalCapital;
	m_quarter.m_netDebtToTotalEquity = other.m_quarter.m_netDebtToTotalEquity;
	m_quarter.m_netMargin = other.m_quarter.m_netMargin;
	m_quarter.m_operatingMargin = other.m_quarter.m_operatingMargin;
	m_quarter.m_pretaxMargin = other.m_quarter.m_pretaxMargin;
	m_quarter.m_salesPerShare = other.m_quarter.m_salesPerShare;
	m_quarter.m_sgaToSale = other.m_quarter.m_sgaToSale;
	m_quarter.m_totalDebtToEquity = other.m_quarter.m_totalDebtToEquity;
	m_quarter.m_totalDebtToTotalAsset = other.m_quarter.m_totalDebtToTotalAsset;
	m_quarter.m_totalDebtToTotalCapital = other.m_quarter.m_totalDebtToTotalCapital;
	m_quarter.m_totalRatio = other.m_quarter.m_totalRatio;
	return *this;
}
CFinnhubStockBasicFinancial::CFinnhubStockBasicFinancial(CFinnhubStockBasicFinancial&& other) noexcept {
	m_symbol = std::move(other.m_symbol);
	m_AverageTradingVolume10Day = other.m_AverageTradingVolume10Day;
	m_High52Week = other.m_High52Week;
	m_HighDate52Week = other.m_HighDate52Week;
	m_Low52Week = other.m_Low52Week;
	m_LowDate52Week = other.m_LowDate52Week;
	m_PriceReturnDaily52Week = other.m_PriceReturnDaily52Week;
	m_PriceReturnDaily13Week = other.m_PriceReturnDaily13Week;
	m_PriceReturnDaily26Week = other.m_PriceReturnDaily26Week;
	m_AverageTradingVolume2Month = other.m_AverageTradingVolume2Month;
	m_PriceReturnDaily5Day = other.m_PriceReturnDaily5Day;
	m_assetTurnoverAnnual = other.m_assetTurnoverAnnual;
	m_assetTurnoverTTM = other.m_assetTurnoverTTM;
	m_beta = other.m_beta;
	m_bookValuePerShareAnnual = other.m_bookValuePerShareAnnual;
	m_bookValuePerShareQuarterly = other.m_bookValuePerShareQuarterly;
	m_bookValueShareGrowth5Y = other.m_bookValueShareGrowth5Y;
	m_capitalSpendingGrowth5Y = other.m_capitalSpendingGrowth5Y;
	m_cashFlowPerShareAnnual = other.m_cashFlowPerShareAnnual;
	m_cashFlowPerShareTTM = other.m_cashFlowPerShareTTM;
	m_cashPerSharePerShareAnnual = other.m_cashPerSharePerShareAnnual;
	m_cashPerSharePerShareQuarterly = other.m_cashPerSharePerShareQuarterly;
	m_currentDividendYieldTTM = other.m_currentDividendYieldTTM;
	m_currentEV_freeCashFlowAnnual = other.m_currentEV_freeCashFlowAnnual;
	m_currentEV_freeCashFlowTTM = other.m_currentEV_freeCashFlowTTM;
	m_currentRatioAnnual = other.m_currentRatioAnnual;
	m_currentRatioQuarterly = other.m_currentRatioQuarterly;
	m_dividendGrowthRate5Y = other.m_dividendGrowthRate5Y;
	m_dividendPerShare5Y = other.m_dividendPerShare5Y;
	m_dividendPerShareAnnual = other.m_dividendPerShareAnnual;
	m_dividendYield5Y = other.m_dividendYield5Y;
	m_dividendYieldIndicatedAnnual = other.m_dividendYieldIndicatedAnnual;
	m_dividendsPerShareTTM = other.m_dividendsPerShareTTM;
	m_ebitdaCagr5Y = other.m_ebitdaCagr5Y;
	m_ebitdaInterimCagr5Y = other.m_ebitdaInterimCagr5Y;
	m_ebitdPerShareTTM = other.m_ebitdPerShareTTM;
	m_epsBasicExclExtraItemsAnnual = other.m_epsBasicExclExtraItemsAnnual;
	m_epsBasicExclExtraItemsTTM = other.m_epsBasicExclExtraItemsTTM;
	m_epsExclExtraItemsAnnual = other.m_epsExclExtraItemsAnnual;
	m_epsExclExtraItemsTTM = other.m_epsExclExtraItemsTTM;
	m_epsGrowth3Y = other.m_epsGrowth3Y;
	m_epsGrowth5Y = other.m_epsGrowth5Y;
	m_epsGrowthQuarterlyYoy = other.m_epsGrowthQuarterlyYoy;
	m_epsGrowthTTMYoy = other.m_epsGrowthTTMYoy;
	m_epsInclExtraItemsAnnual = other.m_epsInclExtraItemsAnnual;
	m_epsInclExtraItemsTTM = other.m_epsInclExtraItemsTTM;
	m_epsNormalizedAnnual = other.m_epsNormalizedAnnual;
	m_focfCagr5Y = other.m_focfCagr5Y;
	m_freeCashFlowAnnual = other.m_freeCashFlowAnnual;
	m_freeCashFlowPerShareTTM = other.m_freeCashFlowPerShareTTM;
	m_freeCashFlowTTM = other.m_freeCashFlowTTM;
	m_freeOperatingCashFlow_revenue5Y = other.m_freeOperatingCashFlow_revenue5Y;
	m_freeOperatingCashFlow_revenueTTM = other.m_freeOperatingCashFlow_revenueTTM;

	m_grossMargin5Y = other.m_grossMargin5Y;
	m_grossMarginAnnual = other.m_grossMarginAnnual;
	m_grossMarginTTM = other.m_grossMarginTTM;
	m_inventoryTurnoverAnnual = other.m_inventoryTurnoverAnnual;
	m_inventoryTurnoverTTM = other.m_inventoryTurnoverTTM;

	m_longTermDebt_equityAnnual = other.m_longTermDebt_equityAnnual;
	m_longTermDebt_equityQuarterly = other.m_longTermDebt_equityQuarterly;

	m_marketCapitalization = other.m_marketCapitalization;
	m_monthToDatePriceReturnDaily = other.m_monthToDatePriceReturnDaily;

	m_netDebtAnnual = other.m_netDebtAnnual;
	m_netDebtInterim = other.m_netDebtInterim;
	m_netIncomeEmployeeAnnual = other.m_netIncomeEmployeeAnnual;
	m_netIncomeEmployeeTTM = other.m_netIncomeEmployeeTTM;
	m_netInterestCoverageAnnual = other.m_netInterestCoverageAnnual;
	m_netInterestCoverageTTM = other.m_netInterestCoverageTTM;
	m_netMarginGrowth5Y = other.m_netMarginGrowth5Y;
	m_netProfitMargin5Y = other.m_netProfitMargin5Y;
	m_netProfitMarginAnnual = other.m_netProfitMarginAnnual;
	m_netProfitMarginTTM = other.m_netProfitMarginTTM;

	m_operatingMargin5Y = other.m_operatingMargin5Y;
	m_operatingMarginAnnual = other.m_operatingMarginAnnual;
	m_operatingMarginTTM = other.m_operatingMarginTTM;

	m_payoutRatioAnnual = other.m_payoutRatioAnnual;
	m_payoutRatioTTM = other.m_payoutRatioTTM;
	m_pbAnnual = other.m_pbAnnual;
	m_pbQuarterly = other.m_pbQuarterly;
	m_pcfShareTTM = other.m_pcfShareTTM;
	m_peBasicExclExtraTTM = other.m_peBasicExclExtraTTM;
	m_peExclExtraAnnual = other.m_peExclExtraAnnual;
	m_peExclExtraHighTTM = other.m_peExclExtraHighTTM;
	m_peExclExtraTTM = other.m_peExclExtraTTM;
	m_peExclLowTTM = other.m_peExclLowTTM;
	m_peInclExtraTTM = other.m_peInclExtraTTM;
	m_peNormalizedAnnual = other.m_peNormalizedAnnual;
	m_pfcfShareTTM = other.m_pfcfShareTTM;
	m_pfcfShareAnnual = other.m_pfcfShareAnnual;
	m_pretaxMargin5Y = other.m_pretaxMargin5Y;
	m_pretaxMarginAnnual = other.m_pretaxMarginAnnual;
	m_pretaxMarginTTM = other.m_pretaxMarginTTM;
	m_priceRelativeToSP50013Week = other.m_priceRelativeToSP50013Week;
	m_priceRelativeToSP50026Week = other.m_priceRelativeToSP50026Week;
	m_priceRelativeToSP5004Week = other.m_priceRelativeToSP5004Week;
	m_priceRelativeToSP50052Week = other.m_priceRelativeToSP50052Week;
	m_priceRelativeToSP500Ytd = other.m_priceRelativeToSP500Ytd;
	m_psAnnual = other.m_psAnnual;
	m_psTTM = other.m_psTTM;
	m_ptbvAnnual = other.m_ptbvAnnual;
	m_ptbvQuarterly = other.m_ptbvQuarterly;

	m_quickRatioAnnual = other.m_quickRatioAnnual;
	m_quickRatioQuarterly = other.m_quickRatioQuarterly;
	m_receivablesTurnoverAnnual = other.m_receivablesTurnoverAnnual;
	m_receivablesTurnoverTTM = other.m_receivablesTurnoverTTM;
	m_revenueEmployeeAnnual = other.m_revenueEmployeeAnnual;
	m_revenueEmployeeTTM = other.m_revenueEmployeeTTM;
	m_revenueGrowth5Y = other.m_revenueGrowth5Y;
	m_revenueGrowth3Y = other.m_revenueGrowth3Y;
	m_revenueGrowthQuarterlyYoy = other.m_revenueGrowthQuarterlyYoy;
	m_revenueGrowthTTMYoy = other.m_revenueGrowthTTMYoy;
	m_revenuePerShareAnnual = other.m_revenuePerShareAnnual;
	m_revenuePerShareTTM = other.m_revenuePerShareTTM;
	m_revenueShareGrowth5Y = other.m_revenueShareGrowth5Y;
	m_roaRfy = other.m_roaRfy;
	m_roaa5Y = other.m_roaa5Y;
	m_roae5Y = other.m_roae5Y;
	m_roaeTTM = other.m_roaeTTM;
	m_roeRfy = other.m_roeRfy;
	m_roeTTM = other.m_roeTTM;
	m_roi5Y = other.m_roi5Y;
	m_roiAnnual = other.m_roiAnnual;
	m_roiTTM = other.m_roiTTM;

	m_tangibleBookValuePerShareAnnual = other.m_tangibleBookValuePerShareAnnual;
	m_tangibleBookValuePerShareQuarterly = other.m_tangibleBookValuePerShareQuarterly;
	m_tbvCagr5Y = other.m_tbvCagr5Y;
	m_totalDebt_totalEquityAnnual = other.m_totalDebt_totalEquityAnnual;
	m_totalDebt_totalEquityQuarterly = other.m_totalDebt_totalEquityQuarterly;
	m_totalDebtCagr5Y = other.m_totalDebtCagr5Y;

	m_yearToDatePriceReturnDaily = other.m_yearToDatePriceReturnDaily;

	m_annual.m_cashRatio = std::move(other.m_annual.m_cashRatio);
	m_annual.m_currentRatio = std::move(other.m_annual.m_currentRatio);
	m_annual.m_ebitPerShare = std::move(other.m_annual.m_ebitPerShare);
	m_annual.m_eps = std::move(other.m_annual.m_eps);
	m_annual.m_grossMargin = std::move(other.m_annual.m_grossMargin);
	m_annual.m_longtermDebtTotalAsset = std::move(other.m_annual.m_longtermDebtTotalAsset);
	m_annual.m_longtermDebtTotalCapital = std::move(other.m_annual.m_longtermDebtTotalCapital);
	m_annual.m_longtermDebtTotalEquity = std::move(other.m_annual.m_longtermDebtTotalEquity);
	m_annual.m_netDebtToTotalCapital = std::move(other.m_annual.m_netDebtToTotalCapital);
	m_annual.m_netDebtToTotalEquity = std::move(other.m_annual.m_netDebtToTotalEquity);
	m_annual.m_netMargin = std::move(other.m_annual.m_netMargin);
	m_annual.m_operatingMargin = std::move(other.m_annual.m_operatingMargin);
	m_annual.m_pretaxMargin = std::move(other.m_annual.m_pretaxMargin);
	m_annual.m_salesPerShare = std::move(other.m_annual.m_salesPerShare);
	m_annual.m_sgaToSale = std::move(other.m_annual.m_sgaToSale);
	m_annual.m_totalDebtToEquity = std::move(other.m_annual.m_totalDebtToEquity);
	m_annual.m_totalDebtToTotalAsset = std::move(other.m_annual.m_totalDebtToTotalAsset);
	m_annual.m_totalDebtToTotalCapital = std::move(other.m_annual.m_totalDebtToTotalCapital);
	m_annual.m_totalRatio = std::move(other.m_annual.m_totalRatio);

	m_quarter.m_cashRatio = std::move(other.m_quarter.m_cashRatio);
	m_quarter.m_currentRatio = std::move(other.m_quarter.m_currentRatio);
	m_quarter.m_ebitPerShare = std::move(other.m_quarter.m_ebitPerShare);
	m_quarter.m_eps = std::move(other.m_quarter.m_eps);
	m_quarter.m_grossMargin = std::move(other.m_quarter.m_grossMargin);
	m_quarter.m_longtermDebtTotalAsset = std::move(other.m_quarter.m_longtermDebtTotalAsset);
	m_quarter.m_longtermDebtTotalCapital = std::move(other.m_quarter.m_longtermDebtTotalCapital);
	m_quarter.m_longtermDebtTotalEquity = std::move(other.m_quarter.m_longtermDebtTotalEquity);
	m_quarter.m_netDebtToTotalCapital = std::move(other.m_quarter.m_netDebtToTotalCapital);
	m_quarter.m_netDebtToTotalEquity = std::move(other.m_quarter.m_netDebtToTotalEquity);
	m_quarter.m_netMargin = std::move(other.m_quarter.m_netMargin);
	m_quarter.m_operatingMargin = std::move(other.m_quarter.m_operatingMargin);
	m_quarter.m_pretaxMargin = std::move(other.m_quarter.m_pretaxMargin);
	m_quarter.m_salesPerShare = std::move(other.m_quarter.m_salesPerShare);
	m_quarter.m_sgaToSale = std::move(other.m_quarter.m_sgaToSale);
	m_quarter.m_totalDebtToEquity = std::move(other.m_quarter.m_totalDebtToEquity);
	m_quarter.m_totalDebtToTotalAsset = std::move(other.m_quarter.m_totalDebtToTotalAsset);
	m_quarter.m_totalDebtToTotalCapital = std::move(other.m_quarter.m_totalDebtToTotalCapital);
	m_quarter.m_totalRatio = std::move(other.m_quarter.m_totalRatio);
}

CFinnhubStockBasicFinancial& CFinnhubStockBasicFinancial::operator=(CFinnhubStockBasicFinancial&& other) noexcept {
	m_symbol = std::move(other.m_symbol);
	m_AverageTradingVolume10Day = other.m_AverageTradingVolume10Day;
	m_High52Week = other.m_High52Week;
	m_HighDate52Week = other.m_HighDate52Week;
	m_Low52Week = other.m_Low52Week;
	m_LowDate52Week = other.m_LowDate52Week;
	m_PriceReturnDaily52Week = other.m_PriceReturnDaily52Week;
	m_PriceReturnDaily13Week = other.m_PriceReturnDaily13Week;
	m_PriceReturnDaily26Week = other.m_PriceReturnDaily26Week;
	m_AverageTradingVolume2Month = other.m_AverageTradingVolume2Month;
	m_PriceReturnDaily5Day = other.m_PriceReturnDaily5Day;
	m_assetTurnoverAnnual = other.m_assetTurnoverAnnual;
	m_assetTurnoverTTM = other.m_assetTurnoverTTM;
	m_beta = other.m_beta;
	m_bookValuePerShareAnnual = other.m_bookValuePerShareAnnual;
	m_bookValuePerShareQuarterly = other.m_bookValuePerShareQuarterly;
	m_bookValueShareGrowth5Y = other.m_bookValueShareGrowth5Y;
	m_capitalSpendingGrowth5Y = other.m_capitalSpendingGrowth5Y;
	m_cashFlowPerShareAnnual = other.m_cashFlowPerShareAnnual;
	m_cashFlowPerShareTTM = other.m_cashFlowPerShareTTM;
	m_cashPerSharePerShareAnnual = other.m_cashPerSharePerShareAnnual;
	m_cashPerSharePerShareQuarterly = other.m_cashPerSharePerShareQuarterly;
	m_currentDividendYieldTTM = other.m_currentDividendYieldTTM;
	m_currentEV_freeCashFlowAnnual = other.m_currentEV_freeCashFlowAnnual;
	m_currentEV_freeCashFlowTTM = other.m_currentEV_freeCashFlowTTM;
	m_currentRatioAnnual = other.m_currentRatioAnnual;
	m_currentRatioQuarterly = other.m_currentRatioQuarterly;
	m_dividendGrowthRate5Y = other.m_dividendGrowthRate5Y;
	m_dividendPerShare5Y = other.m_dividendPerShare5Y;
	m_dividendPerShareAnnual = other.m_dividendPerShareAnnual;
	m_dividendYield5Y = other.m_dividendYield5Y;
	m_dividendYieldIndicatedAnnual = other.m_dividendYieldIndicatedAnnual;
	m_dividendsPerShareTTM = other.m_dividendsPerShareTTM;
	m_ebitdaCagr5Y = other.m_ebitdaCagr5Y;
	m_ebitdaInterimCagr5Y = other.m_ebitdaInterimCagr5Y;
	m_ebitdPerShareTTM = other.m_ebitdPerShareTTM;
	m_epsBasicExclExtraItemsAnnual = other.m_epsBasicExclExtraItemsAnnual;
	m_epsBasicExclExtraItemsTTM = other.m_epsBasicExclExtraItemsTTM;
	m_epsExclExtraItemsAnnual = other.m_epsExclExtraItemsAnnual;
	m_epsExclExtraItemsTTM = other.m_epsExclExtraItemsTTM;
	m_epsGrowth3Y = other.m_epsGrowth3Y;
	m_epsGrowth5Y = other.m_epsGrowth5Y;
	m_epsGrowthQuarterlyYoy = other.m_epsGrowthQuarterlyYoy;
	m_epsGrowthTTMYoy = other.m_epsGrowthTTMYoy;
	m_epsInclExtraItemsAnnual = other.m_epsInclExtraItemsAnnual;
	m_epsInclExtraItemsTTM = other.m_epsInclExtraItemsTTM;
	m_epsNormalizedAnnual = other.m_epsNormalizedAnnual;
	m_focfCagr5Y = other.m_focfCagr5Y;
	m_freeCashFlowAnnual = other.m_freeCashFlowAnnual;
	m_freeCashFlowPerShareTTM = other.m_freeCashFlowPerShareTTM;
	m_freeCashFlowTTM = other.m_freeCashFlowTTM;
	m_freeOperatingCashFlow_revenue5Y = other.m_freeOperatingCashFlow_revenue5Y;
	m_freeOperatingCashFlow_revenueTTM = other.m_freeOperatingCashFlow_revenueTTM;

	m_grossMargin5Y = other.m_grossMargin5Y;
	m_grossMarginAnnual = other.m_grossMarginAnnual;
	m_grossMarginTTM = other.m_grossMarginTTM;
	m_inventoryTurnoverAnnual = other.m_inventoryTurnoverAnnual;
	m_inventoryTurnoverTTM = other.m_inventoryTurnoverTTM;

	m_longTermDebt_equityAnnual = other.m_longTermDebt_equityAnnual;
	m_longTermDebt_equityQuarterly = other.m_longTermDebt_equityQuarterly;

	m_marketCapitalization = other.m_marketCapitalization;
	m_monthToDatePriceReturnDaily = other.m_monthToDatePriceReturnDaily;

	m_netDebtAnnual = other.m_netDebtAnnual;
	m_netDebtInterim = other.m_netDebtInterim;
	m_netIncomeEmployeeAnnual = other.m_netIncomeEmployeeAnnual;
	m_netIncomeEmployeeTTM = other.m_netIncomeEmployeeTTM;
	m_netInterestCoverageAnnual = other.m_netInterestCoverageAnnual;
	m_netInterestCoverageTTM = other.m_netInterestCoverageTTM;
	m_netMarginGrowth5Y = other.m_netMarginGrowth5Y;
	m_netProfitMargin5Y = other.m_netProfitMargin5Y;
	m_netProfitMarginAnnual = other.m_netProfitMarginAnnual;
	m_netProfitMarginTTM = other.m_netProfitMarginTTM;

	m_operatingMargin5Y = other.m_operatingMargin5Y;
	m_operatingMarginAnnual = other.m_operatingMarginAnnual;
	m_operatingMarginTTM = other.m_operatingMarginTTM;

	m_payoutRatioAnnual = other.m_payoutRatioAnnual;
	m_payoutRatioTTM = other.m_payoutRatioTTM;
	m_pbAnnual = other.m_pbAnnual;
	m_pbQuarterly = other.m_pbQuarterly;
	m_pcfShareTTM = other.m_pcfShareTTM;
	m_peBasicExclExtraTTM = other.m_peBasicExclExtraTTM;
	m_peExclExtraAnnual = other.m_peExclExtraAnnual;
	m_peExclExtraHighTTM = other.m_peExclExtraHighTTM;
	m_peExclExtraTTM = other.m_peExclExtraTTM;
	m_peExclLowTTM = other.m_peExclLowTTM;
	m_peInclExtraTTM = other.m_peInclExtraTTM;
	m_peNormalizedAnnual = other.m_peNormalizedAnnual;
	m_pfcfShareTTM = other.m_pfcfShareTTM;
	m_pfcfShareAnnual = other.m_pfcfShareAnnual;
	m_pretaxMargin5Y = other.m_pretaxMargin5Y;
	m_pretaxMarginAnnual = other.m_pretaxMarginAnnual;
	m_pretaxMarginTTM = other.m_pretaxMarginTTM;
	m_priceRelativeToSP50013Week = other.m_priceRelativeToSP50013Week;
	m_priceRelativeToSP50026Week = other.m_priceRelativeToSP50026Week;
	m_priceRelativeToSP5004Week = other.m_priceRelativeToSP5004Week;
	m_priceRelativeToSP50052Week = other.m_priceRelativeToSP50052Week;
	m_priceRelativeToSP500Ytd = other.m_priceRelativeToSP500Ytd;
	m_psAnnual = other.m_psAnnual;
	m_psTTM = other.m_psTTM;
	m_ptbvAnnual = other.m_ptbvAnnual;
	m_ptbvQuarterly = other.m_ptbvQuarterly;

	m_quickRatioAnnual = other.m_quickRatioAnnual;
	m_quickRatioQuarterly = other.m_quickRatioQuarterly;
	m_receivablesTurnoverAnnual = other.m_receivablesTurnoverAnnual;
	m_receivablesTurnoverTTM = other.m_receivablesTurnoverTTM;
	m_revenueEmployeeAnnual = other.m_revenueEmployeeAnnual;
	m_revenueEmployeeTTM = other.m_revenueEmployeeTTM;
	m_revenueGrowth5Y = other.m_revenueGrowth5Y;
	m_revenueGrowth3Y = other.m_revenueGrowth3Y;
	m_revenueGrowthQuarterlyYoy = other.m_revenueGrowthQuarterlyYoy;
	m_revenueGrowthTTMYoy = other.m_revenueGrowthTTMYoy;
	m_revenuePerShareAnnual = other.m_revenuePerShareAnnual;
	m_revenuePerShareTTM = other.m_revenuePerShareTTM;
	m_revenueShareGrowth5Y = other.m_revenueShareGrowth5Y;
	m_roaRfy = other.m_roaRfy;
	m_roaa5Y = other.m_roaa5Y;
	m_roae5Y = other.m_roae5Y;
	m_roaeTTM = other.m_roaeTTM;
	m_roeRfy = other.m_roeRfy;
	m_roeTTM = other.m_roeTTM;
	m_roi5Y = other.m_roi5Y;
	m_roiAnnual = other.m_roiAnnual;
	m_roiTTM = other.m_roiTTM;

	m_tangibleBookValuePerShareAnnual = other.m_tangibleBookValuePerShareAnnual;
	m_tangibleBookValuePerShareQuarterly = other.m_tangibleBookValuePerShareQuarterly;
	m_tbvCagr5Y = other.m_tbvCagr5Y;
	m_totalDebt_totalEquityAnnual = other.m_totalDebt_totalEquityAnnual;
	m_totalDebt_totalEquityQuarterly = other.m_totalDebt_totalEquityQuarterly;
	m_totalDebtCagr5Y = other.m_totalDebtCagr5Y;

	m_yearToDatePriceReturnDaily = other.m_yearToDatePriceReturnDaily;

	m_annual.m_cashRatio = std::move(other.m_annual.m_cashRatio);
	m_annual.m_currentRatio = std::move(other.m_annual.m_currentRatio);
	m_annual.m_ebitPerShare = std::move(other.m_annual.m_ebitPerShare);
	m_annual.m_eps = std::move(other.m_annual.m_eps);
	m_annual.m_grossMargin = std::move(other.m_annual.m_grossMargin);
	m_annual.m_longtermDebtTotalAsset = std::move(other.m_annual.m_longtermDebtTotalAsset);
	m_annual.m_longtermDebtTotalCapital = std::move(other.m_annual.m_longtermDebtTotalCapital);
	m_annual.m_longtermDebtTotalEquity = std::move(other.m_annual.m_longtermDebtTotalEquity);
	m_annual.m_netDebtToTotalCapital = std::move(other.m_annual.m_netDebtToTotalCapital);
	m_annual.m_netDebtToTotalEquity = std::move(other.m_annual.m_netDebtToTotalEquity);
	m_annual.m_netMargin = std::move(other.m_annual.m_netMargin);
	m_annual.m_operatingMargin = std::move(other.m_annual.m_operatingMargin);
	m_annual.m_pretaxMargin = std::move(other.m_annual.m_pretaxMargin);
	m_annual.m_salesPerShare = std::move(other.m_annual.m_salesPerShare);
	m_annual.m_sgaToSale = std::move(other.m_annual.m_sgaToSale);
	m_annual.m_totalDebtToEquity = std::move(other.m_annual.m_totalDebtToEquity);
	m_annual.m_totalDebtToTotalAsset = std::move(other.m_annual.m_totalDebtToTotalAsset);
	m_annual.m_totalDebtToTotalCapital = std::move(other.m_annual.m_totalDebtToTotalCapital);
	m_annual.m_totalRatio = std::move(other.m_annual.m_totalRatio);

	m_quarter.m_cashRatio = std::move(other.m_quarter.m_cashRatio);
	m_quarter.m_currentRatio = std::move(other.m_quarter.m_currentRatio);
	m_quarter.m_ebitPerShare = std::move(other.m_quarter.m_ebitPerShare);
	m_quarter.m_eps = std::move(other.m_quarter.m_eps);
	m_quarter.m_grossMargin = std::move(other.m_quarter.m_grossMargin);
	m_quarter.m_longtermDebtTotalAsset = std::move(other.m_quarter.m_longtermDebtTotalAsset);
	m_quarter.m_longtermDebtTotalCapital = std::move(other.m_quarter.m_longtermDebtTotalCapital);
	m_quarter.m_longtermDebtTotalEquity = std::move(other.m_quarter.m_longtermDebtTotalEquity);
	m_quarter.m_netDebtToTotalCapital = std::move(other.m_quarter.m_netDebtToTotalCapital);
	m_quarter.m_netDebtToTotalEquity = std::move(other.m_quarter.m_netDebtToTotalEquity);
	m_quarter.m_netMargin = std::move(other.m_quarter.m_netMargin);
	m_quarter.m_operatingMargin = std::move(other.m_quarter.m_operatingMargin);
	m_quarter.m_pretaxMargin = std::move(other.m_quarter.m_pretaxMargin);
	m_quarter.m_salesPerShare = std::move(other.m_quarter.m_salesPerShare);
	m_quarter.m_sgaToSale = std::move(other.m_quarter.m_sgaToSale);
	m_quarter.m_totalDebtToEquity = std::move(other.m_quarter.m_totalDebtToEquity);
	m_quarter.m_totalDebtToTotalAsset = std::move(other.m_quarter.m_totalDebtToTotalAsset);
	m_quarter.m_totalDebtToTotalCapital = std::move(other.m_quarter.m_totalDebtToTotalCapital);
	m_quarter.m_totalRatio = std::move(other.m_quarter.m_totalRatio);
	return *this;
}

void CFinnhubStockBasicFinancial::Reset() {
	m_symbol = "";
	m_AverageTradingVolume10Day = 0.0;
	m_High52Week = 0.0;
	m_HighDate52Week = 19800101;
	m_Low52Week = 0.0;
	m_LowDate52Week = 19800101;
	m_PriceReturnDaily52Week = 0.0;
	m_PriceReturnDaily13Week = 0.0;
	m_PriceReturnDaily26Week = 0.0;
	m_AverageTradingVolume2Month = 0.0;
	m_PriceReturnDaily5Day = 0.0;

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

	m_annual.Reset();
	m_quarter.Reset();
}

void CFinnhubStockBasicFinancial::LoadMetric(const CSetFinnhubStockBasicFinancialMetric& setMetric) {
	ASSERT(setMetric.IsOpen());

	m_symbol = T2Utf8(setMetric.m_symbol);

	m_AverageTradingVolume10Day = setMetric.m_AverageTradingVolume10Day;
	m_PriceReturnDaily13Week = setMetric.m_PriceReturnDaily13Week;
	m_PriceReturnDaily26Week = setMetric.m_PriceReturnDaily26Week;
	m_AverageTradingVolume2Month = setMetric.m_AverageTradingVolume3Month;
	m_High52Week = setMetric.m_High52Week;
	m_HighDate52Week = setMetric.m_HighDate52Week;
	m_Low52Week = setMetric.m_Low52Week;
	m_LowDate52Week = setMetric.m_LowDate52Week;
	m_PriceReturnDaily52Week = setMetric.m_PriceReturnDaily52Week;
	m_PriceReturnDaily5Day = setMetric.m_PriceReturnDaily5Day;

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

void CFinnhubStockBasicFinancial::AppendQuarterData(CSetFinnhubStockBasicFinancialQuarter& setBeSaved) {
	CSetFinnhubStockBasicFinancialQuarter setQuarterly;
	vector<CItemOfBasicFinancialSeasonData> vData;
	CItemOfBasicFinancialSeasonData data;

	ASSERT(setBeSaved.IsOpen());
	setQuarterly.m_strFilter = "[Symbol] = '";
	setQuarterly.m_strFilter += m_symbol.c_str();
	setQuarterly.m_strFilter += "'";
	setQuarterly.Open();
	while (!setQuarterly.IsEOF()) {
		data.m_symbol = T2Utf8(setQuarterly.m_symbol);
		data.m_date = setQuarterly.m_date;
		data.m_type = T2Utf8(setQuarterly.m_type);
		data.m_value = setQuarterly.m_value;
		vData.push_back(data);
		setQuarterly.MoveNext();
	}
	setQuarterly.Close();

	SaveAllQuarterData(setBeSaved, vData);
}

void CFinnhubStockBasicFinancial::AppendAnnualData(CSetFinnhubStockBasicFinancialAnnual& setBeSaved) {
	CSetFinnhubStockBasicFinancialAnnual setAnnual;
	vector<CItemOfBasicFinancialSeasonData> vData;
	CItemOfBasicFinancialSeasonData data;

	ASSERT(setBeSaved.IsOpen());
	setAnnual.m_strFilter = "[Symbol] = '";
	setAnnual.m_strFilter += m_symbol.c_str();
	setAnnual.m_strFilter += "'";
	setAnnual.Open();
	while (!setAnnual.IsEOF()) {
		data.m_symbol = T2Utf8(setAnnual.m_symbol);
		data.m_date = setAnnual.m_date;
		data.m_type = T2Utf8(setAnnual.m_type);
		data.m_value = setAnnual.m_value;
		vData.push_back(data);
		setAnnual.MoveNext();
	}
	setAnnual.Close();

	SaveAllAnnualData(setBeSaved, vData);
}

void CFinnhubStockBasicFinancial::SaveAllQuarterData(CSetFinnhubStockBasicFinancialQuarter& SetQuarterData, const vector<CItemOfBasicFinancialSeasonData>& vDBData) {
	SaveQuarterData(SetQuarterData, m_quarter.m_cashRatio, "cashRatio", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_currentRatio, "currentRatio", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_ebitPerShare, "ebitPerShare", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_eps, "eps", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_grossMargin, "grossMargin", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_longtermDebtTotalAsset, "longtermDebtTotalAsset", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_longtermDebtTotalCapital, "longtermDebtTotalCapital", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_longtermDebtTotalEquity, "longtermDebtTotalEquity", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_netDebtToTotalCapital, "netDebtToTotalCapital", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_netDebtToTotalEquity, "netDebtToTotalEquity", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_netMargin, "netMargin", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_operatingMargin, "operatingMargin", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_pretaxMargin, "pretaxMargin", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_salesPerShare, "salesPerShare", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_sgaToSale, "sgaToSale", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_totalDebtToEquity, "totalDebtToEquity", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_totalDebtToTotalAsset, "totalDebtToTotalAsset", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_totalDebtToTotalCapital, "totalDebtToTotalCapital", vDBData);
	SaveQuarterData(SetQuarterData, m_quarter.m_totalRatio, "totalRatio", vDBData);
}

void CFinnhubStockBasicFinancial::SaveQuarterData(CSetFinnhubStockBasicFinancialQuarter& SetQuarterData, const vector<CValueOfPeriod>& vData, const string& typeName, const vector<CItemOfBasicFinancialSeasonData>& vDBData) {
	ASSERT(SetQuarterData.IsOpen());
	for (size_t i = 0; i < vData.size(); i++) {
		if (IsNewData(typeName, vData.at(i), vDBData)) {
			SetQuarterData.AddNew();
			SetQuarterData.m_symbol = m_symbol.c_str();
			SetQuarterData.m_type = typeName.c_str();
			SetQuarterData.m_date = vData.at(i).m_period;
			SetQuarterData.m_value = vData.at(i).m_value;
			SetQuarterData.Update();
		}
	}
}

void CFinnhubStockBasicFinancial::LoadQuarterData(CSetFinnhubStockBasicFinancialQuarter& SetQuarterData) {
	// not implemented
}

void CFinnhubStockBasicFinancial::SaveAllAnnualData(CSetFinnhubStockBasicFinancialAnnual& setAnnualData, const vector<CItemOfBasicFinancialSeasonData>& vDBData) {
	SaveAnnualData(setAnnualData, m_annual.m_cashRatio, "cashRatio", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_currentRatio, "currentRatio", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_ebitPerShare, "ebitPerShare", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_eps, "eps", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_grossMargin, "grossMargin", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_longtermDebtTotalAsset, "longtermDebtTotalAsset", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_longtermDebtTotalCapital, "longtermDebtTotalCapital", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_longtermDebtTotalEquity, "longtermDebtTotalEquity", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_netDebtToTotalCapital, "netDebtToTotalCapital", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_netDebtToTotalEquity, "netDebtToTotalEquity", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_netMargin, "netMargin", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_operatingMargin, "operatingMargin", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_pretaxMargin, "pretaxMargin", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_salesPerShare, "salesPerShare", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_sgaToSale, "sgaToSale", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_totalDebtToEquity, "totalDebtToEquity", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_totalDebtToTotalAsset, "totalDebtToTotalAsset", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_totalDebtToTotalCapital, "totalDebtToTotalCapital", vDBData);
	SaveAnnualData(setAnnualData, m_annual.m_totalRatio, "totalRatio", vDBData);
}

void CFinnhubStockBasicFinancial::SaveAnnualData(CSetFinnhubStockBasicFinancialAnnual& setAnnualData, const vector<CValueOfPeriod>& vData, const string& typeName, const vector<CItemOfBasicFinancialSeasonData>& vDBData) {
	ASSERT(setAnnualData.IsOpen());
	for (size_t i = 0; i < vData.size(); i++) {
		if (IsNewData(typeName, vData.at(i), vDBData)) {
			setAnnualData.AddNew();
			setAnnualData.m_symbol = m_symbol.c_str();
			setAnnualData.m_type = typeName.c_str();
			setAnnualData.m_date = vData.at(i).m_period;
			setAnnualData.m_value = vData.at(i).m_value;
			setAnnualData.Update();
		}
	}
}

void CFinnhubStockBasicFinancial::LoadAnnualData(CSetFinnhubStockBasicFinancialAnnual& setAnnualData) {
	// not implemented
}

bool CFinnhubStockBasicFinancial::IsNewData(const string& type, CValueOfPeriod vData, const vector<CItemOfBasicFinancialSeasonData>& vDBData) {
	for (size_t i = 0; i < vDBData.size(); i++) {
		if (vDBData.at(i).m_type == type && vDBData.at(i).m_date == vData.m_period) {
			return false;
		}
	}
	return true;
}

void CFinnhubStockBasicFinancial::AppendMetric(CSetFinnhubStockBasicFinancialMetric& setMetric) const {
	ASSERT(setMetric.IsOpen());
	setMetric.AddNew();
	SaveMetric(setMetric);
	setMetric.Update();
}

void CFinnhubStockBasicFinancial::UpdateMetric(CSetFinnhubStockBasicFinancialMetric& setMetric) const {
	ASSERT(setMetric.IsOpen());
	setMetric.Edit();
	SaveMetric(setMetric);
	setMetric.Update();
}

void CFinnhubStockBasicFinancial::SaveMetric(CSetFinnhubStockBasicFinancialMetric& setMetric) const {
	setMetric.m_symbol = m_symbol.c_str();
	setMetric.m_AverageTradingVolume10Day = m_AverageTradingVolume10Day;
	setMetric.m_PriceReturnDaily13Week = m_PriceReturnDaily13Week;
	setMetric.m_PriceReturnDaily26Week = m_PriceReturnDaily26Week;
	setMetric.m_AverageTradingVolume3Month = m_AverageTradingVolume2Month;
	setMetric.m_High52Week = m_High52Week;
	setMetric.m_HighDate52Week = m_HighDate52Week;
	setMetric.m_Low52Week = m_Low52Week;
	setMetric.m_LowDate52Week = m_LowDate52Week;
	setMetric.m_PriceReturnDaily52Week = m_PriceReturnDaily52Week;
	setMetric.m_PriceReturnDaily5Day = m_PriceReturnDaily5Day;

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
