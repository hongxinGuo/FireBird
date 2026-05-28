#include"pch.h"

#include "dataBaseConnector.h"
#include"GeneralCheck.h"

#include"TiingoCompanyFinancialState.h"

namespace FireBirdTest {
	class CTiingoCompanyFinancialStateTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

		CTiingoCompanyFinancialState financialState;
	};

	TEST_F(CTiingoCompanyFinancialStateTest, TestInicialize) {
		EXPECT_EQ(financialState.m_symbol, "");
		EXPECT_EQ(financialState.m_exchange, "");
		EXPECT_DOUBLE_EQ(financialState.m_yearQuarter, 0);
		EXPECT_DOUBLE_EQ(financialState.m_accoci, 0);
		EXPECT_DOUBLE_EQ(financialState.m_acctPay, 0);
		EXPECT_DOUBLE_EQ(financialState.m_acctRec, 0);
		EXPECT_DOUBLE_EQ(financialState.m_assetsCurrent, 0);
		EXPECT_DOUBLE_EQ(financialState.m_assetsNonCurrent, 0);
		EXPECT_DOUBLE_EQ(financialState.m_assetTurnover, 0);
		EXPECT_DOUBLE_EQ(financialState.m_bookVal, 0);
		EXPECT_DOUBLE_EQ(financialState.m_businessAcqDisposals, 0);
		EXPECT_DOUBLE_EQ(financialState.m_bvps, 0);
		EXPECT_DOUBLE_EQ(financialState.m_capex, 0);
		EXPECT_DOUBLE_EQ(financialState.m_cashAndEq, 0);
		EXPECT_DOUBLE_EQ(financialState.m_consolidatedIncome, 0);
		EXPECT_DOUBLE_EQ(financialState.m_costRev, 0);
		EXPECT_DOUBLE_EQ(financialState.m_currentRatio, 0);
		EXPECT_DOUBLE_EQ(financialState.m_debt, 0);
		EXPECT_DOUBLE_EQ(financialState.m_debtCurrent, 0);
		EXPECT_DOUBLE_EQ(financialState.m_debtEquity, 0);
		EXPECT_DOUBLE_EQ(financialState.m_debtNonCurrent, 0);
		EXPECT_DOUBLE_EQ(financialState.m_deferredRev, 0);
		EXPECT_DOUBLE_EQ(financialState.m_depamor, 0);
		EXPECT_DOUBLE_EQ(financialState.m_deposits, 0);
		EXPECT_DOUBLE_EQ(financialState.m_ebit, 0);
		EXPECT_DOUBLE_EQ(financialState.m_ebitda, 0);
		EXPECT_DOUBLE_EQ(financialState.m_ebt, 0);
		EXPECT_DOUBLE_EQ(financialState.m_enterpriseVal, 0);
		EXPECT_DOUBLE_EQ(financialState.m_eps, 0);
		EXPECT_DOUBLE_EQ(financialState.m_epsDil, 0);
		EXPECT_DOUBLE_EQ(financialState.m_epsQoQ, 0);
		EXPECT_DOUBLE_EQ(financialState.m_equity, 0);
		EXPECT_DOUBLE_EQ(financialState.m_freeCashFlow, 0);
		EXPECT_DOUBLE_EQ(financialState.m_fxRate, 0);
		EXPECT_DOUBLE_EQ(financialState.m_grossMargin, 0);
		EXPECT_DOUBLE_EQ(financialState.m_grossProfit, 0);
		EXPECT_DOUBLE_EQ(financialState.m_intangibles, 0);
		EXPECT_DOUBLE_EQ(financialState.m_intexp, 0);
		EXPECT_DOUBLE_EQ(financialState.m_inventory, 0);
		EXPECT_DOUBLE_EQ(financialState.m_investments, 0);
		EXPECT_DOUBLE_EQ(financialState.m_investmentsAcqDisposals, 0);
		EXPECT_DOUBLE_EQ(financialState.m_investmentsCurrent, 0);
		EXPECT_DOUBLE_EQ(financialState.m_investmentsNonCurrent, 0);
		EXPECT_DOUBLE_EQ(financialState.m_issrepayDebt, 0);
		EXPECT_DOUBLE_EQ(financialState.m_issrepayEquity, 0);
		EXPECT_DOUBLE_EQ(financialState.m_liabilitiesCurrent, 0);
		EXPECT_DOUBLE_EQ(financialState.m_liabilitiesNonCurrent, 0);
		EXPECT_DOUBLE_EQ(financialState.m_longTermDebtEquity, 0);
		EXPECT_DOUBLE_EQ(financialState.m_marketCap, 0);
		EXPECT_DOUBLE_EQ(financialState.m_ncf, 0);
		EXPECT_DOUBLE_EQ(financialState.m_ncff, 0);
		EXPECT_DOUBLE_EQ(financialState.m_ncfi, 0);
		EXPECT_DOUBLE_EQ(financialState.m_ncfo, 0);
		EXPECT_DOUBLE_EQ(financialState.m_ncfx, 0);
		EXPECT_DOUBLE_EQ(financialState.m_netinc, 0);
		EXPECT_DOUBLE_EQ(financialState.m_netIncComStock, 0);
		EXPECT_DOUBLE_EQ(financialState.m_netIncDiscOps, 0);
		EXPECT_DOUBLE_EQ(financialState.m_netMargin, 0);
		EXPECT_DOUBLE_EQ(financialState.m_nonControllingInterests, 0);
		EXPECT_DOUBLE_EQ(financialState.m_opex, 0);
		EXPECT_DOUBLE_EQ(financialState.m_opinc, 0);
		EXPECT_DOUBLE_EQ(financialState.m_opMargin, 0);
		EXPECT_DOUBLE_EQ(financialState.m_payDiv, 0);
		EXPECT_DOUBLE_EQ(financialState.m_pbRatio, 0);
		EXPECT_DOUBLE_EQ(financialState.m_peRatio, 0);
		EXPECT_DOUBLE_EQ(financialState.m_piotroskiFScore, 0);
		EXPECT_DOUBLE_EQ(financialState.m_ppeq, 0);
		EXPECT_DOUBLE_EQ(financialState.m_prefDVDs, 0);
		EXPECT_DOUBLE_EQ(financialState.m_profitMargin, 0);
		EXPECT_DOUBLE_EQ(financialState.m_retainedEarnings, 0);
		EXPECT_DOUBLE_EQ(financialState.m_revenue, 0);
		EXPECT_DOUBLE_EQ(financialState.m_revenueQoQ, 0);
		EXPECT_DOUBLE_EQ(financialState.m_rnd, 0);
		EXPECT_DOUBLE_EQ(financialState.m_roa, 0);
		EXPECT_DOUBLE_EQ(financialState.m_roe, 0);
		EXPECT_DOUBLE_EQ(financialState.m_rps, 0);
		EXPECT_DOUBLE_EQ(financialState.m_sbcomp, 0);
		EXPECT_DOUBLE_EQ(financialState.m_sga, 0);
		EXPECT_DOUBLE_EQ(financialState.m_shareFactor, 0);
		EXPECT_DOUBLE_EQ(financialState.m_sharesBasic, 0);
		EXPECT_DOUBLE_EQ(financialState.m_shareswa, 0);
		EXPECT_DOUBLE_EQ(financialState.m_shareswaDil, 0);
		EXPECT_DOUBLE_EQ(financialState.m_taxAssets, 0);
		EXPECT_DOUBLE_EQ(financialState.m_taxExp, 0);
		EXPECT_DOUBLE_EQ(financialState.m_taxLiabilities, 0);
		EXPECT_DOUBLE_EQ(financialState.m_totalAssets, 0);
		EXPECT_DOUBLE_EQ(financialState.m_totalLiabilities, 0);
		EXPECT_DOUBLE_EQ(financialState.m_trailingPEG1Y, 0);
	}
}
