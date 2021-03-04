#include"pch.h"
#include"globedef.h"

#include"FinnhubExchange.h"

namespace StockAnalysisTest {
  class CFinnhubExchangeTest : public ::testing::Test
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

  TEST_F(CFinnhubExchangeTest, TestInitialize) {
    CFinnhubExchange FinnhubExchange;
    EXPECT_STREQ(FinnhubExchange.m_strCode, _T(" "));
    EXPECT_STREQ(FinnhubExchange.m_strName, _T(" "));
    EXPECT_STREQ(FinnhubExchange.m_strMic, _T(" "));
    EXPECT_STREQ(FinnhubExchange.m_strTimeZone, _T(" "));
    EXPECT_STREQ(FinnhubExchange.m_strHour, _T(" "));
    EXPECT_STREQ(FinnhubExchange.m_strCloseDate, _T(" "));
    EXPECT_STREQ(FinnhubExchange.m_strCountry, _T(""));
    EXPECT_STREQ(FinnhubExchange.m_strSource, _T(""));
    EXPECT_FALSE(FinnhubExchange.m_fIsActive);
    EXPECT_FALSE(FinnhubExchange.m_fUpdated);
  }

  TEST_F(CFinnhubExchangeTest, TestIsUpdated) {
    CFinnhubExchange finnhubExchange;

    EXPECT_FALSE(finnhubExchange.IsUpdated());
    finnhubExchange.SetUpdated(true);
    EXPECT_TRUE(finnhubExchange.IsUpdated());
  }

  TEST_F(CFinnhubExchangeTest, TestAppend) {
    CSetFinnhubExchange setFinnhubExchange, setFinnhubExchange2;
    CFinnhubExchange FinnhubExchange, FinnhubExchange2;

    FinnhubExchange.m_strCode = _T("AA");
    FinnhubExchange.m_strName = _T("aaa");
    FinnhubExchange.m_strMic = _T("abdc");
    FinnhubExchange.m_strTimeZone = _T("Beijing");
    FinnhubExchange.m_strHour = _T("10101010");
    FinnhubExchange.m_strCloseDate = _T("20202020");
    FinnhubExchange.m_strCountry = _T("dfe");
    FinnhubExchange.m_strSource = _T("abc");
    FinnhubExchange.m_fIsActive = false;
    FinnhubExchange.m_fUpdated = true;

    ASSERT(!gl_fNormalMode);
    setFinnhubExchange.Open();
    setFinnhubExchange.m_pDatabase->BeginTrans();
    FinnhubExchange.Append(setFinnhubExchange);
    setFinnhubExchange.m_pDatabase->CommitTrans();
    setFinnhubExchange.Close();

    setFinnhubExchange2.m_strFilter = _T("[Code] = 'AA'");
    setFinnhubExchange2.Open();
    EXPECT_TRUE(!setFinnhubExchange2.IsEOF()) << "此时已经存入了AA";
    FinnhubExchange2.Load(setFinnhubExchange2);
    EXPECT_STREQ(FinnhubExchange.m_strCode, _T("AA"));
    EXPECT_STREQ(FinnhubExchange.m_strName, _T("aaa"));
    EXPECT_STREQ(FinnhubExchange.m_strMic, _T("abdc"));
    EXPECT_STREQ(FinnhubExchange.m_strTimeZone, _T("Beijing"));
    EXPECT_STREQ(FinnhubExchange.m_strHour, _T("10101010"));
    EXPECT_STREQ(FinnhubExchange.m_strCloseDate, _T("20202020"));
    EXPECT_STREQ(FinnhubExchange.m_strCountry, _T("dfe"));
    EXPECT_STREQ(FinnhubExchange.m_strSource, _T("abc"));
    EXPECT_FALSE(FinnhubExchange.m_fIsActive);
    EXPECT_TRUE(FinnhubExchange.m_fUpdated);

    setFinnhubExchange2.Delete();
    setFinnhubExchange2.Close();
  }
}