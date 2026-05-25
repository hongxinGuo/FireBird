#include"pch.h"

#include"GeneralCheck.h"

#include"NaicsIndustry.h"

namespace FireBirdTest {
	class CNaicsIndustryTest : public ::testing::Test {
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
	};

	TEST_F(CNaicsIndustryTest, TestInitialize) {
		const CNaicsIndustry NaicsIndustry;
		EXPECT_EQ(NaicsIndustry.m_strNaics, " ");
		EXPECT_EQ(NaicsIndustry.m_strNationalIndustry, " ");
		EXPECT_EQ(NaicsIndustry.m_strSector, " ");
		EXPECT_EQ(NaicsIndustry.m_strSubSector, " ");
		EXPECT_FALSE(NaicsIndustry.m_fUpdated);
	}

	TEST_F(CNaicsIndustryTest, TestIsUpdated) {
		CNaicsIndustry finnhubExchange;

		EXPECT_FALSE(finnhubExchange.IsUpdated());
		finnhubExchange.SetUpdated(true);
		EXPECT_TRUE(finnhubExchange.IsUpdated());
	}

}
