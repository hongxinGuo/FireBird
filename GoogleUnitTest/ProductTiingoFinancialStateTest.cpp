#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubStock.h"
#include"WorldMarket.h"

#include"ProductTiingoFinancialState.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoFinancialStateTest : public Test {
	protected:
		static void SetUpTestSuite() {
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

	protected:
		CProductTiingoFinancialState financialState;
	};

	TEST_F(CProductTiingoFinancialStateTest, TestInitialize) {
		EXPECT_EQ(financialState.GetIndex(), 0);
		EXPECT_STREQ(financialState.GetInquiryFunction(), _T("https://api.tiingo.com/tiingo/fundamentals"));
	}

	TEST_F(CProductTiingoFinancialStateTest, TestCreatMessage) {
		financialState.SetIndex(1); // stock code is AA
		EXPECT_STREQ(financialState.CreateMessage(), _T("https://api.tiingo.com/tiingo/fundamentals/AA/statements?"));
	}

	TEST_F(CProductTiingoFinancialStateTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对，缺乏'{'
	Test_TiingoWebData tiingoFinancialState1(
		1, _T(""),
		_T(
			"[{\"date\":\"2024-06-29\",\"year\":2024,\"quarter\":3,\"statementData\":{\"cashFlow\":[{\"dataCode\":\"freeCashFlow\",\"value\":26707000000.0},{\"dataCode\":\"ncff\",\"value\":-36017000000.0},{\"dataCode\":\"sbcomp\",\"value\":2869000000.0},{\"dataCode\":\"ncfi\",\"value\":-127000000.0},{\"dataCode\":\"investmentsAcqDisposals\",\"value\":2412000000.0},{\"dataCode\":\"ncfo\",\"value\":28858000000.0},{\"dataCode\":\"payDiv\",\"value\":-3895000000.0},{\"dataCode\":\"businessAcqDisposals\",\"value\":0.0},{\"dataCode\":\"issrepayEquity\",\"value\":-26522000000.0},{\"dataCode\":\"ncfx\",\"value\":0.0},{\"dataCode\":\"capex\",\"value\":-2151000000.0},{\"dataCode\":\"depamor\",\"value\":2850000000.0},{\"dataCode\":\"issrepayDebt\",\"value\":-3253000000.0},{\"dataCode\":\"ncf\",\"value\":-7286000000.0}],\"incomeStatement\":[{\"dataCode\":\"shareswaDil\",\"value\":15348175000.0},{\"dataCode\":\"intexp\",\"value\":0.0},{\"dataCode\":\"netinc\",\"value\":21448000000.0},{\"dataCode\":\"netIncDiscOps\",\"value\":0.0},{\"dataCode\":\"ebitda\",\"value\":28344000000.0},{\"dataCode\":\"ebt\",\"value\":25494000000.0},{\"dataCode\":\"eps\",\"value\":1.4},{\"dataCode\":\"opinc\",\"value\":25352000000.0},{\"dataCode\":\"shareswa\",\"value\":15287521000.0},{\"dataCode\":\"netIncComStock\",\"value\":21448000000.0},{\"dataCode\":\"consolidatedIncome\",\"value\":21448000000.0},{\"dataCode\":\"taxExp\",\"value\":4046000000.0},{\"dataCode\":\"prefDVDs\",\"value\":0.0},{\"dataCode\":\"sga\",\"value\":6320000000.0},{\"dataCode\":\"epsDil\",\"value\":1.4},{\"dataCode\":\"rnd\",\"value\":8006000000.0},{\"dataCode\":\"nonControllingInterests\",\"value\":0.0},{\"dataCode\":\"revenue\",\"value\":85777000000.0},{\"dataCode\":\"opex\",\"value\":14326000000.0},{\"dataCode\":\"costRev\",\"value\":46099000000.0},{\"dataCode\":\"ebit\",\"value\":25494000000.0},{\"dataCode\":\"grossProfit\",\"value\":39678000000.0}],\"balanceSheet\":[{\"dataCode\":\"debtNonCurrent\",\"value\":86196000000.0},{\"dataCode\":\"liabilitiesCurrent\",\"value\":131624000000.0},{\"dataCode\":\"totalLiabilities\",\"value\":264904000000.0},{\"dataCode\":\"debtCurrent\",\"value\":15108000000.0},{\"dataCode\":\"ppeq\",\"value\":44502000000.0},{\"dataCode\":\"taxLiabilities\",\"value\":0.0},{\"dataCode\":\"investmentsNonCurrent\",\"value\":91240000000.0},{\"dataCode\":\"accoci\",\"value\":-8416000000.0},{\"dataCode\":\"acctRec\",\"value\":43172000000.0},{\"dataCode\":\"totalAssets\",\"value\":331612000000.0},{\"dataCode\":\"assetsNonCurrent\",\"value\":206177000000.0},{\"dataCode\":\"intangibles\",\"value\":0.0},{\"dataCode\":\"cashAndEq\",\"value\":25565000000.0},{\"dataCode\":\"assetsCurrent\",\"value\":125435000000.0},{\"dataCode\":\"deferredRev\",\"value\":8053000000.0},{\"dataCode\":\"investmentsCurrent\",\"value\":36236000000.0},{\"dataCode\":\"retainedEarnings\",\"value\":-4726000000.0},{\"dataCode\":\"taxAssets\",\"value\":0.0},{\"dataCode\":\"deposits\",\"value\":0.0},{\"dataCode\":\"debt\",\"value\":101304000000.0},{\"dataCode\":\"inventory\",\"value\":6165000000.0},{\"dataCode\":\"investments\",\"value\":127476000000.0},{\"dataCode\":\"liabilitiesNonCurrent\",\"value\":133280000000.0},{\"dataCode\":\"sharesBasic\",\"value\":15334082000.0},{\"dataCode\":\"equity\",\"value\":66708000000.0},{\"dataCode\":\"acctPay\",\"value\":47574000000.0}],\"overview\":[{\"dataCode\":\"bvps\",\"value\":4.35030933054877},{\"dataCode\":\"debtEquity\",\"value\":1.51861845655693},{\"dataCode\":\"bookVal\",\"value\":66708000000.0},{\"dataCode\":\"piotroskiFScore\",\"value\":6.0},{\"dataCode\":\"shareFactor\",\"value\":1.0},{\"dataCode\":\"rps\",\"value\":5.59387904668829},{\"dataCode\":\"epsQoQ\",\"value\":0.102362204724409},{\"dataCode\":\"grossMargin\",\"value\":0.462571551814589},{\"dataCode\":\"roe\",\"value\":1.47150259067358},{\"dataCode\":\"profitMargin\",\"value\":0.462571551814589},{\"dataCode\":\"revenueQoQ\",\"value\":0.0486570412117804},{\"dataCode\":\"roa\",\"value\":0.296573389958695},{\"dataCode\":\"longTermDebtEquity\",\"value\":1.29213887389818},{\"dataCode\":\"currentRatio\",\"value\":0.952979699750805}]}}]"));
	// 第一个数据缺项
	Test_TiingoWebData tiingoFinancialState2(
		2, _T(""),
		_T(
			"[{\"date\":\"2024-06-29\",\"year\":2024,\"quarter\":3,\"statementData\":{\"cashFlow\":[{\"dataCode\":\"freeCashFlow\",\"value\":26707000000.0},{\"dataCode\":\"ncff\",\"value\":-36017000000.0},{\"dataCode\":\"sbcomp\",\"value\":2869000000.0},{\"dataCode\":\"ncfi\",\"value\":-127000000.0},{\"dataCode\":\"investmentsAcqDisposals\",\"value\":2412000000.0},{\"dataCode\":\"ncfo\",\"value\":28858000000.0},{\"dataCode\":\"payDiv\",\"value\":-3895000000.0},{\"dataCode\":\"businessAcqDisposals\",\"value\":0.0},{\"dataCode\":\"issrepayEquity\",\"value\":-26522000000.0},{\"dataCode\":\"ncfx\",\"value\":0.0},{\"dataCode\":\"capex\",\"value\":-2151000000.0},{\"dataCode\":\"depamor\",\"value\":2850000000.0},{\"dataCode\":\"issrepayDebt\",\"value\":-3253000000.0},{\"dataCode\":\"ncf\",\"value\":-7286000000.0}],\"incomeStatement\":[{\"dataCode\":\"shareswaDil\",\"value\":15348175000.0},{\"dataCode\":\"intexp\",\"value\":0.0},{\"dataCode\":\"netinc\",\"value\":21448000000.0},{\"dataCode\":\"netIncDiscOps\",\"value\":0.0},{\"dataCode\":\"ebitda\",\"value\":28344000000.0},{\"dataCode\":\"ebt\",\"value\":25494000000.0},{\"dataCode\":\"eps\",\"value\":1.4},{\"dataCode\":\"opinc\",\"value\":25352000000.0},{\"dataCode\":\"shareswa\",\"value\":15287521000.0},{\"dataCode\":\"netIncComStock\",\"value\":21448000000.0},{\"dataCode\":\"consolidatedIncome\",\"value\":21448000000.0},{\"dataCode\":\"taxExp\",\"value\":4046000000.0},{\"dataCode\":\"prefDVDs\",\"value\":0.0},{\"dataCode\":\"sga\",\"value\":6320000000.0},{\"dataCode\":\"epsDil\",\"value\":1.4},{\"dataCode\":\"rnd\",\"value\":8006000000.0},{\"dataCode\":\"nonControllingInterests\",\"value\":0.0},{\"dataCode\":\"revenue\",\"value\":85777000000.0},{\"dataCode\":\"opex\",\"value\":14326000000.0},{\"dataCode\":\"costRev\",\"value\":46099000000.0},{\"dataCode\":\"ebit\",\"value\":25494000000.0},{\"dataCode\":\"grossProfit\",\"value\":39678000000.0}],\"balanceSheet\":[{\"dataCode\":\"debtNonCurrent\",\"value\":86196000000.0},{\"dataCode\":\"liabilitiesCurrent\",\"value\":131624000000.0},{\"dataCode\":\"totalLiabilities\",\"value\":264904000000.0},{\"dataCode\":\"debtCurrent\",\"value\":15108000000.0},{\"dataCode\":\"ppeq\",\"value\":44502000000.0},{\"dataCode\":\"taxLiabilities\",\"value\":0.0},{\"dataCode\":\"investmentsNonCurrent\",\"value\":91240000000.0},{\"dataCode\":\"accoci\",\"value\":-8416000000.0},{\"dataCode\":\"acctRec\",\"value\":43172000000.0},{\"dataCode\":\"totalAssets\",\"value\":331612000000.0},{\"dataCode\":\"assetsNonCurrent\",\"value\":206177000000.0},{\"dataCode\":\"intangibles\",\"value\":0.0},{\"dataCode\":\"cashAndEq\",\"value\":25565000000.0},{\"dataCode\":\"assetsCurrent\",\"value\":125435000000.0},{\"dataCode\":\"deferredRev\",\"value\":8053000000.0},{\"dataCode\":\"investmentsCurrent\",\"value\":36236000000.0},{\"dataCode\":\"retainedEarnings\",\"value\":-4726000000.0},{\"dataCode\":\"taxAssets\",\"value\":0.0},{\"dataCode\":\"deposits\",\"value\":0.0},{\"dataCode\":\"debt\",\"value\":101304000000.0},{\"dataCode\":\"inventory\",\"value\":6165000000.0},{\"dataCode\":\"investments\",\"value\":127476000000.0},{\"dataCode\":\"liabilitiesNonCurrent\",\"value\":133280000000.0},{\"dataCode\":\"sharesBasic\",\"value\":15334082000.0},{\"dataCode\":\"equity\",\"value\":66708000000.0},{\"dataCode\":\"acctPay\",\"value\":47574000000.0}],\"overview\":[{\"dataCode\":\"bvps\",\"value\":4.35030933054877},{\"dataCode\":\"debtEquity\",\"value\":1.51861845655693},{\"dataCode\":\"bookVal\",\"value\":66708000000.0},{\"dataCode\":\"piotroskiFScore\",\"value\":6.0},{\"dataCode\":\"shareFactor\",\"value\":1.0},{\"dataCode\":\"rps\",\"value\":5.59387904668829},{\"dataCode\":\"epsQoQ\",\"value\":0.102362204724409},{\"dataCode\":\"grossMargin\",\"value\":0.462571551814589},{\"dataCode\":\"roe\",\"value\":1.47150259067358},{\"dataCode\":\"profitMargin\",\"value\":0.462571551814589},{\"dataCode\":\"revenueQoQ\",\"value\":0.0486570412117804},{\"dataCode\":\"roa\",\"value\":0.296573389958695},{\"dataCode\":\"longTermDebtEquity\",\"value\":1.29213887389818},{\"dataCode\":\"currentRatio\",\"value\":0.952979699750805}]}}]"));
	// 第二个数据缺项
	Test_TiingoWebData tiingoFinancialState3(
		3, _T(""),
		_T(
			"[{\"date\":\"2024-06-29\",\"year\":2024,\"quarter\":3,\"statementData\":{\"cashFlow\":[{\"dataCode\":\"freeCashFlow\",\"value\":26707000000.0},{\"dataCode\":\"ncff\",\"value\":-36017000000.0},{\"dataCode\":\"sbcomp\",\"value\":2869000000.0},{\"dataCode\":\"ncfi\",\"value\":-127000000.0},{\"dataCode\":\"investmentsAcqDisposals\",\"value\":2412000000.0},{\"dataCode\":\"ncfo\",\"value\":28858000000.0},{\"dataCode\":\"payDiv\",\"value\":-3895000000.0},{\"dataCode\":\"businessAcqDisposals\",\"value\":0.0},{\"dataCode\":\"issrepayEquity\",\"value\":-26522000000.0},{\"dataCode\":\"ncfx\",\"value\":0.0},{\"dataCode\":\"capex\",\"value\":-2151000000.0},{\"dataCode\":\"depamor\",\"value\":2850000000.0},{\"dataCode\":\"issrepayDebt\",\"value\":-3253000000.0},{\"dataCode\":\"ncf\",\"value\":-7286000000.0}],\"incomeStatement\":[{\"dataCode\":\"shareswaDil\",\"value\":15348175000.0},{\"dataCode\":\"intexp\",\"value\":0.0},{\"dataCode\":\"netinc\",\"value\":21448000000.0},{\"dataCode\":\"netIncDiscOps\",\"value\":0.0},{\"dataCode\":\"ebitda\",\"value\":28344000000.0},{\"dataCode\":\"ebt\",\"value\":25494000000.0},{\"dataCode\":\"eps\",\"value\":1.4},{\"dataCode\":\"opinc\",\"value\":25352000000.0},{\"dataCode\":\"shareswa\",\"value\":15287521000.0},{\"dataCode\":\"netIncComStock\",\"value\":21448000000.0},{\"dataCode\":\"consolidatedIncome\",\"value\":21448000000.0},{\"dataCode\":\"taxExp\",\"value\":4046000000.0},{\"dataCode\":\"prefDVDs\",\"value\":0.0},{\"dataCode\":\"sga\",\"value\":6320000000.0},{\"dataCode\":\"epsDil\",\"value\":1.4},{\"dataCode\":\"rnd\",\"value\":8006000000.0},{\"dataCode\":\"nonControllingInterests\",\"value\":0.0},{\"dataCode\":\"revenue\",\"value\":85777000000.0},{\"dataCode\":\"opex\",\"value\":14326000000.0},{\"dataCode\":\"costRev\",\"value\":46099000000.0},{\"dataCode\":\"ebit\",\"value\":25494000000.0},{\"dataCode\":\"grossProfit\",\"value\":39678000000.0}],\"balanceSheet\":[{\"dataCode\":\"debtNonCurrent\",\"value\":86196000000.0},{\"dataCode\":\"liabilitiesCurrent\",\"value\":131624000000.0},{\"dataCode\":\"totalLiabilities\",\"value\":264904000000.0},{\"dataCode\":\"debtCurrent\",\"value\":15108000000.0},{\"dataCode\":\"ppeq\",\"value\":44502000000.0},{\"dataCode\":\"taxLiabilities\",\"value\":0.0},{\"dataCode\":\"investmentsNonCurrent\",\"value\":91240000000.0},{\"dataCode\":\"accoci\",\"value\":-8416000000.0},{\"dataCode\":\"acctRec\",\"value\":43172000000.0},{\"dataCode\":\"totalAssets\",\"value\":331612000000.0},{\"dataCode\":\"assetsNonCurrent\",\"value\":206177000000.0},{\"dataCode\":\"intangibles\",\"value\":0.0},{\"dataCode\":\"cashAndEq\",\"value\":25565000000.0},{\"dataCode\":\"assetsCurrent\",\"value\":125435000000.0},{\"dataCode\":\"deferredRev\",\"value\":8053000000.0},{\"dataCode\":\"investmentsCurrent\",\"value\":36236000000.0},{\"dataCode\":\"retainedEarnings\",\"value\":-4726000000.0},{\"dataCode\":\"taxAssets\",\"value\":0.0},{\"dataCode\":\"deposits\",\"value\":0.0},{\"dataCode\":\"debt\",\"value\":101304000000.0},{\"dataCode\":\"inventory\",\"value\":6165000000.0},{\"dataCode\":\"investments\",\"value\":127476000000.0},{\"dataCode\":\"liabilitiesNonCurrent\",\"value\":133280000000.0},{\"dataCode\":\"sharesBasic\",\"value\":15334082000.0},{\"dataCode\":\"equity\",\"value\":66708000000.0},{\"dataCode\":\"acctPay\",\"value\":47574000000.0}],\"overview\":[{\"dataCode\":\"bvps\",\"value\":4.35030933054877},{\"dataCode\":\"debtEquity\",\"value\":1.51861845655693},{\"dataCode\":\"bookVal\",\"value\":66708000000.0},{\"dataCode\":\"piotroskiFScore\",\"value\":6.0},{\"dataCode\":\"shareFactor\",\"value\":1.0},{\"dataCode\":\"rps\",\"value\":5.59387904668829},{\"dataCode\":\"epsQoQ\",\"value\":0.102362204724409},{\"dataCode\":\"grossMargin\",\"value\":0.462571551814589},{\"dataCode\":\"roe\",\"value\":1.47150259067358},{\"dataCode\":\"profitMargin\",\"value\":0.462571551814589},{\"dataCode\":\"revenueQoQ\",\"value\":0.0486570412117804},{\"dataCode\":\"roa\",\"value\":0.296573389958695},{\"dataCode\":\"longTermDebtEquity\",\"value\":1.29213887389818},{\"dataCode\":\"currentRatio\",\"value\":0.952979699750805}]}}]"));
	// 正确的数据
	Test_TiingoWebData tiingoFinancialState4(
		4, _T(""),
		_T(
			"[{\"date\":\"2024-06-29\",\"year\":2024,\"quarter\":3,\"statementData\":{\"cashFlow\":[{\"dataCode\":\"freeCashFlow\",\"value\":26707000000.0},{\"dataCode\":\"ncff\",\"value\":-36017000000.0},{\"dataCode\":\"sbcomp\",\"value\":2869000000.0},{\"dataCode\":\"ncfi\",\"value\":-127000000.0},{\"dataCode\":\"investmentsAcqDisposals\",\"value\":2412000000.0},{\"dataCode\":\"ncfo\",\"value\":28858000000.0},{\"dataCode\":\"payDiv\",\"value\":-3895000000.0},{\"dataCode\":\"businessAcqDisposals\",\"value\":0.0},{\"dataCode\":\"issrepayEquity\",\"value\":-26522000000.0},{\"dataCode\":\"ncfx\",\"value\":0.0},{\"dataCode\":\"capex\",\"value\":-2151000000.0},{\"dataCode\":\"depamor\",\"value\":2850000000.0},{\"dataCode\":\"issrepayDebt\",\"value\":-3253000000.0},{\"dataCode\":\"ncf\",\"value\":-7286000000.0}],\"incomeStatement\":[{\"dataCode\":\"shareswaDil\",\"value\":15348175000.0},{\"dataCode\":\"intexp\",\"value\":0.0},{\"dataCode\":\"netinc\",\"value\":21448000000.0},{\"dataCode\":\"netIncDiscOps\",\"value\":0.0},{\"dataCode\":\"ebitda\",\"value\":28344000000.0},{\"dataCode\":\"ebt\",\"value\":25494000000.0},{\"dataCode\":\"eps\",\"value\":1.4},{\"dataCode\":\"opinc\",\"value\":25352000000.0},{\"dataCode\":\"shareswa\",\"value\":15287521000.0},{\"dataCode\":\"netIncComStock\",\"value\":21448000000.0},{\"dataCode\":\"consolidatedIncome\",\"value\":21448000000.0},{\"dataCode\":\"taxExp\",\"value\":4046000000.0},{\"dataCode\":\"prefDVDs\",\"value\":0.0},{\"dataCode\":\"sga\",\"value\":6320000000.0},{\"dataCode\":\"epsDil\",\"value\":1.4},{\"dataCode\":\"rnd\",\"value\":8006000000.0},{\"dataCode\":\"nonControllingInterests\",\"value\":0.0},{\"dataCode\":\"revenue\",\"value\":85777000000.0},{\"dataCode\":\"opex\",\"value\":14326000000.0},{\"dataCode\":\"costRev\",\"value\":46099000000.0},{\"dataCode\":\"ebit\",\"value\":25494000000.0},{\"dataCode\":\"grossProfit\",\"value\":39678000000.0}],\"balanceSheet\":[{\"dataCode\":\"debtNonCurrent\",\"value\":86196000000.0},{\"dataCode\":\"liabilitiesCurrent\",\"value\":131624000000.0},{\"dataCode\":\"totalLiabilities\",\"value\":264904000000.0},{\"dataCode\":\"debtCurrent\",\"value\":15108000000.0},{\"dataCode\":\"ppeq\",\"value\":44502000000.0},{\"dataCode\":\"taxLiabilities\",\"value\":0.0},{\"dataCode\":\"investmentsNonCurrent\",\"value\":91240000000.0},{\"dataCode\":\"accoci\",\"value\":-8416000000.0},{\"dataCode\":\"acctRec\",\"value\":43172000000.0},{\"dataCode\":\"totalAssets\",\"value\":331612000000.0},{\"dataCode\":\"assetsNonCurrent\",\"value\":206177000000.0},{\"dataCode\":\"intangibles\",\"value\":0.0},{\"dataCode\":\"cashAndEq\",\"value\":25565000000.0},{\"dataCode\":\"assetsCurrent\",\"value\":125435000000.0},{\"dataCode\":\"deferredRev\",\"value\":8053000000.0},{\"dataCode\":\"investmentsCurrent\",\"value\":36236000000.0},{\"dataCode\":\"retainedEarnings\",\"value\":-4726000000.0},{\"dataCode\":\"taxAssets\",\"value\":0.0},{\"dataCode\":\"deposits\",\"value\":0.0},{\"dataCode\":\"debt\",\"value\":101304000000.0},{\"dataCode\":\"inventory\",\"value\":6165000000.0},{\"dataCode\":\"investments\",\"value\":127476000000.0},{\"dataCode\":\"liabilitiesNonCurrent\",\"value\":133280000000.0},{\"dataCode\":\"sharesBasic\",\"value\":15334082000.0},{\"dataCode\":\"equity\",\"value\":66708000000.0},{\"dataCode\":\"acctPay\",\"value\":47574000000.0}],\"overview\":[{\"dataCode\":\"bvps\",\"value\":4.35030933054877},{\"dataCode\":\"debtEquity\",\"value\":1.51861845655693},{\"dataCode\":\"bookVal\",\"value\":66708000000.0},{\"dataCode\":\"piotroskiFScore\",\"value\":6.0},{\"dataCode\":\"shareFactor\",\"value\":1.0},{\"dataCode\":\"rps\",\"value\":5.59387904668829},{\"dataCode\":\"epsQoQ\",\"value\":0.102362204724409},{\"dataCode\":\"grossMargin\",\"value\":0.462571551814589},{\"dataCode\":\"roe\",\"value\":1.47150259067358},{\"dataCode\":\"profitMargin\",\"value\":0.462571551814589},{\"dataCode\":\"revenueQoQ\",\"value\":0.0486570412117804},{\"dataCode\":\"roa\",\"value\":0.296573389958695},{\"dataCode\":\"longTermDebtEquity\",\"value\":1.29213887389818},{\"dataCode\":\"currentRatio\",\"value\":0.952979699750805}]}}]"));
	// 正确的数据
	Test_TiingoWebData tiingoFinancialState10(
		10, _T(""),
		_T(
			"[{\"date\":\"2024-06-29\",\"year\":2024,\"quarter\":3,\"statementData\":{\"cashFlow\":[{\"dataCode\":\"freeCashFlow\",\"value\":26707000000.0},{\"dataCode\":\"ncff\",\"value\":-36017000000.0},{\"dataCode\":\"sbcomp\",\"value\":2869000000.0},{\"dataCode\":\"ncfi\",\"value\":-127000000.0},{\"dataCode\":\"investmentsAcqDisposals\",\"value\":2412000000.0},{\"dataCode\":\"ncfo\",\"value\":28858000000.0},{\"dataCode\":\"payDiv\",\"value\":-3895000000.0},{\"dataCode\":\"businessAcqDisposals\",\"value\":0.0},{\"dataCode\":\"issrepayEquity\",\"value\":-26522000000.0},{\"dataCode\":\"ncfx\",\"value\":0.0},{\"dataCode\":\"capex\",\"value\":-2151000000.0},{\"dataCode\":\"depamor\",\"value\":2850000000.0},{\"dataCode\":\"issrepayDebt\",\"value\":-3253000000.0},{\"dataCode\":\"ncf\",\"value\":-7286000000.0}],\"incomeStatement\":[{\"dataCode\":\"shareswaDil\",\"value\":15348175000.0},{\"dataCode\":\"intexp\",\"value\":0.0},{\"dataCode\":\"netinc\",\"value\":21448000000.0},{\"dataCode\":\"netIncDiscOps\",\"value\":0.0},{\"dataCode\":\"ebitda\",\"value\":28344000000.0},{\"dataCode\":\"ebt\",\"value\":25494000000.0},{\"dataCode\":\"eps\",\"value\":1.4},{\"dataCode\":\"opinc\",\"value\":25352000000.0},{\"dataCode\":\"shareswa\",\"value\":15287521000.0},{\"dataCode\":\"netIncComStock\",\"value\":21448000000.0},{\"dataCode\":\"consolidatedIncome\",\"value\":21448000000.0},{\"dataCode\":\"taxExp\",\"value\":4046000000.0},{\"dataCode\":\"prefDVDs\",\"value\":0.0},{\"dataCode\":\"sga\",\"value\":6320000000.0},{\"dataCode\":\"epsDil\",\"value\":1.4},{\"dataCode\":\"rnd\",\"value\":8006000000.0},{\"dataCode\":\"nonControllingInterests\",\"value\":0.0},{\"dataCode\":\"revenue\",\"value\":85777000000.0},{\"dataCode\":\"opex\",\"value\":14326000000.0},{\"dataCode\":\"costRev\",\"value\":46099000000.0},{\"dataCode\":\"ebit\",\"value\":25494000000.0},{\"dataCode\":\"grossProfit\",\"value\":39678000000.0}],\"balanceSheet\":[{\"dataCode\":\"debtNonCurrent\",\"value\":86196000000.0},{\"dataCode\":\"liabilitiesCurrent\",\"value\":131624000000.0},{\"dataCode\":\"totalLiabilities\",\"value\":264904000000.0},{\"dataCode\":\"debtCurrent\",\"value\":15108000000.0},{\"dataCode\":\"ppeq\",\"value\":44502000000.0},{\"dataCode\":\"taxLiabilities\",\"value\":0.0},{\"dataCode\":\"investmentsNonCurrent\",\"value\":91240000000.0},{\"dataCode\":\"accoci\",\"value\":-8416000000.0},{\"dataCode\":\"acctRec\",\"value\":43172000000.0},{\"dataCode\":\"totalAssets\",\"value\":331612000000.0},{\"dataCode\":\"assetsNonCurrent\",\"value\":206177000000.0},{\"dataCode\":\"intangibles\",\"value\":0.0},{\"dataCode\":\"cashAndEq\",\"value\":25565000000.0},{\"dataCode\":\"assetsCurrent\",\"value\":125435000000.0},{\"dataCode\":\"deferredRev\",\"value\":8053000000.0},{\"dataCode\":\"investmentsCurrent\",\"value\":36236000000.0},{\"dataCode\":\"retainedEarnings\",\"value\":-4726000000.0},{\"dataCode\":\"taxAssets\",\"value\":0.0},{\"dataCode\":\"deposits\",\"value\":0.0},{\"dataCode\":\"debt\",\"value\":101304000000.0},{\"dataCode\":\"inventory\",\"value\":6165000000.0},{\"dataCode\":\"investments\",\"value\":127476000000.0},{\"dataCode\":\"liabilitiesNonCurrent\",\"value\":133280000000.0},{\"dataCode\":\"sharesBasic\",\"value\":15334082000.0},{\"dataCode\":\"equity\",\"value\":66708000000.0},{\"dataCode\":\"acctPay\",\"value\":47574000000.0}],\"overview\":[{\"dataCode\":\"bvps\",\"value\":4.35030933054877},{\"dataCode\":\"debtEquity\",\"value\":1.51861845655693},{\"dataCode\":\"bookVal\",\"value\":66708000000.0},{\"dataCode\":\"piotroskiFScore\",\"value\":6.0},{\"dataCode\":\"shareFactor\",\"value\":1.0},{\"dataCode\":\"rps\",\"value\":5.59387904668829},{\"dataCode\":\"epsQoQ\",\"value\":0.102362204724409},{\"dataCode\":\"grossMargin\",\"value\":0.462571551814589},{\"dataCode\":\"roe\",\"value\":1.47150259067358},{\"dataCode\":\"profitMargin\",\"value\":0.462571551814589},{\"dataCode\":\"revenueQoQ\",\"value\":0.0486570412117804},{\"dataCode\":\"roa\",\"value\":0.296573389958695},{\"dataCode\":\"longTermDebtEquity\",\"value\":1.29213887389818},{\"dataCode\":\"currentRatio\",\"value\":0.952979699750805}]}}]"));

	class ParseTiingoFinancialStateTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_tiingoFinancialStateProduct.SetIndex(m_lIndex);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CTiingoCompanyFinancialStatesPtr m_pvFinancialState;
		CProductTiingoFinancialState m_tiingoFinancialStateProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoFinancialState,
	                         ParseTiingoFinancialStateTest,
	                         testing::Values(&tiingoFinancialState1, &tiingoFinancialState2,
		                         &tiingoFinancialState3, &tiingoFinancialState4, &tiingoFinancialState10));

	TEST_P(ParseTiingoFinancialStateTest, TestParseFinancialState) {
		m_pvFinancialState = m_tiingoFinancialStateProduct.ParseTiingoFinancialState(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_EQ(m_pvFinancialState->size(), 1);
			break;
		case 2: // 格式不对
			EXPECT_EQ(m_pvFinancialState->size(), 1);
			break;
		case 3: // 缺乏address项
			EXPECT_EQ(m_pvFinancialState->size(), 1);
			break;
		case 4:
			EXPECT_EQ(m_pvFinancialState->size(), 1);
			break;
		case 10:
			EXPECT_EQ(m_pvFinancialState->size(), 1);
			break;
		default:
			break;
		}
	}
	/*
	class ProcessTiingoFinancialStateTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;

			m_tiingoFinancialStateProduct.SetMarket(gl_pWorldMarket);
			m_tiingoFinancialStateProduct.SetIndex(0);
			EXPECT_FALSE(gl_dataContainerFinnhubFinancialState.IsUpdateProfileDB());
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductTiingoFinancialState m_tiingoFinancialStateProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoFinancialState,
	                         ProcessTiingoFinancialStateTest,
	                         testing::Values(&tiingoFinancialState1, &tiingoFinancialState2,
		                         &tiingoFinancialState3, &tiingoFinancialState4, &tiingoFinancialState10));

	TEST_P(ProcessTiingoFinancialStateTest, TestProcessFinancialState) {
		CTiingoCompanyFinancialStatePtr pTiingoFinancialState = nullptr;
		CFinnhubStockPtr pStock = nullptr;
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
		m_tiingoFinancialStateProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 2: // 格式不对
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 3: // 第二个数据缺乏address项,返回一个成功
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1) << "第一个数据是正确的";
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateProfileDB()) << "第一个数据是正确的";

		//恢复原状
			pStock = gl_dataContainerFinnhubStock.GetStock(_T("A"));
			EXPECT_TRUE(pStock->IsUpdateCompanyProfile());
			pStock->SetUpdateProfileDB(false);
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 4:
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());

		//恢复原状
			pStock = gl_dataContainerFinnhubStock.GetStock(_T("AA"));
			EXPECT_TRUE(pStock->IsUpdateCompanyProfile());
			pStock->SetUpdateProfileDB(false);
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 10:
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_TRUE(gl_dataContainerTiingoStock.IsStock(_T("NEW SYMBOL")));
			EXPECT_TRUE((pTiingoStock = gl_dataContainerTiingoStock.GetStock(_T("NEW SYMBOL"))) != nullptr);
			pStock = gl_dataContainerFinnhubStock.GetStock(_T("AA"));
			EXPECT_STREQ(pStock->GetName(), _T("New Name")) << "更改为此新名字";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());

		// 恢复原状
			gl_dataContainerTiingoStock.Delete(pTiingoStock);
			pStock->SetUpdateProfileDB(false);
			pStock->SetName(_T("Alcoa Corp"));
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		default:
			break;
		}
	}
	*/
}
