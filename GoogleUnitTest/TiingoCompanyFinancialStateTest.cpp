#include"pch.h"

////#include"gtest/gtest.h"

import FireBird.System.Configuration;

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
		EXPECT_STREQ(financialState.m_symbol, _T(""));
		EXPECT_STREQ(financialState.m_exchange, _T(""));
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

	TEST_F(CTiingoCompanyFinancialStateTest, TestAppend) {
		CSetTiingoCompanyFinancialState setNaicsIndustry, setNaicsIndustry2;
		CTiingoCompanyFinancialState financialStateToBeSaved;

		financialStateToBeSaved.m_symbol = _T("AAAAA");
		financialStateToBeSaved.m_exchange = _T("US");
		financialStateToBeSaved.m_yearQuarter = 202401;
		financialStateToBeSaved.m_accoci = 1;
		financialStateToBeSaved.m_acctPay = 2;
		financialStateToBeSaved.m_acctRec = 3;
		financialStateToBeSaved.m_assetsCurrent = 4;
		financialStateToBeSaved.m_assetsNonCurrent = 5;
		financialStateToBeSaved.m_assetTurnover = 6;
		financialStateToBeSaved.m_bookVal = 7;
		financialStateToBeSaved.m_businessAcqDisposals = 8;
		financialStateToBeSaved.m_bvps = 9;
		financialStateToBeSaved.m_capex = 10;
		financialStateToBeSaved.m_cashAndEq = 11;
		financialStateToBeSaved.m_consolidatedIncome = 12;
		financialStateToBeSaved.m_costRev = 13;
		financialStateToBeSaved.m_currentRatio = 14;
		financialStateToBeSaved.m_debt = 15;
		financialStateToBeSaved.m_debtCurrent = 16;
		financialStateToBeSaved.m_debtEquity = 85;
		financialStateToBeSaved.m_debtNonCurrent = 17;
		financialStateToBeSaved.m_deferredRev = 18;
		financialStateToBeSaved.m_depamor = 19;
		financialStateToBeSaved.m_deposits = 20;
		financialStateToBeSaved.m_ebit = 21;
		financialStateToBeSaved.m_ebitda = 22;
		financialStateToBeSaved.m_ebt = 23;
		financialStateToBeSaved.m_enterpriseVal = 24;
		financialStateToBeSaved.m_eps = 25;
		financialStateToBeSaved.m_epsDil = 26;
		financialStateToBeSaved.m_epsQoQ = 27;
		financialStateToBeSaved.m_equity = 28;
		financialStateToBeSaved.m_freeCashFlow = 29;
		financialStateToBeSaved.m_fxRate = 30;
		financialStateToBeSaved.m_grossMargin = 31;
		financialStateToBeSaved.m_grossProfit = 32;
		financialStateToBeSaved.m_intangibles = 33;
		financialStateToBeSaved.m_intexp = 34;
		financialStateToBeSaved.m_inventory = 35;
		financialStateToBeSaved.m_investments = 36;
		financialStateToBeSaved.m_investmentsAcqDisposals = 37;
		financialStateToBeSaved.m_investmentsCurrent = 38;
		financialStateToBeSaved.m_investmentsNonCurrent = 39;
		financialStateToBeSaved.m_issrepayDebt = 40;
		financialStateToBeSaved.m_issrepayEquity = 41;
		financialStateToBeSaved.m_liabilitiesCurrent = 42;
		financialStateToBeSaved.m_liabilitiesNonCurrent = 43;
		financialStateToBeSaved.m_longTermDebtEquity = 44;
		financialStateToBeSaved.m_marketCap = 45;
		financialStateToBeSaved.m_ncf = 46;
		financialStateToBeSaved.m_ncff = 47;
		financialStateToBeSaved.m_ncfi = 48;
		financialStateToBeSaved.m_ncfo = 49;
		financialStateToBeSaved.m_ncfx = 50;
		financialStateToBeSaved.m_netinc = 51;
		financialStateToBeSaved.m_netIncComStock = 52;
		financialStateToBeSaved.m_netIncDiscOps = 53;
		financialStateToBeSaved.m_netMargin = 54;
		financialStateToBeSaved.m_nonControllingInterests = 55;
		financialStateToBeSaved.m_opex = 56;
		financialStateToBeSaved.m_opinc = 57;
		financialStateToBeSaved.m_opMargin = 58;
		financialStateToBeSaved.m_payDiv = 59;
		financialStateToBeSaved.m_pbRatio = 60;
		financialStateToBeSaved.m_peRatio = 61;
		financialStateToBeSaved.m_piotroskiFScore = 62;
		financialStateToBeSaved.m_ppeq = 63;
		financialStateToBeSaved.m_prefDVDs = 64;
		financialStateToBeSaved.m_profitMargin = 65;
		financialStateToBeSaved.m_retainedEarnings = 66;
		financialStateToBeSaved.m_revenue = 67;
		financialStateToBeSaved.m_revenueQoQ = 68;
		financialStateToBeSaved.m_rnd = 69;
		financialStateToBeSaved.m_roa = 70;
		financialStateToBeSaved.m_roe = 71;
		financialStateToBeSaved.m_rps = 72;
		financialStateToBeSaved.m_sbcomp = 73;
		financialStateToBeSaved.m_sga = 74;
		financialStateToBeSaved.m_shareFactor = 75;
		financialStateToBeSaved.m_sharesBasic = 76;
		financialStateToBeSaved.m_shareswa = 77;
		financialStateToBeSaved.m_shareswaDil = 78;
		financialStateToBeSaved.m_taxAssets = 79;
		financialStateToBeSaved.m_taxExp = 80;
		financialStateToBeSaved.m_taxLiabilities = 81;
		financialStateToBeSaved.m_totalAssets = 82;
		financialStateToBeSaved.m_totalLiabilities = 83;
		financialStateToBeSaved.m_trailingPEG1Y = 84;

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setNaicsIndustry.Open();
		setNaicsIndustry.m_pDatabase->BeginTrans();
		financialStateToBeSaved.Append(setNaicsIndustry);
		setNaicsIndustry.m_pDatabase->CommitTrans();
		setNaicsIndustry.Close();

		setNaicsIndustry2.m_strFilter = _T("[Symbol] = 'AAAAA'");
		setNaicsIndustry2.Open();
		setNaicsIndustry2.m_pDatabase->BeginTrans();
		EXPECT_TRUE(!setNaicsIndustry2.IsEOF()) << "此时已经存入了AAAAA";
		financialState.Load(setNaicsIndustry2);
		setNaicsIndustry2.Delete();
		setNaicsIndustry2.m_pDatabase->CommitTrans();
		setNaicsIndustry2.Close();

		EXPECT_STREQ(financialState.m_symbol, _T("AAAAA"));
		EXPECT_STREQ(financialState.m_exchange, _T("US"));
		EXPECT_DOUBLE_EQ(financialState.m_yearQuarter, 202401);
		EXPECT_DOUBLE_EQ(financialState.m_accoci, 1);
		EXPECT_DOUBLE_EQ(financialState.m_acctPay, 2);
		EXPECT_DOUBLE_EQ(financialState.m_acctRec, 3);
		EXPECT_DOUBLE_EQ(financialState.m_assetsCurrent, 4);
		EXPECT_DOUBLE_EQ(financialState.m_assetsNonCurrent, 5);
		EXPECT_DOUBLE_EQ(financialState.m_assetTurnover, 6);
		EXPECT_DOUBLE_EQ(financialState.m_bookVal, 7);
		EXPECT_DOUBLE_EQ(financialState.m_businessAcqDisposals, 8);
		EXPECT_DOUBLE_EQ(financialState.m_bvps, 9);
		EXPECT_DOUBLE_EQ(financialState.m_capex, 10);
		EXPECT_DOUBLE_EQ(financialState.m_cashAndEq, 11);
		EXPECT_DOUBLE_EQ(financialState.m_consolidatedIncome, 12);
		EXPECT_DOUBLE_EQ(financialState.m_costRev, 13);
		EXPECT_DOUBLE_EQ(financialState.m_currentRatio, 14);
		EXPECT_DOUBLE_EQ(financialState.m_debt, 15);
		EXPECT_DOUBLE_EQ(financialState.m_debtCurrent, 16);
		EXPECT_DOUBLE_EQ(financialState.m_debtEquity, 85);
		EXPECT_DOUBLE_EQ(financialState.m_debtNonCurrent, 17);
		EXPECT_DOUBLE_EQ(financialState.m_deferredRev, 18);
		EXPECT_DOUBLE_EQ(financialState.m_depamor, 19);
		EXPECT_DOUBLE_EQ(financialState.m_deposits, 20);
		EXPECT_DOUBLE_EQ(financialState.m_ebit, 21);
		EXPECT_DOUBLE_EQ(financialState.m_ebitda, 22);
		EXPECT_DOUBLE_EQ(financialState.m_ebt, 23);
		EXPECT_DOUBLE_EQ(financialState.m_enterpriseVal, 24);
		EXPECT_DOUBLE_EQ(financialState.m_eps, 25);
		EXPECT_DOUBLE_EQ(financialState.m_epsDil, 26);
		EXPECT_DOUBLE_EQ(financialState.m_epsQoQ, 27);
		EXPECT_DOUBLE_EQ(financialState.m_equity, 28);
		EXPECT_DOUBLE_EQ(financialState.m_freeCashFlow, 29);
		EXPECT_DOUBLE_EQ(financialState.m_fxRate, 30);
		EXPECT_DOUBLE_EQ(financialState.m_grossMargin, 31);
		EXPECT_DOUBLE_EQ(financialState.m_grossProfit, 32);
		EXPECT_DOUBLE_EQ(financialState.m_intangibles, 33);
		EXPECT_DOUBLE_EQ(financialState.m_intexp, 34);
		EXPECT_DOUBLE_EQ(financialState.m_inventory, 35);
		EXPECT_DOUBLE_EQ(financialState.m_investments, 36);
		EXPECT_DOUBLE_EQ(financialState.m_investmentsAcqDisposals, 37);
		EXPECT_DOUBLE_EQ(financialState.m_investmentsCurrent, 38);
		EXPECT_DOUBLE_EQ(financialState.m_investmentsNonCurrent, 39);
		EXPECT_DOUBLE_EQ(financialState.m_issrepayDebt, 40);
		EXPECT_DOUBLE_EQ(financialState.m_issrepayEquity, 41);
		EXPECT_DOUBLE_EQ(financialState.m_liabilitiesCurrent, 42);
		EXPECT_DOUBLE_EQ(financialState.m_liabilitiesNonCurrent, 43);
		EXPECT_DOUBLE_EQ(financialState.m_longTermDebtEquity, 44);
		EXPECT_DOUBLE_EQ(financialState.m_marketCap, 45);
		EXPECT_DOUBLE_EQ(financialState.m_ncf, 46);
		EXPECT_DOUBLE_EQ(financialState.m_ncff, 47);
		EXPECT_DOUBLE_EQ(financialState.m_ncfi, 48);
		EXPECT_DOUBLE_EQ(financialState.m_ncfo, 49);
		EXPECT_DOUBLE_EQ(financialState.m_ncfx, 50);
		EXPECT_DOUBLE_EQ(financialState.m_netinc, 51);
		EXPECT_DOUBLE_EQ(financialState.m_netIncComStock, 52);
		EXPECT_DOUBLE_EQ(financialState.m_netIncDiscOps, 53);
		EXPECT_DOUBLE_EQ(financialState.m_netMargin, 54);
		EXPECT_DOUBLE_EQ(financialState.m_nonControllingInterests, 55);
		EXPECT_DOUBLE_EQ(financialState.m_opex, 56);
		EXPECT_DOUBLE_EQ(financialState.m_opinc, 57);
		EXPECT_DOUBLE_EQ(financialState.m_opMargin, 58);
		EXPECT_DOUBLE_EQ(financialState.m_payDiv, 59);
		EXPECT_DOUBLE_EQ(financialState.m_pbRatio, 60);
		EXPECT_DOUBLE_EQ(financialState.m_peRatio, 61);
		EXPECT_DOUBLE_EQ(financialState.m_piotroskiFScore, 62);
		EXPECT_DOUBLE_EQ(financialState.m_ppeq, 63);
		EXPECT_DOUBLE_EQ(financialState.m_prefDVDs, 64);
		EXPECT_DOUBLE_EQ(financialState.m_profitMargin, 65);
		EXPECT_DOUBLE_EQ(financialState.m_retainedEarnings, 66);
		EXPECT_DOUBLE_EQ(financialState.m_revenue, 67);
		EXPECT_DOUBLE_EQ(financialState.m_revenueQoQ, 68);
		EXPECT_DOUBLE_EQ(financialState.m_rnd, 69);
		EXPECT_DOUBLE_EQ(financialState.m_roa, 70);
		EXPECT_DOUBLE_EQ(financialState.m_roe, 71);
		EXPECT_DOUBLE_EQ(financialState.m_rps, 72);
		EXPECT_DOUBLE_EQ(financialState.m_sbcomp, 73);
		EXPECT_DOUBLE_EQ(financialState.m_sga, 74);
		EXPECT_DOUBLE_EQ(financialState.m_shareFactor, 75);
		EXPECT_DOUBLE_EQ(financialState.m_sharesBasic, 76);
		EXPECT_DOUBLE_EQ(financialState.m_shareswa, 77);
		EXPECT_DOUBLE_EQ(financialState.m_shareswaDil, 78);
		EXPECT_DOUBLE_EQ(financialState.m_taxAssets, 79);
		EXPECT_DOUBLE_EQ(financialState.m_taxExp, 80);
		EXPECT_DOUBLE_EQ(financialState.m_taxLiabilities, 81);
		EXPECT_DOUBLE_EQ(financialState.m_totalAssets, 82);
		EXPECT_DOUBLE_EQ(financialState.m_totalLiabilities, 83);
		EXPECT_DOUBLE_EQ(financialState.m_trailingPEG1Y, 84);
	}
}
