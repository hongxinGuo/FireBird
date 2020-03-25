///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ChinaMarket.h"
#include"ChinaStock.h"

#include"SetStockCode.h"
#include"WebInquirer.h"

#include"MockNeteaseDayLineWebInquiry.h"
using namespace Testing;
using namespace testing;

using namespace std;
#include<memory>

namespace StockAnalysisTest {
  class CChinaMarketMockTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      ASSERT_FALSE(gl_fNormalMode);
    }
    static void TearDownTestSuite(void) {
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
      gl_pChinaStockMarket->SetDayLineNeedSaveNumber(0);
    }
    virtual void SetUp(void) override {
      chinaMarket.CalculateTime();
      chinaMarket.ResetNeteaseRTDataInquiringIndex();
      chinaMarket.ResetNeteaseDayLineDataInquiringIndex();
      chinaMarket.ResetSinaRTDataInquiringIndex();
      chinaMarket.ResetTengxunRTDataInquiringIndex();
      chinaMarket.SetSystemReady(true); // 测试市场时，默认系统已经准备好
      chinaMarket.SetPermitResetMarket(true);
      chinaMarket.SetCheckActiveStock(true);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_ThreadStatus.SetSavingTempData(false);

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    CMockChinaMarket chinaMarket;
  };

  TEST_F(CChinaMarketMockTest, TestSaveDayLineData1) {
    CChinaStockPtr pStock = chinaMarket.GetStock(_T("sh600000"));
    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    EXPECT_CALL(chinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(0);
    chinaMarket.SaveDayLineData();
  }

  TEST_F(CChinaMarketMockTest, TestSaveDayLineData2) {
    CChinaStockPtr pStock = chinaMarket.GetStock(_T("sh600000"));

    pStock->SetDayLineNeedSaving(true);
    EXPECT_CALL(chinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(0);
    chinaMarket.SaveDayLineData();
  }

  TEST_F(CChinaMarketMockTest, TestSaveDayLineData3) {
    CChinaStockPtr pStock = chinaMarket.GetStock(_T("sh600000"));

    pStock->SetDayLineNeedSaving(true);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDay(19900101);
    pStock->SetDayLineEndDay(20000101);
    pStock->StoreDayLine(pDayLine);
    EXPECT_CALL(chinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(0);
    chinaMarket.SaveDayLineData();

    pStock->ClearDayLineContainer();
  }

  TEST_F(CChinaMarketMockTest, TestSaveDayLineData4) {
    CChinaStockPtr pStock = chinaMarket.GetStock(_T("sh600000"));

    pStock->SetDayLineNeedSaving(true);
    CDayLinePtr pDayLine = make_shared<CDayLine>();
    pDayLine->SetDay(19900101);
    pStock->SetDayLineEndDay(20000101);
    pStock->StoreDayLine(pDayLine);
    pDayLine = make_shared<CDayLine>();
    pDayLine->SetDay(chinaMarket.GetDay());
    pStock->StoreDayLine(pDayLine);
    pStock->SetDayLineNeedSaving(true);
    EXPECT_CALL(chinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(1);
    EXPECT_TRUE(chinaMarket.SaveDayLineData());
    EXPECT_EQ(gl_pChinaStockMarket->GetDayLineNeedSaveNumber(), 1);
    pStock->ClearDayLineContainer();
  }

  TEST_F(CChinaMarketMockTest, TestUpdateTempRTData) {
    gl_ThreadStatus.SetSavingTempData(true);
    EXPECT_FALSE(chinaMarket.UpdateTempRTData());
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    EXPECT_TRUE(chinaMarket.UpdateTempRTData());
    EXPECT_TRUE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(true);
  }

  TEST_F(CChinaMarketMockTest, TestTaskLoadCurrentStockDayLine1) {
    chinaMarket.ResetCurrentStock();
    EXPECT_CALL(chinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_TRUE(chinaMarket.TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketMockTest, TestTaskLoadCurrentStockDayLine2) {
    chinaMarket.ResetCurrentStock();
    chinaMarket.SetCurrentStock(_T("sh600000"));
    chinaMarket.GetCurrentStock()->SetDayLineLoaded(true);
    EXPECT_CALL(chinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_TRUE(chinaMarket.TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketMockTest, TestTaskLoadCurrentStockDayLine3) {
    chinaMarket.ResetCurrentStock();
    chinaMarket.SetCurrentStock(_T("sh600000"));
    chinaMarket.GetCurrentStock()->SetDayLineLoaded(false);
    EXPECT_CALL(chinaMarket, RunningThreadLoadDayLine(_))
      .Times(1);
    EXPECT_TRUE(chinaMarket.TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketMockTest, TestTaskUpdateStockCodeDB) {
    chinaMarket.SetUpdateStockCodeDB(false);
    EXPECT_CALL(chinaMarket, RunningThreadUpdateStockCodeDB())
      .Times(0);
    EXPECT_FALSE(chinaMarket.TaskUpdateStockCodeDB());
    chinaMarket.SetUpdateStockCodeDB(true);
    EXPECT_CALL(chinaMarket, RunningThreadUpdateStockCodeDB())
      .Times(1);
    EXPECT_TRUE(chinaMarket.TaskUpdateStockCodeDB());
    EXPECT_FALSE(chinaMarket.IsUpdateStockCodeDB());
  }

  TEST_F(CChinaMarketMockTest, TestTaskUpdateOptionDB) {
    chinaMarket.SetUpdateOptionDB(false);
    EXPECT_CALL(chinaMarket, RunningThreadUpdateOptionDB())
      .Times(0);
    EXPECT_FALSE(chinaMarket.TaskUpdateOptionDB());
    chinaMarket.SetUpdateOptionDB(true);
    EXPECT_CALL(chinaMarket, RunningThreadUpdateOptionDB())
      .Times(1);
    EXPECT_TRUE(chinaMarket.TaskUpdateOptionDB());
    EXPECT_FALSE(chinaMarket.IsUpdateOptionDB());
  }

  TEST_F(CChinaMarketMockTest, TestTaskSaveChoicedRTData) {
    chinaMarket.SetSystemReady(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveChoicedRTData())
      .Times(0);
    EXPECT_FALSE(chinaMarket.TaskSaveChoicedRTData());
    chinaMarket.SetSystemReady(true);
    chinaMarket.SetRecordRTData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveChoicedRTData())
      .Times(0);
    EXPECT_FALSE(chinaMarket.TaskSaveChoicedRTData());
    chinaMarket.SetRecordRTData(true);
    EXPECT_CALL(chinaMarket, RunningThreadSaveChoicedRTData())
      .Times(1);
    EXPECT_TRUE(chinaMarket.TaskSaveChoicedRTData());
    chinaMarket.SetRecordRTData(false);
    chinaMarket.SetSystemReady(false);
  }

  TEST_F(CChinaMarketMockTest, TestTaskSaveTempDataIntoDB) {
    chinaMarket.SetSystemReady(true);
    chinaMarket.SetMarketOpened(true);
    gl_ThreadStatus.SetCalculatingRTData(false);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    chinaMarket.TaskSaveTempDataIntoDB(93001);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    chinaMarket.TaskSaveTempDataIntoDB(113559);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    chinaMarket.TaskSaveTempDataIntoDB(130001);
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    chinaMarket.TaskSaveTempDataIntoDB(150559);
  }
}