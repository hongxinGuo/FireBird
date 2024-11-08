// CSetTiingoCompanyFinancialState.cpp : CSetTiingoCompanyFinancialState 类的实现

#include"pch.h"

#include "SetTiingoCompanyFinancialState.h"

CSetTiingoCompanyFinancialState::CSetTiingoCompanyFinancialState(const CString& strSchema, const CString& strTable, CDatabase* pdb)
	: CVirtualRecordset(strSchema, strTable, pdb) {
	m_nFields = 89;
}

void CSetTiingoCompanyFinancialState::DoFieldExchange(CFieldExchange* pFX) {
	pFX->SetFieldType(CFieldExchange::outputColumn);
	// RFX_Text() 和 RFX_Text() 这类宏依赖的是
	// 成员变量的类型，而不是数据库字段的类型。
	// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ID]"), m_ID);
	RFX_Text(pFX, _T("Symbol"), m_symbol);
	RFX_Text(pFX, _T("exchange"), m_exchange);
	RFX_Int(pFX, _T("yearQuarter"), m_yearQuarter);
	RFX_Text(pFX, _T("accoci"), m_accoci);
	RFX_Text(pFX, _T("acctPay"), m_acctPay);
	RFX_Text(pFX, _T("acctRec"), m_acctRec);
	RFX_Text(pFX, _T("assetsCurrent"), m_assetsCurrent);
	RFX_Text(pFX, _T("assetsNonCurrent"), m_assetsNonCurrent);
	RFX_Text(pFX, _T("assetTurnover"), m_assetTurnover);
	RFX_Text(pFX, _T("bookVal"), m_bookVal);
	RFX_Text(pFX, _T("businessAcqDisposals"), m_businessAcqDisposals);
	RFX_Text(pFX, _T("bvps"), m_bvps);
	RFX_Text(pFX, _T("capex"), m_capex);
	RFX_Text(pFX, _T("cashAndEq"), m_cashAndEq);
	RFX_Text(pFX, _T("consolidatedIncome"), m_consolidatedIncome);
	RFX_Text(pFX, _T("costRev"), m_costRev);
	RFX_Text(pFX, _T("currentRatio"), m_currentRatio);
	RFX_Text(pFX, _T("debt"), m_debt);
	RFX_Text(pFX, _T("debtCurrent"), m_debtCurrent);
	RFX_Text(pFX, _T("debtEquity"), m_debtEquity);
	RFX_Text(pFX, _T("debtNonCurrent"), m_debtNonCurrent);
	RFX_Text(pFX, _T("deferredRev"), m_deferredRev);
	RFX_Text(pFX, _T("depamor"), m_depamor);
	RFX_Text(pFX, _T("deposits"), m_deposits);
	RFX_Text(pFX, _T("ebit"), m_ebit);
	RFX_Text(pFX, _T("ebitda"), m_ebitda);
	RFX_Text(pFX, _T("ebt"), m_ebt);
	RFX_Text(pFX, _T("enterpriseVal"), m_enterpriseVal);
	RFX_Text(pFX, _T("eps"), m_eps);
	RFX_Text(pFX, _T("epsDil"), m_epsDil);
	RFX_Text(pFX, _T("epsQoQ"), m_epsQoQ);
	RFX_Text(pFX, _T("equity"), m_equity);
	RFX_Text(pFX, _T("freeCashFlow"), m_freeCashFlow);
	RFX_Text(pFX, _T("fxRate"), m_fxRate);
	RFX_Text(pFX, _T("grossMargin"), m_grossMargin);
	RFX_Text(pFX, _T("grossProfit"), m_grossProfit);
	RFX_Text(pFX, _T("intangibles"), m_intangibles);
	RFX_Text(pFX, _T("intexp"), m_intexp);
	RFX_Text(pFX, _T("inventory"), m_inventory);
	RFX_Text(pFX, _T("investments"), m_investments);
	RFX_Text(pFX, _T("investmentsAcqDisposals"), m_investmentsAcqDisposals);
	RFX_Text(pFX, _T("investmentsCurrent"), m_investmentsCurrent);
	RFX_Text(pFX, _T("investmentsNonCurrent"), m_investmentsNonCurrent);
	RFX_Text(pFX, _T("issrepayDebt"), m_issrepayDebt);
	RFX_Text(pFX, _T("issrepayEquity"), m_issrepayEquity);
	RFX_Text(pFX, _T("liabilitiesCurrent"), m_liabilitiesCurrent);
	RFX_Text(pFX, _T("liabilitiesNonCurrent"), m_liabilitiesNonCurrent);
	RFX_Text(pFX, _T("longTermDebtEquity"), m_longTermDebtEquity);
	RFX_Text(pFX, _T("marketCap"), m_marketCap);
	RFX_Text(pFX, _T("ncf"), m_ncf);
	RFX_Text(pFX, _T("ncff"), m_ncff);
	RFX_Text(pFX, _T("ncfi"), m_ncfi);
	RFX_Text(pFX, _T("ncfo"), m_ncfo);
	RFX_Text(pFX, _T("ncfx"), m_ncfx);
	RFX_Text(pFX, _T("netinc"), m_netinc);
	RFX_Text(pFX, _T("netIncComStock"), m_netIncComStock);
	RFX_Text(pFX, _T("netIncDiscOps"), m_netIncDiscOps);
	RFX_Text(pFX, _T("netMargin"), m_netMargin);
	RFX_Text(pFX, _T("nonControllingInterests"), m_nonControllingInterests);
	RFX_Text(pFX, _T("opex"), m_opex);
	RFX_Text(pFX, _T("opinc"), m_opinc);
	RFX_Text(pFX, _T("opMargin"), m_opMargin);
	RFX_Text(pFX, _T("payDiv"), m_payDiv);
	RFX_Text(pFX, _T("pbRatio"), m_pbRatio);
	RFX_Text(pFX, _T("peRatio"), m_peRatio);
	RFX_Text(pFX, _T("piotroskiFScore"), m_piotroskiFScore);
	RFX_Text(pFX, _T("ppeq"), m_ppeq);
	RFX_Text(pFX, _T("prefDVDs"), m_prefDVDs);
	RFX_Text(pFX, _T("profitMargin"), m_profitMargin);
	RFX_Text(pFX, _T("retainedEarnings"), m_retainedEarnings);
	RFX_Text(pFX, _T("revenue"), m_revenue);
	RFX_Text(pFX, _T("revenueQoQ"), m_revenueQoQ);
	RFX_Text(pFX, _T("rnd"), m_rnd);
	RFX_Text(pFX, _T("roa"), m_roa);
	RFX_Text(pFX, _T("roe"), m_roe);
	RFX_Text(pFX, _T("rps"), m_rps);
	RFX_Text(pFX, _T("sbcomp"), m_sbcomp);
	RFX_Text(pFX, _T("sga"), m_sga);
	RFX_Text(pFX, _T("shareFactor"), m_shareFactor);
	RFX_Text(pFX, _T("sharesBasic"), m_sharesBasic);
	RFX_Text(pFX, _T("shareswa"), m_shareswa);
	RFX_Text(pFX, _T("shareswaDil"), m_shareswaDil);
	RFX_Text(pFX, _T("taxAssets"), m_taxAssets);
	RFX_Text(pFX, _T("taxExp"), m_taxExp);
	RFX_Text(pFX, _T("taxLiabilities"), m_taxLiabilities);
	RFX_Text(pFX, _T("totalAssets"), m_totalAssets);
	RFX_Text(pFX, _T("totalLiabilities"), m_totalLiabilities);
	RFX_Text(pFX, _T("trailingPEG1Y"), m_trailingPEG1Y);
}
