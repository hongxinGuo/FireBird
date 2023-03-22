#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerWorldStock.h"

#include<memory>
using std::make_shared;

using namespace std;

using namespace testing;

CContainerWorldStock m_containerStock{};

namespace FireBirdTest {
	class CContainerWorldStockTest : public Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
			m_containerStock.LoadDB();
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CContainerWorldStockTest, TestValidateStockSymbol1) {
		const auto pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("AAPL"));
		pStock->SetExchangeCode(_T("US"));

		EXPECT_TRUE(m_containerStock.ValidateStockSymbol(pStock));
	}

	TEST_F(CContainerWorldStockTest, TestValidateStockSymbol2) {
		const auto pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("600601.SS"));
		pStock->SetExchangeCode(_T("SS"));

		EXPECT_TRUE(m_containerStock.ValidateStockSymbol(pStock));
	}

	TEST_F(CContainerWorldStockTest, TestValidateStockSymbol3) {
		const auto pStock = make_shared<CWorldStock>();
		pStock->SetSymbol(_T("600601SS"));
		pStock->SetExchangeCode(_T("SS"));

		EXPECT_FALSE(m_containerStock.ValidateStockSymbol(pStock));
	}

	TEST_F(CContainerWorldStockTest, TestIsNeedSaveDayLine) {
		EXPECT_FALSE(m_containerStock.IsDayLineNeedSaving());
		m_containerStock.Get(0)->SetDayLineNeedSaving(true);

		EXPECT_TRUE(m_containerStock.IsDayLineNeedSaving());

		m_containerStock.Get(0)->SetDayLineNeedSaving(false);
	}

	TEST_F(CContainerWorldStockTest, TestIsSaveInsiderTransaction) {
		EXPECT_FALSE(m_containerStock.IsSaveInsiderTransaction());
		m_containerStock.GetStock(0)->SetSaveInsiderTransaction(true);

		EXPECT_TRUE(m_containerStock.IsSaveInsiderTransaction());

		m_containerStock.GetStock(0)->SetSaveInsiderTransaction(false);
	}

	TEST_F(CContainerWorldStockTest, TestIsSaveInsiderSentiment) {
		EXPECT_FALSE(m_containerStock.IsSaveInsiderSentiment());
		m_containerStock.GetStock(0)->SetSaveInsiderSentiment(true);

		EXPECT_TRUE(m_containerStock.IsSaveInsiderSentiment());

		m_containerStock.GetStock(0)->SetSaveInsiderSentiment(false);
	}

	TEST_F(CContainerWorldStockTest, TestIsUpdateBasicFinancialDB) {
		EXPECT_FALSE(m_containerStock.IsUpdateBasicFinancialDB());
		m_containerStock.GetStock(0)->SetUpdateBasicFinancialDB(true);

		EXPECT_TRUE(m_containerStock.IsUpdateBasicFinancialDB());

		m_containerStock.GetStock(0)->SetUpdateBasicFinancialDB(false);
	}

	TEST_F(CContainerWorldStockTest, TestIsUpdateCompanyNewsDB) {
		EXPECT_FALSE(m_containerStock.IsUpdateCompanyNewsDB());
		m_containerStock.GetStock(0)->SetUpdateCompanyNewsDB(true);

		EXPECT_TRUE(m_containerStock.IsUpdateCompanyNewsDB());

		m_containerStock.GetStock(0)->SetUpdateCompanyNewsDB(false);
	}
}
