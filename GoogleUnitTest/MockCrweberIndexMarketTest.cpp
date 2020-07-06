#include"pch.h"

#include"globedef.h"
#include"ChinaStock.h"
#include"MockCrweberIndexMarket.h"
#include"WebInquirer.h"
#include"Thread.h"

// CVirtualMarket无法生成实例，故而其函数的测试放在这里。
static CMockCrweberIndexMarket s_CrweberIndexMarket;

namespace StockAnalysisTest {
  class CMockCrweberIndexMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
      while (gl_ThreadStatus.IsWorkingThreadRunning()) Sleep(1);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      s_CrweberIndexMarket.SetDatabaseLoaded(false);
      s_CrweberIndexMarket.SetPermitResetMarket(true);
      s_CrweberIndexMarket.SetReadyToRun(true);
      s_CrweberIndexMarket.SetResetMarket(true);
      s_CrweberIndexMarket.SetNewestUpdateDay(0);
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
      s_CrweberIndexMarket.SetDatabaseLoaded(false);
      s_CrweberIndexMarket.SetPermitResetMarket(true);
      s_CrweberIndexMarket.SetReadyToRun(true);
      s_CrweberIndexMarket.SetResetMarket(true);
      s_CrweberIndexMarket.SetNewestUpdateDay(0);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }
  };

  TEST_F(CMockCrweberIndexMarketTest, TestThreadMaintainCrweberDB) {
    EXPECT_CALL(s_CrweberIndexMarket, LoadDatabase)
      .Times(1);
    EXPECT_CALL(s_CrweberIndexMarket, SaveDatabase)
      .Times(1);
    EXPECT_EQ(ThreadMaintainCrweberDB(&s_CrweberIndexMarket), (UINT)24);
  }

  TEST_F(CMockCrweberIndexMarketTest, TestThreadSaveCrweberDB) {
    CCrweberIndexPtr pIndex = make_shared<CCrweberIndex>();

    EXPECT_CALL(s_CrweberIndexMarket, SaveCrweberIndexData(pIndex))
      .Times(1);
    EXPECT_EQ(ThreadSaveCrweberDB(&s_CrweberIndexMarket, pIndex), (UINT)23);
  }
}