#include"pch.h"

#include"globedef.h"
#include"MockChinaMarket.h"

#include"thread.h"

using namespace testing;

namespace StockAnalysisTest {
  class CThreadBuildDayLineRSOfDateTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
      EXPECT_FALSE(gl_fExitingSystem);
    }

    virtual void SetUp(void) override {
    }

    virtual void TearDown(void) override {
      gl_fExitingSystem = false;
    }
    CMockChinaMarket market;
  };

  TEST_F(CThreadBuildDayLineRSOfDateTest, TestThreadBuildDayLineRSOfDate) {
    EXPECT_CALL(market, BuildDayLineRSOfDate(20200101))
      .Times(1)
      .WillOnce(Return(true));
    gl_fExitingCalculatingRS = false;
    gl_fExitingSystem = false;
    EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), (UINT)12);

    EXPECT_CALL(market, BuildDayLineRSOfDate(20200101))
      .Times(0);
    gl_fExitingCalculatingRS = true;
    gl_fExitingSystem = false;
    EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), (UINT)12);

    EXPECT_CALL(market, BuildDayLineRSOfDate(20200101))
      .Times(0);
    gl_fExitingCalculatingRS = false;
    gl_fExitingSystem = true;
    EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), (UINT)12);

    EXPECT_CALL(market, BuildDayLineRSOfDate(20200101))
      .Times(0);
    gl_fExitingCalculatingRS = true;
    gl_fExitingSystem = true;
    EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), (UINT)12);
  }
}