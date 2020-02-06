#include"pch.h"

#include"globedef.h"
#include"ChinaStock.h"
#include"CrweberIndexMarket.h"

// CVirtualMarket无法生成实例，故而其函数的测试放在这里。
namespace StockAnalysisTest {
  class CCrweberIndexMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestCase() { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestCase() {
      gl_CrweberIndexMarket.SetDatabaseLoaded(false);
      gl_CrweberIndexMarket.SetPermitResetMarket(true);
      gl_CrweberIndexMarket.SetReadyToRun(true);
      gl_CrweberIndexMarket.SetResetMarket(true);
      gl_CrweberIndexMarket.SetNewestUpdateDay(0);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_CrweberIndexMarket.IsDatabaseLoaded());
      EXPECT_EQ(gl_CrweberIndexMarket.GetNewestUpdateDay(), 0);
      EXPECT_TRUE(gl_CrweberIndexMarket.IsPermitResetMarket());
      EXPECT_TRUE(gl_CrweberIndexMarket.IsReadyToRun());
      EXPECT_TRUE(gl_CrweberIndexMarket.IsResetMarket());
    }

    virtual void TearDown(void) override {
      // clearup
      gl_CrweberIndexMarket.SetDatabaseLoaded(false);
      gl_CrweberIndexMarket.SetPermitResetMarket(true);
      gl_CrweberIndexMarket.SetReadyToRun(true);
      gl_CrweberIndexMarket.SetResetMarket(true);
      gl_CrweberIndexMarket.SetNewestUpdateDay(0);
    }
  };

  TEST_F(CCrweberIndexMarketTest, TestInitialize) {
    EXPECT_FALSE(gl_CrweberIndexMarket.IsDatabaseLoaded());
    EXPECT_EQ(gl_CrweberIndexMarket.GetNewestUpdateDay(), 0);
    EXPECT_TRUE(gl_CrweberIndexMarket.IsPermitResetMarket());
  }

  TEST_F(CCrweberIndexMarketTest, TestSetNewestUpdateDay) {
    EXPECT_EQ(gl_CrweberIndexMarket.GetNewestUpdateDay(), 0);
    gl_CrweberIndexMarket.SetNewestUpdateDay(20201220);
    EXPECT_EQ(gl_CrweberIndexMarket.GetNewestUpdateDay(), 20201220);
  }

  TEST_F(CCrweberIndexMarketTest, TestSetDatabaseLoaded) {
    EXPECT_FALSE(gl_CrweberIndexMarket.IsDatabaseLoaded());
    gl_CrweberIndexMarket.SetDatabaseLoaded(true);
    EXPECT_TRUE(gl_CrweberIndexMarket.IsDatabaseLoaded());
    gl_CrweberIndexMarket.SetDatabaseLoaded(false);
    EXPECT_FALSE(gl_CrweberIndexMarket.IsDatabaseLoaded());
  }
}