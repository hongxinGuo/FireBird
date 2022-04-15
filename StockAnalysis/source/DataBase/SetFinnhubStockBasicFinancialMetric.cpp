// SetFinnhubStockBasicFinancialMetric.cpp : CSetFinnhubStockBasicFinancialMetric 类的实现

// 代码生成在 2019年6月2日, 5:10

#include"pch.h"
#include"globedef.h"

#include "SetFinnhubStockBasicFinancialMetric.h"

IMPLEMENT_DYNAMIC(CSetFinnhubStockBasicFinancialMetric, CVirtualRecordset)

CSetFinnhubStockBasicFinancialMetric::CSetFinnhubStockBasicFinancialMetric(CString strSchema, CString strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_ID = 0;
	m_symbol = _T("");
	m_13WeekPriceReturnDaily = 0;
	m_26WeekPriceReturnDaily = 0;
	m_52WeekPriceReturnDaily = 0;

	m_nFields = 131; // 129个实际字段，一个ID，一个代码名称。
}

void CSetFinnhubStockBasicFinancialMetric::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Int() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("[Symbol]"), m_symbol);

	RFX_Double(pFX, _T("[10DayAverageTradingVolume]"), m_10DayAverageTradingVolume);
	RFX_Double(pFX, _T("[13WeekPriceReturnDaily]"), m_13WeekPriceReturnDaily);
	RFX_Double(pFX, _T("[26WeekPriceReturnDaily]"), m_26WeekPriceReturnDaily);
	RFX_Double(pFX, _T("[3MonthAverageTradingVolume]"), m_3MonthAverageTradingVolume);
	RFX_Double(pFX, _T("[52WeekHigh]"), m_52WeekHigh);
	RFX_Int(pFX, _T("[52WeekHighDate]"), m_52WeekHighDate);
	RFX_Double(pFX, _T("[52WeekLow]"), m_52WeekLow);
	RFX_Int(pFX, _T("[52WeekLowDate]"), m_52WeekLowDate);
	RFX_Double(pFX, _T("[52WeekPriceReturnDaily]"), m_52WeekPriceReturnDaily);
	RFX_Double(pFX, _T("[5DayPriceReturnDaily]"), m_5DayPriceReturnDaily);

	RFX_Double(pFX, _T("[assetTurnoverAnnual]"), m_assetTurnoverAnnual);
	RFX_Double(pFX, _T("[assetTurnoverTTM]"), m_assetTurnoverTTM);

	RFX_Double(pFX, _T("[beta]"), m_beta);
	RFX_Double(pFX, _T("[bookValuePerShareAnnual]"), m_bookValuePerShareAnnual);
	RFX_Double(pFX, _T("[bookValuePerShareQuarterly]"), m_bookValuePerShareQuarterly);
	RFX_Double(pFX, _T("[bookValueShareGrowth5Y]"), m_bookValueShareGrowth5Y);

	RFX_Double(pFX, _T("[capitalSpendingGrowth5Y]"), m_capitalSpendingGrowth5Y);
	RFX_Double(pFX, _T("[cashFlowPerShareAnnual]"), m_cashFlowPerShareAnnual);
	RFX_Double(pFX, _T("[cashFlowPerShareTTM]"), m_cashFlowPerShareTTM);
	RFX_Double(pFX, _T("[cashPerSharePerShareAnnual]"), m_cashPerSharePerShareAnnual);
	RFX_Double(pFX, _T("[cashPerSharePerShareQuarterly]"), m_cashPerSharePerShareQuarterly);
	RFX_Double(pFX, _T("[currentDividendYieldTTM]"), m_currentDividendYieldTTM);
	RFX_Double(pFX, _T("[currentEv_freeCashFlowAnnual]"), m_currentEV_freeCashFlowAnnual);
	RFX_Double(pFX, _T("[currentEv_freeCashFlowTTM]"), m_currentEV_freeCashFlowTTM);
	RFX_Double(pFX, _T("[currentRatioAnnual]"), m_currentRatioAnnual);
	RFX_Double(pFX, _T("[currentRatioQuarterly]"), m_currentRatioQuarterly);

	RFX_Double(pFX, _T("[dividendGrowthRate5Y]"), m_dividendGrowthRate5Y);
	RFX_Double(pFX, _T("[dividendPerShare5Y]"), m_dividendPerShare5Y);
	RFX_Double(pFX, _T("[dividendPerShareAnnual]"), m_dividendPerShareAnnual);
	RFX_Double(pFX, _T("[dividendYield5Y]"), m_dividendYield5Y);
	RFX_Double(pFX, _T("[dividendYieldIndicatedAnnual]"), m_dividendYieldIndicatedAnnual);
	RFX_Double(pFX, _T("[dividendsPerShareTTM]"), m_dividendsPerShareTTM);

	RFX_Double(pFX, _T("[ebitdPerShareTTM]"), m_ebitdPerShareTTM);
	RFX_Double(pFX, _T("[ebitdaCagr5Y]"), m_ebitdaCagr5Y);
	RFX_Double(pFX, _T("[ebitdaInterimCagr5Y]"), m_ebitdaInterimCagr5Y);
	RFX_Double(pFX, _T("[epsBasicExclExtraItemsAnnual]"), m_epsBasicExclExtraItemsAnnual);
	RFX_Double(pFX, _T("[epsBasicExclExtraItemsTTM]"), m_epsBasicExclExtraItemsTTM);
	RFX_Double(pFX, _T("[epsExclExtraItemsAnnual]"), m_epsExclExtraItemsAnnual);
	RFX_Double(pFX, _T("[epsExclExtraItemsTTM]"), m_epsExclExtraItemsTTM);
	RFX_Double(pFX, _T("[epsGrowth3Y]"), m_epsGrowth3Y);
	RFX_Double(pFX, _T("[epsGrowth5Y]"), m_epsGrowth5Y);
	RFX_Double(pFX, _T("[epsGrowthQuarterlyYoy]"), m_epsGrowthQuarterlyYoy);
	RFX_Double(pFX, _T("[epsGrowthTTMYoy]"), m_epsGrowthTTMYoy);
	RFX_Double(pFX, _T("[epsInclExtraItemsAnnual]"), m_epsInclExtraItemsAnnual);
	RFX_Double(pFX, _T("[epsInclExtraItemsTTM]"), m_epsInclExtraItemsTTM);
	RFX_Double(pFX, _T("[epsNormalizedAnnual]"), m_epsNormalizedAnnual);

	RFX_Double(pFX, _T("[focfCagr5Y]"), m_focfCagr5Y);
	RFX_Double(pFX, _T("[freeCashFlowAnnual]"), m_freeCashFlowAnnual);
	RFX_Double(pFX, _T("[freeCashFlowPerShareTTM]"), m_freeCashFlowPerShareTTM);
	RFX_Double(pFX, _T("[freeCashFlowTTM]"), m_freeCashFlowTTM);
	RFX_Double(pFX, _T("[freeOperatingCashFlow_revenue5Y]"), m_freeOperatingCashFlow_revenue5Y);
	RFX_Double(pFX, _T("[freeOperatingCashFlow_revenueTTM]"), m_freeOperatingCashFlow_revenueTTM);

	RFX_Double(pFX, _T("[grossMargin5Y]"), m_grossMargin5Y);
	RFX_Double(pFX, _T("[grossMarginAnnual]"), m_grossMarginAnnual);
	RFX_Double(pFX, _T("[grossMarginTTM]"), m_grossMarginTTM);

	RFX_Double(pFX, _T("[inventoryTurnoverAnnual]"), m_inventoryTurnoverAnnual);
	RFX_Double(pFX, _T("[inventoryTurnoverTTM]"), m_inventoryTurnoverTTM);

	RFX_Double(pFX, _T("[longTermDebt_equityAnnual]"), m_longTermDebt_equityAnnual);
	RFX_Double(pFX, _T("[longTermDebt_equityQuarterly]"), m_longTermDebt_equityQuarterly);

	RFX_Double(pFX, _T("[marketCapitalization]"), m_marketCapitalization);
	RFX_Double(pFX, _T("[monthToDatePriceReturnDaily]"), m_monthToDatePriceReturnDaily);

	RFX_Double(pFX, _T("[netDebtAnnual]"), m_netDebtAnnual);
	RFX_Double(pFX, _T("[netDebtInterim]"), m_netDebtInterim);
	RFX_Double(pFX, _T("[netIncomeEmployeeAnnual]"), m_netIncomeEmployeeAnnual);
	RFX_Double(pFX, _T("[netIncomeEmployeeTTM]"), m_netIncomeEmployeeTTM);
	RFX_Double(pFX, _T("[netInterestCoverageAnnual]"), m_netInterestCoverageAnnual);
	RFX_Double(pFX, _T("[netInterestCoverageTTM]"), m_netInterestCoverageTTM);
	RFX_Double(pFX, _T("[netMarginGrowth5Y]"), m_netMarginGrowth5Y);
	RFX_Double(pFX, _T("[netProfitMargin5Y]"), m_netProfitMargin5Y);
	RFX_Double(pFX, _T("[netProfitMarginAnnual]"), m_netProfitMarginAnnual);
	RFX_Double(pFX, _T("[netProfitMarginTTM]"), m_netProfitMarginTTM);

	RFX_Double(pFX, _T("[operatingMargin5Y]"), m_operatingMargin5Y);
	RFX_Double(pFX, _T("[operatingMarginAnnual]"), m_operatingMarginAnnual);
	RFX_Double(pFX, _T("[operatingMarginTTM]"), m_operatingMarginTTM);

	RFX_Double(pFX, _T("[payoutRatioAnnual]"), m_payoutRatioAnnual);
	RFX_Double(pFX, _T("[payoutRatioTTM]"), m_payoutRatioTTM);
	RFX_Double(pFX, _T("[pbAnnual]"), m_pbAnnual);
	RFX_Double(pFX, _T("[pbQuarterly]"), m_pbQuarterly);
	RFX_Double(pFX, _T("[pcfShareTTM]"), m_pcfShareTTM);
	RFX_Double(pFX, _T("[peBasicExclExtraTTM]"), m_peBasicExclExtraTTM);
	RFX_Double(pFX, _T("[peExclExtraAnnual]"), m_peExclExtraAnnual);
	RFX_Double(pFX, _T("[peExclExtraHighTTM]"), m_peExclExtraHighTTM);
	RFX_Double(pFX, _T("[peExclExtraTTM]"), m_peExclExtraTTM);
	RFX_Double(pFX, _T("[peExclLowTTM]"), m_peExclLowTTM);
	RFX_Double(pFX, _T("[peInclExtraTTM]"), m_peInclExtraTTM);
	RFX_Double(pFX, _T("[peNormalizedAnnual]"), m_peNormalizedAnnual);
	RFX_Double(pFX, _T("[pfcfShareAnnual]"), m_pfcfShareAnnual);
	RFX_Double(pFX, _T("[pfcfShareTTM]"), m_pfcfShareTTM);
	RFX_Double(pFX, _T("[pretaxMargin5Y]"), m_pretaxMargin5Y);
	RFX_Double(pFX, _T("[pretaxMarginAnnual]"), m_pretaxMarginAnnual);
	RFX_Double(pFX, _T("[pretaxMarginTTM]"), m_pretaxMarginTTM);
	RFX_Double(pFX, _T("[priceRelativeToSP50013Week]"), m_priceRelativeToSP50013Week);
	RFX_Double(pFX, _T("[priceRelativeToSP50026Week]"), m_priceRelativeToSP50026Week);
	RFX_Double(pFX, _T("[priceRelativeToSP5004Week]"), m_priceRelativeToSP5004Week);
	RFX_Double(pFX, _T("[PriceRelativeToSP50052Week]"), m_priceRelativeToSP50052Week);
	RFX_Double(pFX, _T("[priceRelativeToSP500Ytd]"), m_priceRelativeToSP500Ytd);
	RFX_Double(pFX, _T("[psAnnual]"), m_psAnnual);
	RFX_Double(pFX, _T("[psTTM]"), m_psTTM);
	RFX_Double(pFX, _T("[ptbvAnnual]"), m_ptbvAnnual);
	RFX_Double(pFX, _T("[ptbvQuarterly]"), m_ptbvQuarterly);

	RFX_Double(pFX, _T("[quickRatioAnnual]"), m_quickRatioAnnual);
	RFX_Double(pFX, _T("[quickRatioQuarterly]"), m_quickRatioQuarterly);

	RFX_Double(pFX, _T("[receivablesTurnoverAnnual]"), m_receivablesTurnoverAnnual);
	RFX_Double(pFX, _T("[receivablesTurnoverTTM]"), m_receivablesTurnoverTTM);
	RFX_Double(pFX, _T("[revenueEmployeeAnnual]"), m_revenueEmployeeAnnual);
	RFX_Double(pFX, _T("[revenueEmployeeTTM]"), m_revenueEmployeeTTM);
	RFX_Double(pFX, _T("[revenueGrowth3Y]"), m_revenueGrowth3Y);
	RFX_Double(pFX, _T("[revenueGrowth5Y]"), m_revenueGrowth5Y);
	RFX_Double(pFX, _T("[revenueGrowthQuarterlyYoy]"), m_revenueGrowthQuarterlyYoy);
	RFX_Double(pFX, _T("[revenueGrowthTTMYoy]"), m_revenueGrowthTTMYoy);
	RFX_Double(pFX, _T("[revenuePerShareAnnual]"), m_revenuePerShareAnnual);
	RFX_Double(pFX, _T("[revenuePerShareTTM]"), m_revenuePerShareTTM);
	RFX_Double(pFX, _T("[revenueShareGrowth5Y]"), m_revenueShareGrowth5Y);
	RFX_Double(pFX, _T("[roaRfy]"), m_roaRfy);
	RFX_Double(pFX, _T("[roaa5Y]"), m_roaa5Y);
	RFX_Double(pFX, _T("[roae5Y]"), m_roae5Y);
	RFX_Double(pFX, _T("[roaeTTM]"), m_roaeTTM);
	RFX_Double(pFX, _T("[roeRfy]"), m_roeRfy);
	RFX_Double(pFX, _T("[roeTTM]"), m_roeTTM);
	RFX_Double(pFX, _T("[roi5Y]"), m_roi5Y);
	RFX_Double(pFX, _T("[roiAnnual]"), m_roiAnnual);
	RFX_Double(pFX, _T("[roiTTM]"), m_roiTTM);

	RFX_Double(pFX, _T("[tangibleBookValuePerShareAnnual]"), m_tangibleBookValuePerShareAnnual);
	RFX_Double(pFX, _T("[tangibleBookValuePerShareQuarterly]"), m_tangibleBookValuePerShareQuarterly);
	RFX_Double(pFX, _T("[tbvCagr5Y]"), m_tbvCagr5Y);
	RFX_Double(pFX, _T("[totalDebt_totalEquityAnnual]"), m_totalDebt_totalEquityAnnual);
	RFX_Double(pFX, _T("[totalDebt_totalEquityQuarterly]"), m_totalDebt_totalEquityQuarterly);
	RFX_Double(pFX, _T("[totalDebtCagr5Y]"), m_totalDebtCagr5Y);

	RFX_Double(pFX, _T("[yearToDatePriceReturnDaily]"), m_yearToDatePriceReturnDaily);
}
/////////////////////////////////////////////////////////////////////////////
// CSetCountry 诊断

#ifdef _DEBUG
void CSetFinnhubStockBasicFinancialMetric::AssertValid() const {
	CVirtualRecordset::AssertValid();
}

void CSetFinnhubStockBasicFinancialMetric::Dump(CDumpContext& dc) const {
	CVirtualRecordset::Dump(dc);
}
#endif //_DEBUG