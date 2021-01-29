#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"EconomicCalendar.h"

namespace StockAnalysisTest {
  class CEconomicCalendarTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestSuite() {
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
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
    CEconomicCalendar EconomicCalendar;

    EconomicCalendar.m_strEvent = _T("Abcdefghighlmnop");

    ASSERT(!gl_fNormalMode);
    setEconomicCalendar.Open();
    setEconomicCalendar.m_pDatabase->BeginTrans();
    EconomicCalendar.Append(setEconomicCalendar);
    setEconomicCalendar.m_pDatabase->CommitTrans();
    setEconomicCalendar.Close();

    setEconomicCalendar2.m_strFilter = _T("[Event] = 'Abcdefghighlmnop'");
    setEconomicCalendar2.Open();
    EXPECT_TRUE(!setEconomicCalendar2.IsEOF()) << "此时已经存入了Abcdefghighlmnop";
    setEconomicCalendar2.Delete();
    setEconomicCalendar2.Close();
  }
}