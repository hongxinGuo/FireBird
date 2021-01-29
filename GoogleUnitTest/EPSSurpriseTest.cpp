#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"EPSSurprise.h"

namespace StockAnalysisTest {
  class CEPSSurpriseTest : public ::testing::Test
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

  TEST_F(CEPSSurpriseTest, TestInitialize) {
    CEPSSurprise EPSSurprise;
    EXPECT_STREQ(EPSSurprise.m_strSymbol, _T(" "));
    EXPECT_EQ(EPSSurprise.m_lDate, 0);
    EXPECT_DOUBLE_EQ(EPSSurprise.m_dActual, 0.0);
    EXPECT_DOUBLE_EQ(EPSSurprise.m_dEstimate, 0.0);
  }

  TEST_F(CEPSSurpriseTest, TestAppend) {
    CSetEPSSurprise setEPSSurprise, setEPSSurprise2;
    CEPSSurprise EPSSurprise;

    EPSSurprise.m_strSymbol = _T("AAAAA");
    EPSSurprise.m_dActual = 1.2;

    ASSERT(!gl_fNormalMode);
    setEPSSurprise.Open();
    setEPSSurprise.m_pDatabase->BeginTrans();
    EPSSurprise.Append(setEPSSurprise);
    setEPSSurprise.m_pDatabase->CommitTrans();
    setEPSSurprise.Close();

    setEPSSurprise2.m_strFilter = _T("[Symbol] = 'AAAAA'");
    setEPSSurprise2.Open();
    EXPECT_TRUE(!setEPSSurprise2.IsEOF()) << "此时已经存入了ABCDEF";
    EXPECT_DOUBLE_EQ(setEPSSurprise2.m_Actual, 1.2);
    setEPSSurprise2.Delete();
    setEPSSurprise2.Close();
  }
}