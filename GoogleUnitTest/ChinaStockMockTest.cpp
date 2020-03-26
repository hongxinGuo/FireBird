#include"stdafx.h"

#include"pch.h"
#include"globedef.h"

#include"ChinaStock.h"
#include"ChinaMarket.h"

#include"MockChinaStock.h"

using namespace Testing;

namespace StockAnalysisTest {
  static CSinaRTWebInquiry m_SinaRTWebInquiry; // 新浪实时数据采集
  static CTengxunRTWebInquiry m_TengxunRTWebData; // 腾讯实时数据采集
  static CNeteaseDayLineWebInquiry m_NeteaseDayLineWebInquiry; // 网易日线历史数据

  class CChinaStockMockTest : public ::testing::Test
  {
  protected:
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
      gl_pChinaStockMarket->CalculateTime();
      gl_pChinaStockMarket->SetUpdateStockCodeDB(false);
      gl_pChinaStockMarket->SetUpdateOptionDB(false);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

  protected:
    CMockChinaStockPtr pStock;
  };

  TEST_F(CChinaStockMockTest, TestCalculateDayLineRS) {
    EXPECT_CALL(*pStock, CalculateDayLineRS(3))
      .Times(1);
    EXPECT_CALL(*pStock, CalculateDayLineRS(5))
      .Times(1);
    EXPECT_CALL(*pStock, CalculateDayLineRS(10))
      .Times(1);
    EXPECT_CALL(*pStock, CalculateDayLineRS(30))
      .Times(1);
    EXPECT_CALL(*pStock, CalculateDayLineRS(60))
      .Times(1);
    EXPECT_CALL(*pStock, CalculateDayLineRS(120))
      .Times(1);
    pStock->CalculateDayLineRelativeStrong();
  }

  TEST_F(CChinaStockMockTest, TestCalculateDayLineRSLogarithm) {
    EXPECT_CALL(*pStock, CalculateDayLineRSLogarithm(3))
      .Times(1);
    EXPECT_CALL(*pStock, CalculateDayLineRSLogarithm(5))
      .Times(1);
    EXPECT_CALL(*pStock, CalculateDayLineRSLogarithm(10))
      .Times(1);
    EXPECT_CALL(*pStock, CalculateDayLineRSLogarithm(30))
      .Times(1);
    EXPECT_CALL(*pStock, CalculateDayLineRSLogarithm(60))
      .Times(1);
    EXPECT_CALL(*pStock, CalculateDayLineRSLogarithm(120))
      .Times(1);
    pStock->CalculateDayLineRelativeStrongLogarithm();
  }

  TEST_F(CChinaStockMockTest, TestShowCurrentTransaction) {
    EXPECT_CALL(*pStock, ReportGuadanTransaction())
      .Times(0);
    pStock->ShowCurrentTransaction();
    EXPECT_EQ(pStock->GetCurrentTransationVolume(), 0);
    pStock->SetCurrentTransationVolume(1);
    EXPECT_CALL(*pStock, ReportGuadanTransaction())
      .Times(1);
    pStock->ShowCurrentTransaction();
  }
}