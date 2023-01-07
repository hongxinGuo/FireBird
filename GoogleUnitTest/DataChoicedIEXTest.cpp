#include"pch.h"

#include"GeneralCheck.h"

#include"DataChosenStock.h"

using namespace testing;

namespace FireBirdTest {
	class CDataChosenIEXTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CDataChosenStock m_dataChosenIEX;
	};

	TEST_F(CDataChosenIEXTest, TestInitialize) {
		EXPECT_EQ(m_dataChosenIEX.GetSize(), 0);
	}

	TEST_F(CDataChosenIEXTest, TestLoad) {
		EXPECT_EQ(m_dataChosenIEX.GetSize(), 0) << "初始未装载IEX代码";

		m_dataChosenIEX.LoadDB();
		EXPECT_EQ(m_dataChosenIEX.GetSize(), 4) << "默认状态下装载4个代码";

		CWorldStockPtr pIEX = m_dataChosenIEX.GetStock(2);
		EXPECT_STREQ(pIEX->GetSymbol(), _T("AAL")) << "装载时没有排序，使用的是原始位置";
	}
}
