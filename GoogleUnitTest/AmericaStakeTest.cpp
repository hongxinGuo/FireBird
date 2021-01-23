#include"stdafx.h"

#include"pch.h"
#include"globedef.h"

#include"WorldStock.h"
#include"WorldMarket.h"

using namespace testing;

namespace StockAnalysisTest {
  class CWorldStockTest : public ::testing::Test
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
    CWorldStockPtr pStake;
  };

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus1) {
    CWorldStock stake;

    stake.m_fDayLineNeedUpdate = true;

    stake.m_fIsActive = false;
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stake.CheckDayLineUpdateStatus(0, 0, 0, i)) << "非活跃股票工作日不更新日线\n";
    }
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus2) {
    CWorldStock stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_NULL__;
    EXPECT_FALSE(stake.CheckDayLineUpdateStatus(0, 0, 0, 0)) << "无效股票不检查日线\n";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus3) {
    CWorldStock stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_DELISTED__;
    for (int i = 0; i < 6; i++) {
      EXPECT_FALSE(stake.CheckDayLineUpdateStatus(0, 0, 0, i)) << "摘牌股票只在星期六检查日线\n";
      stake.m_fDayLineNeedUpdate = true;
    }
    EXPECT_TRUE(stake.CheckDayLineUpdateStatus(0, 0, 0, 6)) << "摘牌股票只在星期六检查日线\n";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus4) {
    CWorldStock stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_IPOED__;
    stake.m_fIsActive = true;
    stake.m_lDayLineEndDate = gl_pWorldMarket->GetPrevDay(gl_pWorldMarket->GetFormatedMarketDate(), 100);
    EXPECT_TRUE(stake.CheckDayLineUpdateStatus(gl_pWorldMarket->GetFormatedMarketDate(), 20210108, 0, 1));
    stake.m_lDayLineEndDate = gl_pWorldMarket->GetPrevDay(stake.m_lDayLineEndDate);
    EXPECT_FALSE(stake.CheckDayLineUpdateStatus(gl_pWorldMarket->GetFormatedMarketDate(), 20210108, 0, 1)) << "早于100天的股票不再更新日线";
  }

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus5) {
    CWorldStock stake;

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

  TEST_F(CWorldStockTest, TestCheckCheckDayLineUpdateStatus6) {
    CWorldStock stake;

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

  TEST_F(CWorldStockTest, TestSaveDayLine) {
    CWorldStock stake;
    vector<CDayLinePtr> vDayLine;
    CDayLinePtr pDayLine;
    CSetWorldStockDayLine setDayLine;

    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStakeCode(_T("A"));
    pDayLine->SetDate(20200101); // 这个需要添加进数据库
    pDayLine->SetClose(10010);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStakeCode(_T("A"));
    pDayLine->SetDate(20210101); // 这个需要添加进数据库
    pDayLine->SetClose(12345);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStakeCode(_T("A"));
    pDayLine->SetDate(20210107); // 这个数据库中有，无需添加
    pDayLine->SetClose(10020);
    vDayLine.push_back(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetStakeCode(_T("A"));
    pDayLine->SetDate(20210123); // 这个需要添加进数据库
    pDayLine->SetClose(10030);
    vDayLine.push_back(pDayLine);

    stake.m_strSymbol = _T("A");
    stake.m_lDayLineEndDate = 20210107;
    stake.UpdateDayLine(vDayLine);

    stake.SaveDayLine();

    setDayLine.m_strFilter = _T("[Symbol] = 'A'");
    setDayLine.m_strSort = _T("[Date]");
    setDayLine.Open();
    setDayLine.m_pDatabase->BeginTrans();
    EXPECT_TRUE(setDayLine.m_Date == 20200101);
    EXPECT_STREQ(setDayLine.m_Close, _T("10.010"));
    setDayLine.Delete();
    while (setDayLine.m_Date != 20210101) setDayLine.MoveNext();
    EXPECT_STREQ(setDayLine.m_Close, _T("12.345"));
    setDayLine.Delete();
    setDayLine.MoveLast();
    EXPECT_TRUE(setDayLine.m_Date = 20210123);
    EXPECT_STREQ(setDayLine.m_Close, _T("10.030"));
    setDayLine.Delete();
    setDayLine.m_pDatabase->CommitTrans();
    setDayLine.Close();
  }
}