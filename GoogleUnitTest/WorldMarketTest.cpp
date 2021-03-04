#include"pch.h"
#include"globedef.h"

#include"WorldStock.h"
#include"WorldMarket.h"

using namespace testing;

namespace StockAnalysisTest {
  class CWorldMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }
    static void TearDownTestSuite(void) {
    }
    virtual void SetUp(void) override {
    }

    virtual void TearDown(void) override {
      // clearu
    }

  protected:
  };

  TEST_F(CWorldMarketTest, TestIsFinnhubInquiring) {
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubInquiring());
    gl_pWorldMarket->SetFinnhubInquiring(true);
    EXPECT_TRUE(gl_pWorldMarket->IsFinnhubInquiring());
    gl_pWorldMarket->SetFinnhubInquiring(false);
    EXPECT_FALSE(gl_pWorldMarket->IsFinnhubInquiring());
  }

  TEST_F(CWorldMarketTest, TestGetTotalStock) {
    EXPECT_EQ(gl_pWorldMarket->GetTotalStock(), 2462) << "默认状态下数据库总数为2462";
  }

  TEST_F(CWorldMarketTest, TestGetStock) {
    CWorldStockPtr pStock = gl_pWorldMarket->GetStock(0); // 000001.SS
    EXPECT_STREQ(pStock->GetSymbol(), _T("000001.SS")) << "第一个股票代码为000001.SS";
    pStock = gl_pWorldMarket->GetStock(_T("000001.SS"));
    EXPECT_FALSE(pStock == nullptr);
    EXPECT_STREQ(pStock->GetDescription(), _T("SSE Composite Index"));
  }
}