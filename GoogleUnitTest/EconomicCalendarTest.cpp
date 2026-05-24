#include"pch.h"

#include"GeneralCheck.h"

#include"EconomicCalendar.h"

namespace FireBirdTest {
	class CEconomicCalendarTest : public ::testing::Test {
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

	TEST_F(CEconomicCalendarTest, TestInitialize) {
		const CEconomicCalendar EconomicCalendar;
		EXPECT_EQ(EconomicCalendar.m_strCountry, " ");
		EXPECT_EQ(EconomicCalendar.m_strEvent, " ");
		EXPECT_EQ(EconomicCalendar.m_strImpact, " ");
		EXPECT_EQ(EconomicCalendar.m_strUnit, " ");
		EXPECT_DOUBLE_EQ(EconomicCalendar.m_dActual, 0.0);
		EXPECT_DOUBLE_EQ(EconomicCalendar.m_dEstimate, 0.0);
		EXPECT_DOUBLE_EQ(EconomicCalendar.m_dPrev, 0.0);
	}
}
