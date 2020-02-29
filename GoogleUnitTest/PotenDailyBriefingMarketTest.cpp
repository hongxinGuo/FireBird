#include"pch.h"

#include"globedef.h"
#include"ChinaStock.h"
#include"PotenDailyBriefingMarket.h"
#include"WebInquirer.h"

// CVirtualMarket�޷�����ʵ�����ʶ��亯���Ĳ��Է������
namespace StockAnalysisTest {
  class CPotenDailyBriefingMarketTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
      EXPECT_EQ(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20180411);
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsReadyToRun());
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
      gl_PotenDailyBriefingMarket.ClearDatabase();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_PotenDailyBriefingMarket.SetDatabaseLoaded(false);
      gl_PotenDailyBriefingMarket.SetPermitResetMarket(true);
      gl_PotenDailyBriefingMarket.SetReadyToRun(true);
      gl_PotenDailyBriefingMarket.SetResetMarket(true);
      gl_PotenDailyBriefingMarket.SetCurrentInquiringDay(20180411);
      gl_PotenDailyBriefingMarket.ClearDatabase();
      gl_PotenDailyBriefingMarket.Reset();
      gl_PotenDailyBriefingMarket.SetTodayDataUpdated(false);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
    }
  };

  TEST_F(CPotenDailyBriefingMarketTest, TestInitialize) {
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20180411);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestTaskResetMarket) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.TaskResetMarket(5959);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.TaskResetMarket(13001);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.TaskResetMarket(10000);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.SetPermitResetMarket(true); // ���ô˱�ʶ
    gl_PotenDailyBriefingMarket.TaskResetMarket(13000);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());

    gl_PotenDailyBriefingMarket.SetPermitResetMarket(true); // ���ô˱�ʶ
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestIsResetMarket) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.SetResetMarket(false);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsResetMarket());
    gl_PotenDailyBriefingMarket.SetResetMarket(true);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsResetMarket());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestCheckTodayDataUpdated) {
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsTodayDataUpdated());
    gl_PotenDailyBriefingMarket.SetCurrentInquiringDay(gl_PotenDailyBriefingMarket.GetNextDay(gl_PotenDailyBriefingMarket.GetDay()));
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.TaskCheckTodayDataUpdated());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsTodayDataUpdated());
    gl_PotenDailyBriefingMarket.SetTodayDataUpdated(false);
    gl_PotenDailyBriefingMarket.SetCurrentInquiringDay(gl_PotenDailyBriefingMarket.GetDay());
    if (gl_PotenDailyBriefingMarket.IsWorkingDay()) {
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.TaskCheckTodayDataUpdated());
      EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsTodayDataUpdated());
    }
    else {
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.TaskCheckTodayDataUpdated()) << _T("��ĩ������");
      EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsTodayDataUpdated()) << _T("��ĩ������");
    }
    gl_PotenDailyBriefingMarket.SetTodayDataUpdated(false);
    gl_PotenDailyBriefingMarket.SetCurrentInquiringDay(gl_PotenDailyBriefingMarket.GetPrevDay(gl_PotenDailyBriefingMarket.GetDay()));
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.TaskCheckTodayDataUpdated());
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsTodayDataUpdated());

    gl_PotenDailyBriefingMarket.SetTodayDataUpdated(true);
    gl_PotenDailyBriefingMarket.SetCurrentInquiringDay(gl_PotenDailyBriefingMarket.GetNextDay(gl_PotenDailyBriefingMarket.GetDay()));
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsTodayDataUpdated());
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.TaskCheckTodayDataUpdated());
    gl_PotenDailyBriefingMarket.SetCurrentInquiringDay(gl_PotenDailyBriefingMarket.GetDay());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsTodayDataUpdated());
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.TaskCheckTodayDataUpdated());
    gl_PotenDailyBriefingMarket.SetCurrentInquiringDay(gl_PotenDailyBriefingMarket.GetPrevDay(gl_PotenDailyBriefingMarket.GetDay()));
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsTodayDataUpdated());
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.TaskCheckTodayDataUpdated());
    gl_PotenDailyBriefingMarket.SetTodayDataUpdated(false);
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestTaskLoadDatabase) {
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.TaskLoadDataBase());
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsDatabaseLoaded());
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.TaskLoadDataBase());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestTaskInquiringData) {
    gl_PotenDailyBriefingMarket.SetTodayDataUpdated(true);
    EXPECT_FALSE(gl_WebInquirer.IsReadingPotenDailyBriefing()) << _T("�˱�ʶ�޷����ã��ʶ���ԶΪ��");
    gl_PotenDailyBriefingMarket.SetDatabaseLoaded(true);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.TaskInquiringData());
    gl_PotenDailyBriefingMarket.SetTodayDataUpdated(false);
    gl_PotenDailyBriefingMarket.SetDatabaseLoaded(false);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.TaskInquiringData());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestSetCurrentInquiringDay) {
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20180411);
    gl_PotenDailyBriefingMarket.SetCurrentInquiringDay(20190101);
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20190101);
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestResetMarket) {
    gl_PotenDailyBriefingMarket.ResetMarket();
    CString str = gl_systemMessage.PopInformationMessage();
    CString strLeft = str.Left(29);
    EXPECT_STREQ(strLeft, _T("����poten.com��������׼ʱ�䣺"));
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestLoadDatabase) {
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetDatabaseSize(), 0);
    gl_PotenDailyBriefingMarket.LoadDatabase();
    EXPECT_GT(gl_PotenDailyBriefingMarket.GetDatabaseSize(), 0);
    EXPECT_GT(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20180411);
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestSchedulingTaskPerSecond) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.SchedulingTaskPerSecond(1, 10000));
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestSchedulingTaskPer10Second) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.SchedulingTaskPer10Second(10, 10000));
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.SchedulingTaskPer10Second(9, 11000));
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.SchedulingTaskPer10Second(1, 11000));
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestSchedulingTaskPerMinute) {
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.SchedulingTaskPerMinute(60, 10000));
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.SchedulingTaskPerMinute(59, 11000));
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.SchedulingTaskPerMinute(1, 11000));
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestChoiceNextInquiringDay) {
    gl_PotenDailyBriefingMarket.__TEST_SetLoadedDay(20190101, true);
    gl_PotenDailyBriefingMarket.__TEST_SetLoadedDay(20190102, false);
    gl_PotenDailyBriefingMarket.__TEST_SetLoadedDay(20190103, true);
    gl_PotenDailyBriefingMarket.__TEST_SetLoadedDay(20190104, false);
    gl_PotenDailyBriefingMarket.__TEST_SetLoadedDay(20190105, true); // ����������Ϊ��
    gl_PotenDailyBriefingMarket.__TEST_SetLoadedDay(20190106, true); // �����գ���Ϊ��
    gl_PotenDailyBriefingMarket.__TEST_SetLoadedDay(20190107, true);
    gl_PotenDailyBriefingMarket.__TEST_SetLoadedDay(20190108, false);
    gl_PotenDailyBriefingMarket.__TEST_SetLoadedDay(20190109, true);
    gl_PotenDailyBriefingMarket.__TEST_SetLoadedDay(20190110, true);
    gl_PotenDailyBriefingMarket.SetCurrentInquiringDay(20190101);
    gl_PotenDailyBriefingMarket.ChoiceNextInquiringDay();
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20190102);
    gl_PotenDailyBriefingMarket.ChoiceNextInquiringDay();
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20190104);
    gl_PotenDailyBriefingMarket.ChoiceNextInquiringDay();
    EXPECT_EQ(gl_PotenDailyBriefingMarket.GetCurrentInquiringDay(), 20190108);
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestIsTodayDataUpdated) {
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsTodayDataUpdated());
    gl_PotenDailyBriefingMarket.SetTodayDataUpdated(true);
    EXPECT_TRUE(gl_PotenDailyBriefingMarket.IsTodayDataUpdated());
    gl_PotenDailyBriefingMarket.SetTodayDataUpdated(false);
    EXPECT_FALSE(gl_PotenDailyBriefingMarket.IsTodayDataUpdated());
  }
}