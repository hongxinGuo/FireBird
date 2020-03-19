///////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʹ��Mock�����ChinaMarket�ĺ�����λ�ڴ��ļ��С�
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
  class CChinaMarketTest2 : public ::testing::Test
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
      ASSERT_FALSE(gl_fNormalMode);
      chinaMarket.CalculateTime();
      chinaMarket.ResetNeteaseRTDataInquiringIndex();
      chinaMarket.ResetNeteaseDayLineDataInquiringIndex();
      chinaMarket.ResetSinaRTDataInquiringIndex();
      chinaMarket.ResetTengxunRTDataInquiringIndex();
      chinaMarket.SetSystemReady(true); // �����г�ʱ��Ĭ��ϵͳ�Ѿ�׼����
      chinaMarket.SetPermitResetMarket(true);
      chinaMarket.SetCheckActiveStock(true);
      EXPECT_TRUE(chinaMarket.IsResetMarket());

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_ThreadStatus.SetSavingTempData(false);
      chinaMarket.SetRTDataSetCleared(false);
      chinaMarket.SetUpdateStockCodeDB(false);
      chinaMarket.SetUpdateOptionDB(false);
      chinaMarket.ClearChoicedRTDataQueue();
      chinaMarket.SetResetMarket(true);
      chinaMarket.ResetNeteaseRTDataInquiringIndex();
      chinaMarket.ResetNeteaseDayLineDataInquiringIndex();
      chinaMarket.ResetSinaRTDataInquiringIndex();
      chinaMarket.ResetTengxunRTDataInquiringIndex();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      chinaMarket.SetCurrentStockChanged(false);
      chinaMarket.SetPermitResetMarket(true);
      chinaMarket.SetCheckActiveStock(true);
      chinaMarket.SetSystemReady(true); // �뿪�˲���ʱ��Ĭ��ϵͳ��׼���á�
      for (int i = 0; i < chinaMarket.GetTotalStock(); i++) {
        CChinaStockPtr pStock = chinaMarket.GetStock(i);
        if (!pStock->IsDayLineNeedUpdate()) pStock->SetDayLineNeedUpdate(true);
        if (pStock->IsDayLineNeedProcess()) pStock->SetDayLineNeedProcess(false);
        if (pStock->IsDayLineNeedSaving()) pStock->SetDayLineNeedSaving(false);
      }

      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_systemMessage.GetDayLineInfoDequeSize() > 0) gl_systemMessage.PopDayLineInfoMessage();
      while (gl_systemMessage.GetInnerSystemInformationDequeSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();
    }

    CMockChinaMarket chinaMarket;
  };

  TEST_F(CChinaMarketTest2, TestSaveDayLineData1) {
    CChinaStockPtr pStock = chinaMarket.GetStock(_T("sh600000"));
    EXPECT_FALSE(pStock->IsDayLineNeedSaving());
    EXPECT_CALL(chinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(0)
      .WillOnce(Return(true));
    chinaMarket.SaveDayLineData();
  }

  TEST_F(CChinaMarketTest2, TestSaveDayLineData2) {
    CChinaStockPtr pStock = chinaMarket.GetStock(_T("sh600000"));

    pStock->SetDayLineNeedSaving(true);
    EXPECT_CALL(chinaMarket, RunningThreadSaveDayLineOfOneStock(_))
      .Times(0);
    chinaMarket.SaveDayLineData();
  }

  TEST_F(CChinaMarketTest2, TestSaveDayLineData3) {
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

  TEST_F(CChinaMarketTest2, TestSaveDayLineData4) {
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

  TEST_F(CChinaMarketTest2, TestUpdateTempRTData) {
    gl_ThreadStatus.SetSavingTempData(true);
    EXPECT_FALSE(chinaMarket.UpdateTempRTData());
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_CALL(chinaMarket, RunningThreadSaveTempRTData())
      .Times(1);
    EXPECT_TRUE(chinaMarket.UpdateTempRTData());
    EXPECT_TRUE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(true);
  }

  TEST_F(CChinaMarketTest2, TestTaskLoadCurrentStockDayLine1) {
    chinaMarket.ResetCurrentStock();
    EXPECT_CALL(chinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_TRUE(chinaMarket.TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketTest2, TestTaskLoadCurrentStockDayLine2) {
    chinaMarket.ResetCurrentStock();
    chinaMarket.SetCurrentStock(_T("sh600000"));
    chinaMarket.GetCurrentStock()->SetDayLineLoaded(true);
    EXPECT_CALL(chinaMarket, RunningThreadLoadDayLine(_))
      .Times(0);
    EXPECT_TRUE(chinaMarket.TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketTest2, TestTaskLoadCurrentStockDayLine3) {
    chinaMarket.ResetCurrentStock();
    chinaMarket.SetCurrentStock(_T("sh600000"));
    chinaMarket.GetCurrentStock()->SetDayLineLoaded(false);
    EXPECT_CALL(chinaMarket, RunningThreadLoadDayLine(_))
      .Times(1);
    EXPECT_TRUE(chinaMarket.TaskLoadCurrentStockDayLine());
  }

  TEST_F(CChinaMarketTest2, TestTaskUpdateStockCodeDB) {
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

  TEST_F(CChinaMarketTest2, TestTaskUpdateOptionDB) {
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

  TEST_F(CChinaMarketTest2, TestTaskSaveChoicedRTData) {
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
}