#include"stdafx.h"

#include"pch.h"
#include"globedef.h"

#include"AmericaStake.h"
#include"AmericaMarket.h"

using namespace testing;

namespace StockAnalysisTest {
  class CAmericaStockTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }
    static void TearDownTestSuite(void) {
    }
    virtual void SetUp(void) override {
      pStake = nullptr;
    }

    virtual void TearDown(void) override {
      // clearu
    }

  protected:
    CAmericaStakePtr pStake;
  };

  TEST_F(CAmericaStockTest, TestCheckCheckDayLineUpdateStatus1) {
    CAmericaStake stake;

    stake.m_fDayLineNeedUpdate = true;

    stake.m_fIsActive = false;
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stake.CheckDayLineUpdateStatus(0, 0, 0, i)) << "非活跃股票工作日不更新日线\n";
    }
  }

  TEST_F(CAmericaStockTest, TestCheckCheckDayLineUpdateStatus2) {
    CAmericaStake stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_NULL__;
    EXPECT_FALSE(stake.CheckDayLineUpdateStatus(0, 0, 0, 0)) << "无效股票不检查日线\n";
  }

  TEST_F(CAmericaStockTest, TestCheckCheckDayLineUpdateStatus3) {
    CAmericaStake stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_DELISTED__;
    for (int i = 0; i < 6; i++) {
      EXPECT_FALSE(stake.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期六检查日线\n";
      stake.m_fDayLineNeedUpdate = true;
    }
    EXPECT_TRUE(stake.CheckDayLineUpdateStatus(0, 0, 0, 6)) << "摘牌股票只在星期六检查日线\n";
  }

  TEST_F(CAmericaStockTest, TestCheckCheckDayLineUpdateStatus4) {
    CAmericaStake stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_IPOED__;
    stake.m_fIsActive = true;
    stake.m_lDayLineEndDate = gl_pAmericaMarket->GetPrevDay(gl_pAmericaMarket->GetFormatedMarketDate(), 100);
    EXPECT_TRUE(stake.CheckDayLineUpdateStatus(gl_pAmericaMarket->GetFormatedMarketDate(), 20210108, 0, 1));
    stake.m_lDayLineEndDate = gl_pAmericaMarket->GetPrevDay(stake.m_lDayLineEndDate);
    EXPECT_FALSE(stake.CheckDayLineUpdateStatus(gl_pAmericaMarket->GetFormatedMarketDate(), 20210108, 0, 1)) << "早于100天的股票不再更新日线";
  }

  TEST_F(CAmericaStockTest, TestCheckCheckDayLineUpdateStatus5) {
    CAmericaStake stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_IPOED__;
    stake.m_fIsActive = true;
    stake.m_lDayLineEndDate = 20210108; // 本日交易日日线已接收
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stake.CheckDayLineUpdateStatus(20210108, 20210107, 170001, i)) << "时间晚于17时后，检查当天日线";
      stake.m_fDayLineNeedUpdate = true; // 重置状态
    }
    stake.m_lDayLineEndDate = 20210107; // 本日交易日日线尚未接收
    for (int i = 1; i < 6; i++) {
      EXPECT_TRUE(stake.CheckDayLineUpdateStatus(20210108, 20210107, 170001, i)) << "时间晚于17时后，检查当天日线";
    }
  }

  TEST_F(CAmericaStockTest, TestCheckCheckDayLineUpdateStatus6) {
    CAmericaStake stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_IPOED__;
    stake.m_fIsActive = true;
    stake.m_lDayLineEndDate = 20210107; // 上一交易日日线数据已接收
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stake.CheckDayLineUpdateStatus(20210108, 20210107, 170000, i)) << "时间不晚于17时，检查上一交易日日线";
      stake.m_fDayLineNeedUpdate = true; // 重置之
    }
    stake.m_lDayLineEndDate = 20210106; // 上一交易日日线数据未接收
    for (int i = 1; i < 6; i++) {
      EXPECT_TRUE(stake.CheckDayLineUpdateStatus(20210108, 20210107, 170000, i)) << "时间不晚于17时，检查上一交易日日线";
    }
  }
}