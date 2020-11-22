#include"stdafx.h"

#include"pch.h"
#include"globedef.h"
#include"Thread.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

#include"MockChinaStock.h"

using namespace testing;

namespace StockAnalysisTest {
  static CSinaRTWebInquiry m_SinaRTWebInquiry; // 新浪实时数据采集
  static CTengxunRTWebInquiry m_TengxunRTWebData; // 腾讯实时数据采集
  static CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiry; // 网易日线历史数据

  class CMockChinaStockTest : public ::testing::Test
  {
  protected:
    static void TearDownTestSuite(void) {
      EXPECT_EQ(gl_pChinaStockMarket->GetCurrentStock(), nullptr) << gl_pChinaStockMarket->GetCurrentStock()->GetStakeCode();
    }
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      pStock = make_shared<CMockChinaStock>();
      gl_pChinaStockMarket->CalculateTime();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pChinaStockMarket->SetDayLineNeedUpdateNumber(12000);
      gl_pChinaStockMarket->CalculateTime();
      gl_pChinaStockMarket->SetUpdateStockCodeDB(false);
      gl_pChinaStockMarket->SetUpdateOptionDB(false);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

  protected:
    CMockChinaStockPtr pStock;
    CMockChinaStock stock;
  };

  TEST_F(CMockChinaStockTest, TestShowCurrentTransaction) {
    EXPECT_CALL(*pStock, ReportGuadanTransaction())
      .Times(0);
    pStock->ShowCurrentTransaction();
    EXPECT_EQ(pStock->GetCurrentTransationVolume(), 0);
    pStock->SetCurrentTransationVolume(1);
    EXPECT_CALL(*pStock, ReportGuadanTransaction())
      .Times(1);
    pStock->ShowCurrentTransaction();
  }

  TEST_F(CMockChinaStockTest, TestShowCurrentInformationOfCancelingGuadan) {
    EXPECT_CALL(*pStock, ReportGuadan())
      .Times(1);
    pStock->ShowCurrentInformationOfCancelingGuadan();
  }

  TEST_F(CMockChinaStockTest, TestThreadSaveDayLineOfOneStock) {
    EXPECT_CALL(*pStock, SaveDayLineBasicInfo)
      .Times(0);
    pStock->SetDayLineLoaded(true);
    pStock->SetStakeCode(_T("sh601111"));
    gl_fExitingSystem = true;
    EXPECT_EQ(ThreadSaveDayLineBasicInfoOfStock(pStock), (UINT)15);
    EXPECT_TRUE(pStock->IsDayLineLoaded());
    EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 0);

    EXPECT_CALL(*pStock, SaveDayLineBasicInfo)
      .Times(1)
      .WillOnce(Return(false));
    pStock->SetDayLineLoaded(true);
    pStock->SetStakeCode(_T("sh601111"));
    gl_fExitingSystem = false;
    EXPECT_EQ(ThreadSaveDayLineBasicInfoOfStock(pStock), (UINT)15);
    EXPECT_FALSE(pStock->IsDayLineLoaded()) << "存储时不涉及卸载日线数据\n";
    EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 0);

    EXPECT_CALL(*pStock, SaveDayLineBasicInfo)
      .Times(1)
      .WillOnce(Return(true));
    pStock->SetDayLineLoaded(true);
    pStock->SetStakeCode(_T("sh601111"));
    gl_fExitingSystem = false;
    EXPECT_EQ(ThreadSaveDayLineBasicInfoOfStock(pStock), (UINT)15);
    EXPECT_FALSE(pStock->IsDayLineLoaded()) << "存储时不涉及卸载日线数据\n";
    EXPECT_EQ(gl_systemMessage.GetDayLineInfoDequeSize(), 1);
    CString str = gl_systemMessage.PopDayLineInfoMessage();
    EXPECT_STREQ(str, _T("sh601111日线资料存储完成"));
  }

  TEST_F(CMockChinaStockTest, TestThreadLoadDayLine) {
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pStock->StoreDayLine(pDayLine);
    InSequence seq;
    EXPECT_CALL(*pStock, LoadDayLine)
      .Times(1);
    pStock->SetDayLineLoaded(false);
    EXPECT_EQ(ThreadLoadDayLine(pStock), (UINT)16);
    EXPECT_TRUE(pStock->IsDayLineLoaded());
    EXPECT_EQ(pStock->GetDayLineSize(), 0) << _T("存储日线数据后清空队列\n");
  }
  TEST_F(CMockChinaStockTest, TestThreadLoadWeekLine) {
    CWeekLinePtr pWeekLine = make_shared<CWeekLine>();
    pStock->StoreWeekLine(pWeekLine);
    EXPECT_CALL(*pStock, LoadWeekLine)
      .Times(1);
    pStock->SetWeekLineLoaded(false);
    EXPECT_EQ(ThreadLoadWeekLine(pStock), (UINT)29);
    EXPECT_TRUE(pStock->IsWeekLineLoaded());
    EXPECT_EQ(pStock->GetWeekLineSize(), 0) << _T("存储周线数据后清空队列\n");
  }
}