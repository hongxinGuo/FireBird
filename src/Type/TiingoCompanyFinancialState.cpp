#include"pch.h"

#include"TiingoCompanyFinancialState.h"

#include "ConvertToString.h"

CTiingoCompanyFinancialState::CTiingoCompanyFinancialState() {
	m_symbol = _T("");
	m_exchange = _T("");
	m_yearQuarter = 0;
	m_accoci = 0;
	m_acctPay = 0;
	m_acctRec = 0;
	m_assetsCurrent = 0;
	m_assetsNonCurrent = 0;
	m_assetTurnover = 0;
	m_bookVal = 0;
	m_businessAcqDisposals = 0;
	m_bvps = 0;
	m_capex = 0;
	m_cashAndEq = 0;
	m_consolidatedIncome = 0;
	m_costRev = 0;
	m_currentRatio = 0;
	m_debt = 0;
	m_debtCurrent = 0;
	m_debtEquity = 0;
	m_debtNonCurrent = 0;
	m_deferredRev = 0;
	m_depamor = 0;
	m_deposits = 0;
	m_ebit = 0;
	m_ebitda = 0;
	m_ebt = 0;
	m_enterpriseVal = 0;
	m_eps = 0;
	m_epsDil = 0;
	m_epsQoQ = 0;
	m_equity = 0;
	m_freeCashFlow = 0;
	m_fxRate = 0;
	m_grossMargin = 0;
	m_grossProfit = 0;
	m_intangibles = 0;
	m_intexp = 0;
	m_inventory = 0;
	m_investments = 0;
	m_investmentsAcqDisposals = 0;
	m_investmentsCurrent = 0;
	m_investmentsNonCurrent = 0;
	m_issrepayDebt = 0;
	m_issrepayEquity = 0;
	m_liabilitiesCurrent = 0;
	m_liabilitiesNonCurrent = 0;
	m_longTermDebtEquity = 0;
	m_marketCap = 0;
	m_ncf = 0;
	m_ncff = 0;
	m_ncfi = 0;
	m_ncfo = 0;
	m_ncfx = 0;
	m_netinc = 0;
	m_netIncComStock = 0;
	m_netIncDiscOps = 0;
	m_netMargin = 0;
	m_nonControllingInterests = 0;
	m_opex = 0;
	m_opinc = 0;
	m_opMargin = 0;
	m_payDiv = 0;
	m_pbRatio = 0;
	m_peRatio = 0;
	m_piotroskiFScore = 0;
	m_ppeq = 0;
	m_prefDVDs = 0;
	m_profitMargin = 0;
	m_retainedEarnings = 0;
	m_revenue = 0;
	m_revenueQoQ = 0;
	m_rnd = 0;
	m_roa = 0;
	m_roe = 0;
	m_rps = 0;
	m_sbcomp = 0;
	m_sga = 0;
	m_shareFactor = 0;
	m_sharesBasic = 0;
	m_shareswa = 0;
	m_shareswaDil = 0;
	m_taxAssets = 0;
	m_taxExp = 0;
	m_taxLiabilities = 0;
	m_totalAssets = 0;
	m_totalLiabilities = 0;
	m_trailingPEG1Y = 0;
}

