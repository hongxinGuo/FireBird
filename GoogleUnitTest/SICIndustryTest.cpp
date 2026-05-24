#include"pch.h"

#include"GeneralCheck.h"

#include"SICIndustry.h"

namespace FireBirdTest {
	class CSICIndustryTest : public ::testing::Test {
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

	TEST_F(CSICIndustryTest, TestInitialize) {
		const CSICIndustry SICIndustry;
		EXPECT_EQ(SICIndustry.m_lCode, 0);
		EXPECT_EQ(SICIndustry.m_strIndustry, " ");
		EXPECT_EQ(SICIndustry.m_strSector, " ");
		EXPECT_FALSE(SICIndustry.m_fUpdated);
	}

	TEST_F(CSICIndustryTest, TestIsUpdated) {
		CSICIndustry finnhubExchange;

		EXPECT_FALSE(finnhubExchange.IsUpdated());
		finnhubExchange.SetUpdated(true);
		EXPECT_TRUE(finnhubExchange.IsUpdated());
	}
}
