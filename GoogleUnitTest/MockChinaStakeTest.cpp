#include"stdafx.h"

#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include"ChinaStake.h"
#include"ChinaMarket.h"

#include"MockChinaStake.h"

using namespace testing;

namespace StockAnalysisTest {
  static CSinaRTWebInquiry m_SinaRTWebInquiry; // 新浪实时数据采集
  static CTengxunRTWebInquiry m_TengxunRTWebData; // 腾讯实时数据采集
  static CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiry; // 网易日线历史数据

  class CMockChinaStockTest : public ::testing::Test
  {
  protected:
    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_pChinaStakeMarket->GetCurrentStock(), nullptr) << gl_pChinaStakeMarket->GetCurrentStock()->GetStakeCode();
    }
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      pStake = make_shared<CMockChinaStake>();
      gl_pChinaStakeMarket->CalculateTime();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pChinaStakeMarket->SetDayLineNeedUpdateNumber(gl_pChinaStakeMarket->GetTotalStock());
      gl_pChinaStakeMarket->CalculateTime();
      gl_pChinaStakeMarket->SetUpdateStockCodeDB(false);
      gl_pChinaStakeMarket->SetUpdateOptionDB(false);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

  protected:
    CMockChinaStakePtr pStake;
    CMockChinaStake stock;
  };

  TEST_F(CMockChinaStockTest, TestShowCurrentTransaction) {
    EXPECT_CALL(*pStake, ReportGuadanTransaction())
      .Times(0);
    pStake->ShowCurrentTransaction();
    EXPECT_EQ(pStake->GetCurrentTransationVolume(), 0);
    pStake->SetCurrentTransationVolume(1);
    EXPECT_CALL(*pStake, ReportGuadanTransaction())
      .Times(1);
    pStake->ShowCurrentTransaction();
  }

  TEST_F(CMockChinaStockTest, TestShowCurrentInformationOfCancelingGuadan) {
    EXPECT_CALL(*pStake, ReportGuadan())
      .Times(1);
    pStake->ShowCurrentInformationOfCancelingGuadan();
  }

  TEST_F(CMockChinaStockTest, TestThreadSaveDayLineOfOneStock) {
    EXPECT_CALL(*pStake, SaveDayLineBasicInfo)
      .Times(0);
    pStake->SetDayLineLoaded(true);
    pStake->SetStakeCode(_T("sh601111"));
    gl_fExitingSystem = true;
    EXPECT_EQ(ThreadSaveDayLineBasicInfoOfStock(pStake), (UINT)15);
    EXPECT_TRUE(pStake->IsDayLineLoaded());
    EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 0);

    EXPECT_CALL(*pStake, SaveDayLineBasicInfo)
      .Times(1)
      .WillOnce(Return(false));
    pStake->SetDayLineLoaded(true);
    pStake->SetStakeCode(_T("sh601111"));
    gl_fExitingSystem = false;
    EXPECT_EQ(ThreadSaveDayLineBasicInfoOfStock(pStake), (UINT)15);
    EXPECT_FALSE(pStake->IsDayLineLoaded()) << "存储时不涉及卸载日线数据\n";
    EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 0);

    EXPECT_CALL(*pStake, SaveDayLineBasicInfo)
      .Times(1)
      .WillOnce(Return(true));
    pStake->SetDayLineLoaded(true);
    pStake->SetStakeCode(_T("sh601111"));
    gl_fExitingSystem = false;
    EXPECT_EQ(ThreadSaveDayLineBasicInfoOfStock(pStake), (UINT)15);
    EXPECT_FALSE(pStake->IsDayLineLoaded()) << "存储时不涉及卸载日线数据\n";
    EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 1);
    CString str = gl_systemMessage.PopDayLineInfoMessage();
    EXPECT_STREQ(str, _T("sh601111日线资料存储完成"));
  }

  TEST_F(CMockChinaStockTest, TestThreadLoadDayLine) {
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pStake->StoreDayLine(pDayLine);
    InSequence seq;
    EXPECT_CALL(*pStake, LoadDayLine)
      .Times(1);
    pStake->SetDayLineLoaded(false);
    EXPECT_EQ(ThreadLoadDayLine(pStake), (UINT)16);
    EXPECT_TRUE(pStake->IsDayLineLoaded());
    EXPECT_EQ(pStake->GetDayLineSize(), 0) << _T("存储日线数据后清空队列\n");
  }
  TEST_F(CMockChinaStockTest, TestThreadLoadWeekLine) {
    CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
    pStake->StoreWeekLine(pWeekLine);
    EXPECT_CALL(*pStake, LoadWeekLine)
      .Times(1);
    pStake->SetWeekLineLoaded(false);
    EXPECT_EQ(ThreadLoadWeekLine(pStake), (UINT)29);
    EXPECT_TRUE(pStake->IsWeekLineLoaded());
    EXPECT_EQ(pStake->GetWeekLineSize(), 0) << _T("存储周线数据后清空队列\n");
  }
}