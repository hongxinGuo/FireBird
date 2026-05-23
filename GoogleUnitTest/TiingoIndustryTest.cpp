#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoIndustry.h"

namespace FireBirdTest {
	class CTiingoIndustryTest : public ::testing::Test {
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

	TEST_F(CTiingoIndustryTest, TestInitialize) {
		const CTiingoIndustry TiingoIndustry;
		EXPECT_EQ(TiingoIndustry.m_strIndustry, " ");
		EXPECT_EQ(TiingoIndustry.m_strSector, " ");
		EXPECT_FALSE(TiingoIndustry.m_fUpdated);
	}

	TEST_F(CTiingoIndustryTest, TestIsUpdated) {
		CTiingoIndustry finnhubExchange;

		EXPECT_FALSE(finnhubExchange.IsUpdated());
		finnhubExchange.SetUpdated(true);
		EXPECT_TRUE(finnhubExchange.IsUpdated());
	}
}
