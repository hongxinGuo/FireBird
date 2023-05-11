#include"pch.h"

#include"GeneralCheck.h"

#include"StockSection.h"

namespace FireBirdTest {
	class CStockSectionTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
			SCOPED_TRACE(""); GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE(""); GeneralCheck();
		}
	};

	TEST_F(CStockSectionTest, TestIsActive) {
		CStockSection stockSection;

		EXPECT_FALSE(stockSection.IsActive());
		stockSection.SetActive(true);
		EXPECT_TRUE(stockSection.IsActive());
		stockSection.SetActive(false);
		EXPECT_FALSE(stockSection.IsActive());
	}

	TEST_F(CStockSectionTest, TestGetIndexNumber) {
		CStockSection stockSection;

		EXPECT_EQ(stockSection.GetIndexNumber(), 0);
		stockSection.SetIndexNumber(10101010);
		EXPECT_EQ(stockSection.GetIndexNumber(), 10101010);
	}

	TEST_F(CStockSectionTest, TestGetMarket) {
		CStockSection stockSection;

		EXPECT_EQ(stockSection.GetMarket(), 0);
		stockSection.SetMarket(1010);
		EXPECT_EQ(stockSection.GetMarket(), 1010);
	}

	TEST_F(CStockSectionTest, TestGetComment) {
		CStockSection stockSection;

		EXPECT_STREQ(stockSection.GetComment(), _T(""));
		stockSection.SetComment(_T("abc"));
		EXPECT_STREQ(stockSection.GetComment(), _T("abc"));
	}

	TEST_F(CStockSectionTest, TestIsBuildStockPtr) {
		CStockSection stockSection;

		EXPECT_FALSE(stockSection.IsBuildStockPtr());
		stockSection.SetBuildStockPtr(true);
		EXPECT_TRUE(stockSection.IsBuildStockPtr());
		stockSection.SetBuildStockPtr(false);
		EXPECT_FALSE(stockSection.IsBuildStockPtr());
	}
}
