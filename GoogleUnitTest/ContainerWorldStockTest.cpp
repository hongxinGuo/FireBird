#include"pch.h"

#include"GeneralCheck.h"

#include"ContainerWorldStock.h"

#include<memory>
using std::make_shared;

using namespace std;

using namespace testing;

CContainerWorldStock m_containerStock{};

namespace FireBirdTest {
	class CDataWorldStockTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			m_containerStock.LoadDB();
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp

			GeneralCheck();
		}

	protected:
	};

	TEST_F(CDataWorldStockTest, TestIsNeedSaveDayLine) {
		EXPECT_FALSE(m_containerStock.IsDayLineNeedSaving());
		m_containerStock.Get(0)->SetDayLineNeedSaving(true);

		EXPECT_TRUE(m_containerStock.IsDayLineNeedSaving());

		m_containerStock.Get(0)->SetDayLineNeedSaving(false);
	}

	TEST_F(CDataWorldStockTest, TestIsSaveInsiderTransaction) {
		EXPECT_FALSE(m_containerStock.IsSaveInsiderTransaction());
		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetSaveInsiderTransaction(true);

		EXPECT_TRUE(m_containerStock.IsSaveInsiderTransaction());

		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetSaveInsiderTransaction(false);
	}

	TEST_F(CDataWorldStockTest, TestIsSaveInsiderSentiment) {
		EXPECT_FALSE(m_containerStock.IsSaveInsiderSentiment());
		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetSaveInsiderSentiment(true);

		EXPECT_TRUE(m_containerStock.IsSaveInsiderSentiment());

		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetSaveInsiderSentiment(false);
	}

	TEST_F(CDataWorldStockTest, TestIsUpdateBasicFinancialDB) {
		EXPECT_FALSE(m_containerStock.IsUpdateBasicFinancialDB());
		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetUpdateBasicFinancialDB(true);

		EXPECT_TRUE(m_containerStock.IsUpdateBasicFinancialDB());

		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetUpdateBasicFinancialDB(false);
	}

	TEST_F(CDataWorldStockTest, TestIsUpdateCompanyNewsDB) {
		EXPECT_FALSE(m_containerStock.IsUpdateCompanyNewsDB());
		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetUpdateCompanyNewsDB(true);

		EXPECT_TRUE(m_containerStock.IsUpdateCompanyNewsDB());

		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetUpdateCompanyNewsDB(false);
	}
}
