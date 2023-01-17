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

	TEST_F(CDataWorldStockTest, TestIsNeedSaveInsiderTransaction) {
		EXPECT_FALSE(m_containerStock.IsNeedSaveInsiderTransaction());
		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetInsiderTransactionNeedSave(true);

		EXPECT_TRUE(m_containerStock.IsNeedSaveInsiderTransaction());

		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetInsiderTransactionNeedSave(false);
	}

	TEST_F(CDataWorldStockTest, TestIsNeedSaveInsiderSentiment) {
		EXPECT_FALSE(m_containerStock.IsNeedSaveInsiderSentiment());
		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetInsiderSentimentNeedSave(true);

		EXPECT_TRUE(m_containerStock.IsNeedSaveInsiderSentiment());

		dynamic_pointer_cast<CWorldStock>(m_containerStock.Get(0))->SetInsiderSentimentNeedSave(false);
	}
}
