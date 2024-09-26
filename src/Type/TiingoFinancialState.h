#pragma once

#include"SetTiingoFinancialState.h"

#include<memory>
#include<vector>

class CTiingoFinancialState final {
public:
	CTiingoFinancialState();
	// 不允许复制和赋值。
	CTiingoFinancialState(const CTiingoFinancialState&) = delete;
	CTiingoFinancialState& operator=(const CTiingoFinancialState&) = delete;
	CTiingoFinancialState(const CTiingoFinancialState&&) noexcept = delete;
	CTiingoFinancialState& operator=(const CTiingoFinancialState&&) noexcept = delete;
	~CTiingoFinancialState() = default;

	void Reset();

	void Load(const CSetTiingoFinancialState& setTiingoFinancialState);
	void Append(CSetTiingoFinancialState& setTiingoFinancialState);
	void Save(CSetTiingoFinancialState& setTiingoFinancialState);

public:
	CString m_strSymbol;
	CString m_strExchange;
	long long m_accoci;
	long long m_acctPay;
	long long m_acctRec;
	long long m_assetCurrent;
	long long m_assetNotCurrent;
	long long assetTurnover;
	long long m_boolVal;
	long long m_businessAcqDisposals;
	long long m_bvps;
	long long m_capex;
	long long m_cashAndEq;
	long long m_consolidatedIncome;
	long long m_costRev;
	long long m_currentRatio;
	long long m_debt;
	long long m_debtCurrent;
	long long m_debtEquity;
	long long m_debtNonCurrent;
	long long m_deferredRev;
	long long m_depamor;
	long long m_deposits;
	long long m_ebit;
	long long m_ebitda;
	long long m_ebt;
	long long m_enterpriseVal;
	long long m_eps;
	long long m_epsDil;
	long long m_epsQoQ;
	long long m_equity;
	long long m_freeCashFlow;
	long long m_fxRate;
	long long m_grossMargin;
	long long m_grossProfit;
	long long m_intangibles;
	long long m_intexp;
	long long m_inventory;
	long long m_investments;
	long long m_investmentsAcqDisposals;
	long long m_investmentsCurrent;
	long long m_investmentsNonCurrent;
	long long m_issrepayDebt;
	long long m_issrepayEquity;
	long long m_liabilitiesCurrent;
	long long m_liabilitiesEquity;
	long long m_longTermDebtEquity;
	long long m_marketCap;
	long long m_ncf;
	long long m_ncff;
	long long m_ncfi;
	long long m_ncfo;
	long long m_ncfx;
	long long m_netinc;
	long long m_netincComStock;
	long long m_netIncDiscOps;
	long long m_netMargin;
	long long m_nonControllingInterests;
	long long m_opex;
	long long m_opinc;
	long long m_opMargin;
	long long m_payDiv;
	long long m_pbRatio;
	long long m_peRatio;
	long long m_piotroskiFScore;
	long long m_ppeq;
	long long m_prefDVDs;
	long long m_profitMargin;
	long long m_retainedEarnings;
	long long m_revenue;
	long long m_revenueQoQ;
	long long m_rnd;
	long long m_roa;
	long long m_roe;
	long long m_rps;
	long long m_sbcomp;
	long long m_sga;
	long long m_shareFactor;
	long long m_shareBasic;
	long long m_shareswa;
	long long m_sharewaDil;
	long long m_taxAssets;
	long long m_taxExp;
	long long m_taxLiabilities;
	long long m_totalAssets;
	long long m_totalLiabilities;
	long long m_railingPEG1Y;

	// 无需存储数据区
};

using CTiingoFinancialStatePtr = shared_ptr<CTiingoFinancialState>;
using CTiingoFinancialStatesPtr = shared_ptr<vector<CTiingoFinancialStatePtr>>;