void CTiingoCompanyFinancialState::Load(const CSetTiingoCompanyFinancialState& setTiingoFinancialState) {
	m_symbol = setTiingoFinancialState.m_symbol;
	m_exchange = setTiingoFinancialState.m_exchange;
	m_yearQuarter = setTiingoFinancialState.m_yearQuarter;
	m_accoci = atof(setTiingoFinancialState.m_accoci);
	m_acctPay = atof(setTiingoFinancialState.m_acctPay);
	m_acctRec = atof(setTiingoFinancialState.m_acctRec);
	m_assetsCurrent = atof(setTiingoFinancialState.m_assetsCurrent);
	m_assetsNonCurrent = atof(setTiingoFinancialState.m_assetsNonCurrent);
	m_assetTurnover = atof(setTiingoFinancialState.m_assetTurnover);
	m_bookVal = atof(setTiingoFinancialState.m_bookVal);
	m_businessAcqDisposals = atof(setTiingoFinancialState.m_businessAcqDisposals);
	m_bvps = atof(setTiingoFinancialState.m_bvps);
	m_capex = atof(setTiingoFinancialState.m_capex);
	m_cashAndEq = atof(setTiingoFinancialState.m_cashAndEq);
	m_consolidatedIncome = atof(setTiingoFinancialState.m_consolidatedIncome);
	m_costRev = atof(setTiingoFinancialState.m_costRev);
	m_currentRatio = atof(setTiingoFinancialState.m_currentRatio);
	m_debt = atof(setTiingoFinancialState.m_debt);
	m_debtCurrent = atof(setTiingoFinancialState.m_debtCurrent);
	m_debtEquity = atof(setTiingoFinancialState.m_debtEquity);
	m_debtNonCurrent = atof(setTiingoFinancialState.m_debtNonCurrent);
	m_deferredRev = atof(setTiingoFinancialState.m_deferredRev);
	m_depamor = atof(setTiingoFinancialState.m_depamor);
	m_deposits = atof(setTiingoFinancialState.m_deposits);
	m_ebit = atof(setTiingoFinancialState.m_ebit);
	m_ebitda = atof(setTiingoFinancialState.m_ebitda);
	m_ebt = atof(setTiingoFinancialState.m_ebt);
	m_enterpriseVal = atof(setTiingoFinancialState.m_enterpriseVal);
	m_eps = atof(setTiingoFinancialState.m_eps);
	m_epsDil = atof(setTiingoFinancialState.m_epsDil);
	m_epsQoQ = atof(setTiingoFinancialState.m_epsQoQ);
	m_equity = atof(setTiingoFinancialState.m_equity);
	m_freeCashFlow = atof(setTiingoFinancialState.m_freeCashFlow);
	m_fxRate = atof(setTiingoFinancialState.m_fxRate);
	m_grossMargin = atof(setTiingoFinancialState.m_grossMargin);
	m_grossProfit = atof(setTiingoFinancialState.m_grossProfit);
	m_intangibles = atof(setTiingoFinancialState.m_intangibles);
	m_intexp = atof(setTiingoFinancialState.m_intexp);
	m_inventory = atof(setTiingoFinancialState.m_inventory);
	m_investments = atof(setTiingoFinancialState.m_investments);
	m_investmentsAcqDisposals = atof(setTiingoFinancialState.m_investmentsAcqDisposals);
	m_investmentsCurrent = atof(setTiingoFinancialState.m_investmentsCurrent);
	m_investmentsNonCurrent = atof(setTiingoFinancialState.m_investmentsNonCurrent);
	m_issrepayDebt = atof(setTiingoFinancialState.m_issrepayDebt);
	m_issrepayEquity = atof(setTiingoFinancialState.m_issrepayEquity);
	m_liabilitiesCurrent = atof(setTiingoFinancialState.m_liabilitiesCurrent);
	m_liabilitiesNonCurrent = atof(setTiingoFinancialState.m_liabilitiesNonCurrent);
	m_longTermDebtEquity = atof(setTiingoFinancialState.m_longTermDebtEquity);
	m_marketCap = atof(setTiingoFinancialState.m_marketCap);
	m_ncf = atof(setTiingoFinancialState.m_ncf);
	m_ncff = atof(setTiingoFinancialState.m_ncff);
	m_ncfi = atof(setTiingoFinancialState.m_ncfi);
	m_ncfo = atof(setTiingoFinancialState.m_ncfo);
	m_ncfx = atof(setTiingoFinancialState.m_ncfx);
	m_netinc = atof(setTiingoFinancialState.m_netinc);
	m_netIncComStock = atof(setTiingoFinancialState.m_netIncComStock);
	m_netIncDiscOps = atof(setTiingoFinancialState.m_netIncDiscOps);
	m_netMargin = atof(setTiingoFinancialState.m_netMargin);
	m_nonControllingInterests = atof(setTiingoFinancialState.m_nonControllingInterests);
	m_opex = atof(setTiingoFinancialState.m_opex);
	m_opinc = atof(setTiingoFinancialState.m_opinc);
	m_opMargin = atof(setTiingoFinancialState.m_opMargin);
	m_payDiv = atof(setTiingoFinancialState.m_payDiv);
	m_pbRatio = atof(setTiingoFinancialState.m_pbRatio);
	m_peRatio = atof(setTiingoFinancialState.m_peRatio);
	m_piotroskiFScore = atof(setTiingoFinancialState.m_piotroskiFScore);
	m_ppeq = atof(setTiingoFinancialState.m_ppeq);
	m_prefDVDs = atof(setTiingoFinancialState.m_prefDVDs);
	m_profitMargin = atof(setTiingoFinancialState.m_profitMargin);
	m_retainedEarnings = atof(setTiingoFinancialState.m_retainedEarnings);
	m_revenue = atof(setTiingoFinancialState.m_revenue);
	m_revenueQoQ = atof(setTiingoFinancialState.m_revenueQoQ);
	m_rnd = atof(setTiingoFinancialState.m_rnd);
	m_roa = atof(setTiingoFinancialState.m_roa);
	m_roe = atof(setTiingoFinancialState.m_roe);
	m_rps = atof(setTiingoFinancialState.m_rps);
	m_sbcomp = atof(setTiingoFinancialState.m_sbcomp);
	m_sga = atof(setTiingoFinancialState.m_sga);
	m_shareFactor = atof(setTiingoFinancialState.m_shareFactor);
	m_sharesBasic = atof(setTiingoFinancialState.m_sharesBasic);
	m_shareswa = atof(setTiingoFinancialState.m_shareswa);
	m_shareswaDil = atof(setTiingoFinancialState.m_shareswaDil);
	m_taxAssets = atof(setTiingoFinancialState.m_taxAssets);
	m_taxExp = atof(setTiingoFinancialState.m_taxExp);
	m_taxLiabilities = atof(setTiingoFinancialState.m_taxLiabilities);
	m_totalAssets = atof(setTiingoFinancialState.m_totalAssets);
	m_totalLiabilities = atof(setTiingoFinancialState.m_totalLiabilities);
	m_trailingPEG1Y = atof(setTiingoFinancialState.m_trailingPEG1Y);
}

