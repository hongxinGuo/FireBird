#include"pch.h"
#include"globedef.h"

#include"StockSection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
  class CStockSectionTest : public ::testing::Test
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

  TEST_F(CStockSectionTest, TestIsActive) {
    CStockSection stockSection;

    EXPECT_FALSE(stockSection.IsActive());
    stockSection.SetActive(true);
    EXPECT_TRUE(stockSection.IsActive());
    stockSection.SetActive(false);
    EXPECT_FALSE(stockSection.IsActive());
  }

  TEST_F(CStockSectionTest, TestGetIndexNumber) {
    CStockSection stockSection;

    EXPECT_EQ(stockSection.GetIndexNumber(), 0);
    stockSection.SetIndexNumber(10101010);
    EXPECT_EQ(stockSection.GetIndexNumber(), 10101010);
  }

  TEST_F(CStockSectionTest, TestGetMarket) {
    CStockSection stockSection;

    EXPECT_EQ(stockSection.GetMarket(), 0);
    stockSection.SetMarket(1010);
    EXPECT_EQ(stockSection.GetMarket(), 1010);
  }

  TEST_F(CStockSectionTest, TestGetComment) {
    CStockSection stockSection;

    EXPECT_STREQ(stockSection.GetComment(), _T(""));
    stockSection.SetComment(_T("abc"));
    EXPECT_STREQ(stockSection.GetComment(), _T("abc"));
  }

  TEST_F(CStockSectionTest, TestIsBuildStockPtr) {
    CStockSection stockSection;

    EXPECT_FALSE(stockSection.IsBuildStockPtr());
    stockSection.SetBuildStockPtr(true);
    EXPECT_TRUE(stockSection.IsBuildStockPtr());
    stockSection.SetBuildStockPtr(false);
    EXPECT_FALSE(stockSection.IsBuildStockPtr());
  }
}