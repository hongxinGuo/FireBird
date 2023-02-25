#include"pch.h"

#include"GeneralCheck.h"

#include"EconomicCalendar.h"

namespace FireBirdTest {
	class CEconomicCalendarTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}
	};

	TEST_F(CEconomicCalendarTest, TestInitialize) {
		CEconomicCalendar EconomicCalendar;
		EXPECT_STREQ(EconomicCalendar.m_strCountry, _T(" "));
		EXPECT_STREQ(EconomicCalendar.m_strEvent, _T(" "));
		EXPECT_STREQ(EconomicCalendar.m_strImpact, _T(" "));
		EXPECT_STREQ(EconomicCalendar.m_strUnit, _T(" "));
		EXPECT_DOUBLE_EQ(EconomicCalendar.m_dActual, 0.0);
		EXPECT_DOUBLE_EQ(EconomicCalendar.m_dEstimate, 0.0);
		EXPECT_DOUBLE_EQ(EconomicCalendar.m_dPrev, 0.0);
	}

	TEST_F(CEconomicCalendarTest, TestAppend) {
		CSetEconomicCalendar setEconomicCalendar, setEconomicCalendar2;
		CEconomicCalendar EconomicCalendar, EconomicCalendar2;

		EconomicCalendar.m_strEvent = _T("Abcdefghighlmnop");
		EconomicCalendar.m_strCountry = _T("US");
		EconomicCalendar.m_strImpact = _T("High");
		EconomicCalendar.m_strTime = _T("20202020");
		EconomicCalendar.m_strUnit = _T("Doller");
		EconomicCalendar.m_dActual = 1.2;
		EconomicCalendar.m_dEstimate = 1.1;
		EconomicCalendar.m_dPrev = 1.0;

		ASSERT(!gl_systemStatus.IsWorkingMode());
		setEconomicCalendar.Open();
		setEconomicCalendar.m_pDatabase->BeginTrans();
		EconomicCalendar.Append(setEconomicCalendar);
		setEconomicCalendar.m_pDatabase->CommitTrans();
		setEconomicCalendar.Close();

		setEconomicCalendar2.m_strFilter = _T("[Event] = 'Abcdefghighlmnop'");
		setEconomicCalendar2.Open();
		EXPECT_TRUE(!setEconomicCalendar2.IsEOF()) << "此时已经存入了Abcdefghighlmnop";
		EconomicCalendar2.Load(setEconomicCalendar2);
		EXPECT_STREQ(EconomicCalendar2.m_strCountry, _T("US"));
		EXPECT_STREQ(EconomicCalendar2.m_strEvent, _T("Abcdefghighlmnop"));
		EXPECT_STREQ(EconomicCalendar2.m_strImpact, _T("High"));
		EXPECT_STREQ(EconomicCalendar2.m_strTime, _T("20202020"));
		EXPECT_STREQ(EconomicCalendar2.m_strUnit, _T("Doller"));
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
