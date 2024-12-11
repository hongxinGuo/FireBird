#include"pch.h"
module;
#include<memory>
#include<vector>
export module FireBird.Type.TiingoCompanyFinancialState;

export import FireBird.Set.TiingoCompanyFinancialState;

export {
	class CTiingoCompanyFinancialState final {
	public:
		CTiingoCompanyFinancialState();
		// 不允许复制和赋值。
		CTiingoCompanyFinancialState(const CTiingoCompanyFinancialState&) = delete;
		CTiingoCompanyFinancialState& operator=(const CTiingoCompanyFinancialState&) = delete;
		CTiingoCompanyFinancialState(const CTiingoCompanyFinancialState&&) noexcept = delete;
		CTiingoCompanyFinancialState& operator=(const CTiingoCompanyFinancialState&&) noexcept = delete;
		~CTiingoCompanyFinancialState() = default;

		void Load(const CSetTiingoCompanyFinancialState& setTiingoFinancialState);
		void Append(CSetTiingoCompanyFinancialState& setTiingoFinancialState) const;
		void Save(CSetTiingoCompanyFinancialState& setTiingoFinancialState) const;

		void Assign(int index, double fValue);

	public:
		CString m_symbol{ _T("") };
		CString m_exchange{ _T("") };
		int m_yearQuarter{ 0 };
		double m_accoci{ 0 };
		double m_acctPay{ 0 };
		double m_acctRec{ 0 };
		double m_assetsCurrent{ 0 };
		double m_assetsNonCurrent{ 0 };
		double m_assetTurnover{ 0 };
		double m_bookVal{ 0 };
		double m_businessAcqDisposals{ 0 };
		double m_bvps{ 0 };
		double m_capex{ 0 };
		double m_cashAndEq{ 0 };
		double m_consolidatedIncome{ 0 };
		double m_costRev{ 0 };
		double m_currentRatio{ 0 };
		double m_debt{ 0 };
		double m_debtCurrent{ 0 };
		double m_debtEquity{ 0 };
		double m_debtNonCurrent{ 0 };
		double m_deferredRev{ 0 };
		double m_depamor{ 0 };
		double m_deposits{ 0 };
		double m_ebit{ 0 };
		double m_ebitda{ 0 };
		double m_ebt{ 0 };
		double m_enterpriseVal{ 0 };
		double m_eps{ 0 };
		double m_epsDil{ 0 };
		double m_epsQoQ{ 0 };
		double m_equity{ 0 };
		double m_freeCashFlow{ 0 };
		double m_fxRate{ 0 };
		double m_grossMargin{ 0 };
		double m_grossProfit{ 0 };
		double m_intangibles{ 0 };
		double m_intexp{ 0 };
		double m_inventory{ 0 };
		double m_investments{ 0 };
		double m_investmentsAcqDisposals{ 0 };
		double m_investmentsCurrent{ 0 };
		double m_investmentsNonCurrent{ 0 };
		double m_issrepayDebt{ 0 };
		double m_issrepayEquity{ 0 };
		double m_liabilitiesCurrent{ 0 };
		double m_liabilitiesNonCurrent{ 0 };
		double m_longTermDebtEquity{ 0 };
		double m_marketCap{ 0 };
		double m_ncf{ 0 };
		double m_ncff{ 0 };
		double m_ncfi{ 0 };
		double m_ncfo{ 0 };
		double m_ncfx{ 0 };
		double m_netinc{ 0 };
		double m_netIncComStock{ 0 };
		double m_netIncDiscOps{ 0 };
		double m_netMargin{ 0 };
		double m_nonControllingInterests{ 0 };
		double m_opex{ 0 };
		double m_opinc{ 0 };
		double m_opMargin{ 0 };
		double m_payDiv{ 0 };
		double m_pbRatio{ 0 };
		double m_peRatio{ 0 };
		double m_piotroskiFScore{ 0 };
		double m_ppeq{ 0 };
		double m_prefDVDs{ 0 };
		double m_profitMargin{ 0 };
		double m_retainedEarnings{ 0 };
		double m_revenue{ 0 };
		double m_revenueQoQ{ 0 };
		double m_rnd{ 0 };
		double m_roa{ 0 };
		double m_roe{ 0 };
		double m_rps{ 0 };
		double m_sbcomp{ 0 };
		double m_sga{ 0 };
		double m_shareFactor{ 0 };
		double m_sharesBasic{ 0 };
		double m_shareswa{ 0 };
		double m_shareswaDil{ 0 };
		double m_taxAssets{ 0 };
		double m_taxExp{ 0 };
		double m_taxLiabilities{ 0 };
		double m_totalAssets{ 0 };
		double m_totalLiabilities{ 0 };
		double m_trailingPEG1Y{ 0 };

		// 无需存储数据区
	};

	using CTiingoCompanyFinancialStatePtr = shared_ptr<CTiingoCompanyFinancialState>;
	using CTiingoCompanyFinancialStatesPtr = shared_ptr<vector<CTiingoCompanyFinancialStatePtr>>;
}