void CTiingoCompanyFinancialState::Append(CSetTiingoCompanyFinancialState& setTiingoFinancialState) const {
	setTiingoFinancialState.AddNew();
	Save(setTiingoFinancialState);
	setTiingoFinancialState.Update();
}

void CTiingoCompanyFinancialState::Save(CSetTiingoCompanyFinancialState& setTiingoFinancialState) const {
	setTiingoFinancialState.m_symbol = m_symbol.c_str();
	setTiingoFinancialState.m_exchange = m_exchange.c_str();
	setTiingoFinancialState.m_yearQuarter = m_yearQuarter;
	setTiingoFinancialState.m_accoci = ConvertValueToString(m_accoci);
	setTiingoFinancialState.m_acctPay = ConvertValueToString(m_acctPay);
	setTiingoFinancialState.m_acctRec = ConvertValueToString(m_acctRec);
	setTiingoFinancialState.m_assetsCurrent = ConvertValueToString(m_assetsCurrent);
	setTiingoFinancialState.m_assetsNonCurrent = ConvertValueToString(m_assetsNonCurrent);
	setTiingoFinancialState.m_assetTurnover = ConvertValueToString(m_assetTurnover);
	setTiingoFinancialState.m_bookVal = ConvertValueToString(m_bookVal);
	setTiingoFinancialState.m_businessAcqDisposals = ConvertValueToString(m_businessAcqDisposals);
	setTiingoFinancialState.m_bvps = ConvertValueToString(m_bvps);
	setTiingoFinancialState.m_capex = ConvertValueToString(m_capex);
	setTiingoFinancialState.m_cashAndEq = ConvertValueToString(m_cashAndEq);
	setTiingoFinancialState.m_consolidatedIncome = ConvertValueToString(m_consolidatedIncome);
	setTiingoFinancialState.m_costRev = ConvertValueToString(m_costRev);
	setTiingoFinancialState.m_currentRatio = ConvertValueToString(m_currentRatio);
	setTiingoFinancialState.m_debt = ConvertValueToString(m_debt);
	setTiingoFinancialState.m_debtCurrent = ConvertValueToString(m_debtCurrent);
	setTiingoFinancialState.m_debtEquity = ConvertValueToString(m_debtEquity);
	setTiingoFinancialState.m_debtNonCurrent = ConvertValueToString(m_debtNonCurrent);
	setTiingoFinancialState.m_deferredRev = ConvertValueToString(m_deferredRev);
	setTiingoFinancialState.m_depamor = ConvertValueToString(m_depamor);
	setTiingoFinancialState.m_deposits = ConvertValueToString(m_deposits);
	setTiingoFinancialState.m_ebit = ConvertValueToString(m_ebit);
	setTiingoFinancialState.m_ebitda = ConvertValueToString(m_ebitda);
	setTiingoFinancialState.m_ebt = ConvertValueToString(m_ebt);
	setTiingoFinancialState.m_enterpriseVal = ConvertValueToString(m_enterpriseVal);
	setTiingoFinancialState.m_eps = ConvertValueToString(m_eps);
	setTiingoFinancialState.m_epsDil = ConvertValueToString(m_epsDil);
	setTiingoFinancialState.m_epsQoQ = ConvertValueToString(m_epsQoQ);
	setTiingoFinancialState.m_equity = ConvertValueToString(m_equity);
	setTiingoFinancialState.m_freeCashFlow = ConvertValueToString(m_freeCashFlow);
	setTiingoFinancialState.m_fxRate = ConvertValueToString(m_fxRate);
	setTiingoFinancialState.m_grossMargin = ConvertValueToString(m_grossMargin);
	setTiingoFinancialState.m_grossProfit = ConvertValueToString(m_grossProfit);
	setTiingoFinancialState.m_intangibles = ConvertValueToString(m_intangibles);
	setTiingoFinancialState.m_intexp = ConvertValueToString(m_intexp);
	setTiingoFinancialState.m_inventory = ConvertValueToString(m_inventory);
	setTiingoFinancialState.m_investments = ConvertValueToString(m_investments);
	setTiingoFinancialState.m_investmentsAcqDisposals = ConvertValueToString(m_investmentsAcqDisposals);
	setTiingoFinancialState.m_investmentsCurrent = ConvertValueToString(m_investmentsCurrent);
	setTiingoFinancialState.m_investmentsNonCurrent = ConvertValueToString(m_investmentsNonCurrent);
	setTiingoFinancialState.m_issrepayDebt = ConvertValueToString(m_issrepayDebt);
	setTiingoFinancialState.m_issrepayEquity = ConvertValueToString(m_issrepayEquity);
	setTiingoFinancialState.m_liabilitiesCurrent = ConvertValueToString(m_liabilitiesCurrent);
	setTiingoFinancialState.m_liabilitiesNonCurrent = ConvertValueToString(m_liabilitiesNonCurrent);
	setTiingoFinancialState.m_longTermDebtEquity = ConvertValueToString(m_longTermDebtEquity);
	setTiingoFinancialState.m_marketCap = ConvertValueToString(m_marketCap);
	setTiingoFinancialState.m_ncf = ConvertValueToString(m_ncf);
	setTiingoFinancialState.m_ncff = ConvertValueToString(m_ncff);
	setTiingoFinancialState.m_ncfi = ConvertValueToString(m_ncfi);
	setTiingoFinancialState.m_ncfo = ConvertValueToString(m_ncfo);
	setTiingoFinancialState.m_ncfx = ConvertValueToString(m_ncfx);
	setTiingoFinancialState.m_netinc = ConvertValueToString(m_netinc);
	setTiingoFinancialState.m_netIncComStock = ConvertValueToString(m_netIncComStock);
	setTiingoFinancialState.m_netIncDiscOps = ConvertValueToString(m_netIncDiscOps);
	setTiingoFinancialState.m_netMargin = ConvertValueToString(m_netMargin);
	setTiingoFinancialState.m_nonControllingInterests = ConvertValueToString(m_nonControllingInterests);
	setTiingoFinancialState.m_opex = ConvertValueToString(m_opex);
	setTiingoFinancialState.m_opinc = ConvertValueToString(m_opinc);
	setTiingoFinancialState.m_opMargin = ConvertValueToString(m_opMargin);
	setTiingoFinancialState.m_payDiv = ConvertValueToString(m_payDiv);
	setTiingoFinancialState.m_pbRatio = ConvertValueToString(m_pbRatio);
	setTiingoFinancialState.m_peRatio = ConvertValueToString(m_peRatio);
	setTiingoFinancialState.m_piotroskiFScore = ConvertValueToString(m_piotroskiFScore);
	setTiingoFinancialState.m_ppeq = ConvertValueToString(m_ppeq);
	setTiingoFinancialState.m_prefDVDs = ConvertValueToString(m_prefDVDs);
	setTiingoFinancialState.m_profitMargin = ConvertValueToString(m_profitMargin);
	setTiingoFinancialState.m_retainedEarnings = ConvertValueToString(m_retainedEarnings);
	setTiingoFinancialState.m_revenue = ConvertValueToString(m_revenue);
	setTiingoFinancialState.m_revenueQoQ = ConvertValueToString(m_revenueQoQ);
	setTiingoFinancialState.m_rnd = ConvertValueToString(m_rnd);
	setTiingoFinancialState.m_roa = ConvertValueToString(m_roa);
	setTiingoFinancialState.m_roe = ConvertValueToString(m_roe);
	setTiingoFinancialState.m_rps = ConvertValueToString(m_rps);
	setTiingoFinancialState.m_sbcomp = ConvertValueToString(m_sbcomp);
	setTiingoFinancialState.m_sga = ConvertValueToString(m_sga);
	setTiingoFinancialState.m_shareFactor = ConvertValueToString(m_shareFactor);
	setTiingoFinancialState.m_sharesBasic = ConvertValueToString(m_sharesBasic);
	setTiingoFinancialState.m_shareswa = ConvertValueToString(m_shareswa);
	setTiingoFinancialState.m_shareswaDil = ConvertValueToString(m_shareswaDil);
	setTiingoFinancialState.m_taxAssets = ConvertValueToString(m_taxAssets);
	setTiingoFinancialState.m_taxExp = ConvertValueToString(m_taxExp);
	setTiingoFinancialState.m_taxLiabilities = ConvertValueToString(m_taxLiabilities);
	setTiingoFinancialState.m_totalAssets = ConvertValueToString(m_totalAssets);
	setTiingoFinancialState.m_totalLiabilities = ConvertValueToString(m_totalLiabilities);
	setTiingoFinancialState.m_trailingPEG1Y = ConvertValueToString(m_trailingPEG1Y);
}

