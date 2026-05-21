#include"pch.h"

#include"GeneralCheck.h"

#include"Country.h"

namespace FireBirdTest {
	class CCountryTest : public ::testing::Test {
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

	TEST_F(CCountryTest, TestInitialize) {
		const CCountry country;
		EXPECT_EQ(country.m_strCode2.compare(" "), 0);
		EXPECT_EQ(country.m_strCode3.compare(" "), 0);
		EXPECT_EQ(country.m_strCodeNo.compare(" "), 0);
		EXPECT_EQ(country.m_strCountry.compare(" "), 0);
		EXPECT_EQ(country.m_strCurrency.compare(" "), 0);
		EXPECT_EQ(country.m_strCurrencyCode.compare(" "), 0);
	}
}
