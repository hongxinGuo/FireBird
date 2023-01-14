#include"pch.h"

#include"GeneralCheck.h"

#include"DataWorldStock.h"

#include<memory>
using std::make_shared;

using namespace std;

using namespace testing;

CDataWorldStock m_dataWorldStock{};

namespace FireBirdTest {
	class CDataWorldStockTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			m_dataWorldStock.LoadDB();
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
		EXPECT_FALSE(m_dataWorldStock.IsNeedSaveDayLine());
		m_dataWorldStock.GetStock(0)->SetDayLineNeedSaving(true);

		EXPECT_TRUE(m_dataWorldStock.IsNeedSaveDayLine());

		m_dataWorldStock.GetStock(0)->SetDayLineNeedSaving(false);
	}

	TEST_F(CDataWorldStockTest, TestIsNeedSaveInsiderTransaction) {
		EXPECT_FALSE(m_dataWorldStock.IsNeedSaveInsiderTransaction());
		m_dataWorldStock.GetStock(0)->SetInsiderTransactionNeedSave(true);

		EXPECT_TRUE(m_dataWorldStock.IsNeedSaveInsiderTransaction());

		m_dataWorldStock.GetStock(0)->SetInsiderTransactionNeedSave(false);
	}

	TEST_F(CDataWorldStockTest, TestIsNeedSaveInsiderSentiment) {
		EXPECT_FALSE(m_dataWorldStock.IsNeedSaveInsiderSentiment());
		m_dataWorldStock.GetStock(0)->SetInsiderSentimentNeedSave(true);

		EXPECT_TRUE(m_dataWorldStock.IsNeedSaveInsiderSentiment());

		m_dataWorldStock.GetStock(0)->SetInsiderSentimentNeedSave(false);
	}

}
