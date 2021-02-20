#include"pch.h"

#include"globedef.h"
#include"ChinaStock.h"
#include"MockCrweberIndexMarket.h"
#include"WebInquirer.h"
#include"Thread.h"

static CMockCrweberIndexMarket* s_pCrweberIndexMarket;

namespace StockAnalysisTest {
  class CMockCrweberIndexMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      s_pCrweberIndexMarket = new CMockCrweberIndexMarket;
    }

    static void TearDownTestSuite(void) {
      delete s_pCrweberIndexMarket;
      while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      s_pCrweberIndexMarket->SetDatabaseLoaded(false);
      s_pCrweberIndexMarket->SetPermitResetMarket(true);
      s_pCrweberIndexMarket->SetReadyToRun(true);
      s_pCrweberIndexMarket->SetResetMarket(true);
      s_pCrweberIndexMarket->SetNewestUpdateDate(0);
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
      s_pCrweberIndexMarket->SetDatabaseLoaded(false);
      s_pCrweberIndexMarket->SetPermitResetMarket(true);
      s_pCrweberIndexMarket->SetReadyToRun(true);
      s_pCrweberIndexMarket->SetResetMarket(true);
      s_pCrweberIndexMarket->SetNewestUpdateDate(0);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }
  };

  TEST_F(CMockCrweberIndexMarketTest, TestThreadMaintainCrweberDB) {
    EXPECT_CALL(*s_pCrweberIndexMarket, LoadDatabase)
      .Times(1);
    EXPECT_CALL(*s_pCrweberIndexMarket, SaveDatabase)
      .Times(1);
    EXPECT_EQ(ThreadMaintainCrweberDB(s_pCrweberIndexMarket), (UINT)24);
  }

  TEST_F(CMockCrweberIndexMarketTest, TestThreadSaveCrweberDB) {
    CCrweberIndexPtr pIndex = make_shared<CCrweberIndex>();

    EXPECT_CALL(*s_pCrweberIndexMarket, SaveCrweberIndexData(pIndex))
      .Times(1);
    EXPECT_EQ(ThreadSaveCrweberDB(s_pCrweberIndexMarket, pIndex), (UINT)23);
  }
}