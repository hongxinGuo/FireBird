#include"pch.h"
#include"globedef.h"

#include"TiingoStockFundanmental.h"

namespace StockAnalysisTest {
  class CTiingoStockFundanmentalTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestSuite() {
      ASSERT_FALSE(gl_fNormalMode);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(CTiingoStockFundanmentalTest, TestInitialize) {
    CTiingoStockFundanmental tiingo;

    EXPECT_STREQ(tiingo.m_strTiingoPermaTicker, _T(""));
    EXPECT_STREQ(tiingo.m_strTicker, _T(""));
    EXPECT_STREQ(tiingo.m_strName, _T(""));
    EXPECT_FALSE(tiingo.m_fIsActive);
    EXPECT_FALSE(tiingo.m_fIsADR);
    EXPECT_STREQ(tiingo.m_strTiingoIndustry, _T(""));
    EXPECT_STREQ(tiingo.m_strTiingoSector, _T(""));
    EXPECT_EQ(tiingo.m_iSICCode, 0);
    EXPECT_STREQ(tiingo.m_strSICIndustry, _T(""));
    EXPECT_STREQ(tiingo.m_strSICSector, _T(""));
    EXPECT_STREQ(tiingo.m_strReportingCurrency, _T(""));
    EXPECT_STREQ(tiingo.m_strLocation, _T(""));
    EXPECT_STREQ(tiingo.m_strCompanyWebSite, _T(""));
    EXPECT_STREQ(tiingo.m_strSECFilingWebSite, _T(""));
    EXPECT_EQ(tiingo.m_lStatementUpdateDate, 19800101);
    EXPECT_EQ(tiingo.m_lDailyDataUpdateDate, 19800101);
  }
}