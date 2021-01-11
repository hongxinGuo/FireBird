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
      EXPECT_FALSE(stake.CheckDayLineUpdateStatus(0, 0, 0, i)) << "�ǻ�Ծ��Ʊ�����ղ���������\n";
    }
  }

  TEST_F(CAmericaStockTest, TestCheckCheckDayLineUpdateStatus2) {
    CAmericaStake stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_NULL__;
    EXPECT_FALSE(stake.CheckDayLineUpdateStatus(0, 0, 0, 0)) << "��Ч��Ʊ���������\n";
  }

  TEST_F(CAmericaStockTest, TestCheckCheckDayLineUpdateStatus3) {
    CAmericaStake stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_DELISTED__;
    for (int i = 0; i < 6; i++) {
      EXPECT_FALSE(stake.CheckDayLineUpdateStatus(0, 0, 0, i)) << "ժ�ƹ�Ʊֻ���������������\n";
      stake.m_fDayLineNeedUpdate = true;
    }
    EXPECT_TRUE(stake.CheckDayLineUpdateStatus(0, 0, 0, 6)) << "ժ�ƹ�Ʊֻ���������������\n";
  }

  TEST_F(CAmericaStockTest, TestCheckCheckDayLineUpdateStatus4) {
    CAmericaStake stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_IPOED__;
    stake.m_fIsActive = true;
    stake.m_lDayLineEndDate = gl_pAmericaMarket->GetPrevDay(gl_pAmericaMarket->GetFormatedMarketDate(), 100);
    EXPECT_TRUE(stake.CheckDayLineUpdateStatus(gl_pAmericaMarket->GetFormatedMarketDate(), 20210108, 0, 1));
    stake.m_lDayLineEndDate = gl_pAmericaMarket->GetPrevDay(stake.m_lDayLineEndDate);
    EXPECT_FALSE(stake.CheckDayLineUpdateStatus(gl_pAmericaMarket->GetFormatedMarketDate(), 20210108, 0, 1)) << "����100��Ĺ�Ʊ���ٸ�������";
  }

  TEST_F(CAmericaStockTest, TestCheckCheckDayLineUpdateStatus5) {
    CAmericaStake stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_IPOED__;
    stake.m_fIsActive = true;
    stake.m_lDayLineEndDate = 20210108; // ���ս����������ѽ���
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stake.CheckDayLineUpdateStatus(20210108, 20210107, 170001, i)) << "ʱ������17ʱ�󣬼�鵱������";
      stake.m_fDayLineNeedUpdate = true; // ����״̬
    }
    stake.m_lDayLineEndDate = 20210107; // ���ս�����������δ����
    for (int i = 1; i < 6; i++) {
      EXPECT_TRUE(stake.CheckDayLineUpdateStatus(20210108, 20210107, 170001, i)) << "ʱ������17ʱ�󣬼�鵱������";
    }
  }

  TEST_F(CAmericaStockTest, TestCheckCheckDayLineUpdateStatus6) {
    CAmericaStake stake;

    stake.m_fDayLineNeedUpdate = true;
    stake.m_lIPOStatus = __STAKE_IPOED__;
    stake.m_fIsActive = true;
    stake.m_lDayLineEndDate = 20210107; // ��һ���������������ѽ���
    for (int i = 1; i < 6; i++) {
      EXPECT_FALSE(stake.CheckDayLineUpdateStatus(20210108, 20210107, 170000, i)) << "ʱ�䲻����17ʱ�������һ����������";
      stake.m_fDayLineNeedUpdate = true; // ����֮
    }
    stake.m_lDayLineEndDate = 20210106; // ��һ��������������δ����
    for (int i = 1; i < 6; i++) {
      EXPECT_TRUE(stake.CheckDayLineUpdateStatus(20210108, 20210107, 170000, i)) << "ʱ�䲻����17ʱ�������һ����������";
    }
  }
}