void CTiingoCompanyFinancialState::Assign(int index, double fValue) {
	switch (index) {
	case 1:
		m_accoci = fValue;
		break;
	case 2:
		m_acctPay = fValue;
		break;
	case 3:
		m_acctRec = fValue;
		break;
	case 4:
		m_assetsCurrent = fValue;
		break;
	case 5:
		m_assetsNonCurrent = fValue;
		break;
	case 6: // 尚未使用
		m_assetTurnover = fValue;
		break;
	case 7:
		m_bookVal = fValue;
		break;
	case 8:
		m_businessAcqDisposals = fValue;
		break;
	case 9:
		m_bvps = fValue;
		break;
	case 10:
		m_capex = fValue;
		break;
	case 11:
		m_cashAndEq = fValue;
		break;
	case 12:
		m_consolidatedIncome = fValue;
		break;
	case 13:
		m_costRev = fValue;
		break;
	case 14:
		m_currentRatio = fValue;
		break;
	case 15:
		m_debt = fValue;
		break;
	case 16:
		m_debtCurrent = fValue;
		break;
	case 17:
		m_debtEquity = fValue;
		break;
	case 18:
		m_debtNonCurrent = fValue;
		break;
	case 19:
		m_deferredRev = fValue;
		break;
	case 20:
		m_depamor = fValue;
		break;
	case 21:
		m_deposits = fValue;
		break;
	case 22:
		m_ebit = fValue;
		break;
	case 23:
		m_ebitda = fValue;
		break;
	case 24:
		m_ebt = fValue;
		break;
	case 25: // 尚未使用
		m_enterpriseVal = fValue;
		break;
	case 26:
		m_eps = fValue;
		break;
	case 27:
		m_epsDil = fValue;
		break;
	case 28:
		m_epsQoQ = fValue;
		break;
	case 29:
		m_equity = fValue;
		break;
	case 30:
		m_freeCashFlow = fValue;
		break;
	case 31: // 尚未使用
		m_fxRate = fValue;
		break;
	case 32:
		m_grossMargin = fValue;
		break;
	case 33:
		m_grossProfit = fValue;
		break;
	case 34:
		m_intangibles = fValue;
		break;
	case 35:
		m_intexp = fValue;
		break;
	case 36:
		m_inventory = fValue;
		break;
	case 37:
		m_investments = fValue;
		break;
	case 38:
		m_investmentsAcqDisposals = fValue;
		break;
	case 39:
		m_investmentsCurrent = fValue;
		break;
	case 40:
		m_investmentsNonCurrent = fValue;
		break;
	case 41:
		m_issrepayDebt = fValue;
		break;
	case 42:
		m_issrepayEquity = fValue;
		break;
	case 43:
		m_liabilitiesCurrent = fValue;
		break;
	case 44:
		m_liabilitiesNonCurrent = fValue;
		break;
	case 45:
		m_longTermDebtEquity = fValue;
		break;
	case 46: // 尚未使用
		m_marketCap = fValue;
		break;
	case 47:
		m_ncf = fValue;
		break;
	case 48:
		m_ncff = fValue;
		break;
	case 49:
		m_ncfi = fValue;
		break;
	case 50:
		m_ncfo = fValue;
		break;
	case 51:
		m_ncfx = fValue;
		break;
	case 52:
		m_netinc = fValue;
		break;
	case 53:
		m_netIncComStock = fValue;
		break;
	case 54:
		m_netIncDiscOps = fValue;
		break;
	case 55: // 尚未使用
		m_netMargin = fValue;
		break;
	case 56:
		m_nonControllingInterests = fValue;
		break;
	case 57:
		m_opex = fValue;
		break;
	case 58:
		m_opinc = fValue;
		break;
	case 59: // 尚未使用
		m_opMargin = fValue;
		break;
	case 60:
		m_payDiv = fValue;
		break;
	case 61: // 尚未使用
		m_pbRatio = fValue;
		break;
	case 62: // 尚未使用
		m_peRatio = fValue;
		break;
	case 63:
		m_piotroskiFScore = fValue;
		break;
	case 64:
		m_ppeq = fValue;
		break;
	case 65:
		m_prefDVDs = fValue;
		break;
	case 66:
		m_profitMargin = fValue;
		break;
	case 67:
		m_retainedEarnings = fValue;
		break;
	case 68:
		m_revenue = fValue;
		break;
	case 69:
		m_revenueQoQ = fValue;
		break;
	case 70:
		m_rnd = fValue;
		break;
	case 71:
		m_roa = fValue;
		break;
	case 72:
		m_roe = fValue;
		break;
	case 73:
		m_rps = fValue;
		break;
	case 74:
		m_sbcomp = fValue;
		break;
	case 75:
		m_sga = fValue;
		break;
	case 76:
		m_shareFactor = fValue;
		break;
	case 77:
		m_sharesBasic = fValue;
		break;
	case 78:
		m_shareswa = fValue;
		break;
	case 79:
		m_shareswaDil = fValue;
		break;
	case 80:
		m_taxAssets = fValue;
		break;
	case 81:
		m_taxExp = fValue;
		break;
	case 82:
		m_taxLiabilities = fValue;
		break;
	case 83:
		m_totalAssets = fValue;
		break;
	case 84:
		m_totalLiabilities = fValue;
		break;
	case 85: // 尚未使用
		m_trailingPEG1Y = fValue;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}
