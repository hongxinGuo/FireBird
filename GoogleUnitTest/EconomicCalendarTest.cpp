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

	TEST_F(CEconomicCalendarTest, TestAppend) {
		CSetEconomicCalendar setEconomicCalendar, setEconomicCalendar2;
		CEconomicCalendar EconomicCalendar, EconomicCalendar2;

		EconomicCalendar.m_strEvent = "Abcdefghighlmnop";
		EconomicCalendar.m_strCountry = "US";
		EconomicCalendar.m_strImpact = "High";
		EconomicCalendar.m_strTime = "20202020";
		EconomicCalendar.m_strUnit = "Doller";
		EconomicCalendar.m_dActual = 1.2;
		EconomicCalendar.m_dEstimate = 1.1;
		EconomicCalendar.m_dPrev = 1.0;

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setEconomicCalendar.Open();
		setEconomicCalendar.m_pDatabase->BeginTrans();
		EconomicCalendar.Append(setEconomicCalendar);
		setEconomicCalendar.m_pDatabase->CommitTrans();
		setEconomicCalendar.Close();

		setEconomicCalendar2.m_strFilter = "[Event] = 'Abcdefghighlmnop'";
		setEconomicCalendar2.Open();
		EXPECT_TRUE(!setEconomicCalendar2.IsEOF()) << "此时已经存入了Abcdefghighlmnop";
		EconomicCalendar2.Load(setEconomicCalendar2);
		EXPECT_EQ(EconomicCalendar2.m_strCountry, "US");
		EXPECT_EQ(EconomicCalendar2.m_strEvent, "Abcdefghighlmnop");
		EXPECT_EQ(EconomicCalendar2.m_strImpact, "High");
		EXPECT_EQ(EconomicCalendar2.m_strTime, "20202020");
		EXPECT_EQ(EconomicCalendar2.m_strUnit, "Doller");
		EXPECT_DOUBLE_EQ(EconomicCalendar2.m_dActual, 1.2);
		EXPECT_DOUBLE_EQ(EconomicCalendar2.m_dEstimate, 1.1);
		EXPECT_DOUBLE_EQ(EconomicCalendar2.m_dPrev, 1.0);
		setEconomicCalendar2.m_pDatabase->BeginTrans();
		while (!setEconomicCalendar2.IsEOF()) {
			setEconomicCalendar2.Delete();
			setEconomicCalendar2.MoveNext();
		}
		setEconomicCalendar2.m_pDatabase->CommitTrans();
		setEconomicCalendar2.Close();
	}
}
