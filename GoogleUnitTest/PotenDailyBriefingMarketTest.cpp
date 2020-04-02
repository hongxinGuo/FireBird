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
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
      while (gl_ThreadStatus.IsWorkingThreadRunning()) Sleep(1);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsDatabaseLoaded());
      EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDay(), 20180411);
      EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
      EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsReadyToRun());
      EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
      gl_pPotenDailyBriefingMarket->ClearDatabase();
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pPotenDailyBriefingMarket->SetDatabaseLoaded(false);
      gl_pPotenDailyBriefingMarket->SetPermitResetMarket(true);
      gl_pPotenDailyBriefingMarket->SetReadyToRun(true);
      gl_pPotenDailyBriefingMarket->SetResetMarket(true);
      gl_pPotenDailyBriefingMarket->SetCurrentInquiringDay(20180411);
      gl_pPotenDailyBriefingMarket->ClearDatabase();
      gl_pPotenDailyBriefingMarket->Reset();
      gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
      while (gl_WebInquirer.IsReadingWebThreadRunning()) Sleep(1);
    }
  };

  TEST_F(CPotenDailyBriefingMarketTest, TestInitialize) {
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsDatabaseLoaded());
    EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDay(), 20180411);
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestTaskResetMarket) {
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
    gl_pPotenDailyBriefingMarket->TaskResetMarket(5959);
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
    gl_pPotenDailyBriefingMarket->TaskResetMarket(13001);
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
    gl_pPotenDailyBriefingMarket->TaskResetMarket(10000);
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
    gl_pPotenDailyBriefingMarket->SetPermitResetMarket(true); // ���ô˱�ʶ
    gl_pPotenDailyBriefingMarket->TaskResetMarket(13000);
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());

    gl_pPotenDailyBriefingMarket->SetPermitResetMarket(true); // ���ô˱�ʶ
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestIsResetMarket) {
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
    gl_pPotenDailyBriefingMarket->SetResetMarket(false);
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsResetMarket());
    gl_pPotenDailyBriefingMarket->SetResetMarket(true);
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsResetMarket());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestCheckTodayDataUpdated) {
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
    gl_pPotenDailyBriefingMarket->SetCurrentInquiringDay(gl_pPotenDailyBriefingMarket->GetNextDay(gl_pPotenDailyBriefingMarket->GetDay()));
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->TaskCheckTodayDataUpdated());
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
    gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
    gl_pPotenDailyBriefingMarket->SetCurrentInquiringDay(gl_pPotenDailyBriefingMarket->GetDay());
    if (gl_pPotenDailyBriefingMarket->IsWorkingDay()) {
      EXPECT_TRUE(gl_pPotenDailyBriefingMarket->TaskCheckTodayDataUpdated());
      EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
    }
    else {
      EXPECT_TRUE(gl_pPotenDailyBriefingMarket->TaskCheckTodayDataUpdated()) << _T("��ĩ������");
      EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated()) << _T("��ĩ������");
    }
    gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
    gl_pPotenDailyBriefingMarket->SetCurrentInquiringDay(gl_pPotenDailyBriefingMarket->GetPrevDay(gl_pPotenDailyBriefingMarket->GetDay()));
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->TaskCheckTodayDataUpdated());
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());

    gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(true);
    gl_pPotenDailyBriefingMarket->SetCurrentInquiringDay(gl_pPotenDailyBriefingMarket->GetNextDay(gl_pPotenDailyBriefingMarket->GetDay()));
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->TaskCheckTodayDataUpdated());
    gl_pPotenDailyBriefingMarket->SetCurrentInquiringDay(gl_pPotenDailyBriefingMarket->GetDay());
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->TaskCheckTodayDataUpdated());
    gl_pPotenDailyBriefingMarket->SetCurrentInquiringDay(gl_pPotenDailyBriefingMarket->GetPrevDay(gl_pPotenDailyBriefingMarket->GetDay()));
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->TaskCheckTodayDataUpdated());
    gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestTaskLoadDatabase) {
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsDatabaseLoaded());
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->TaskLoadDataBase());
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsDatabaseLoaded());
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->TaskLoadDataBase());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestTaskInquiringData) {
    gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(true);
    EXPECT_FALSE(gl_WebInquirer.IsReadingPotenDailyBriefing()) << _T("�˱�ʶ�޷����ã��ʶ���ԶΪ��");
    gl_pPotenDailyBriefingMarket->SetDatabaseLoaded(true);
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->TaskInquiringData());
    gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
    gl_pPotenDailyBriefingMarket->SetDatabaseLoaded(false);
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->TaskInquiringData());
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestSetCurrentInquiringDay) {
    EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDay(), 20180411);
    gl_pPotenDailyBriefingMarket->SetCurrentInquiringDay(20190101);
    EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDay(), 20190101);
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestResetMarket) {
    gl_pPotenDailyBriefingMarket->ResetMarket();
    CString str = gl_systemMessage.PopInformationMessage();
    CString strLeft = str.Left(29);
    EXPECT_STREQ(strLeft, _T("����poten.com��������׼ʱ�䣺"));
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestLoadDatabase) {
    CSetPotenDailyBriefing setPoten;

    EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetDatabaseSize(), 0);
    gl_pPotenDailyBriefingMarket->LoadDatabase();
    EXPECT_GT(gl_pPotenDailyBriefingMarket->GetDatabaseSize(), 0);
    setPoten.Open();
    setPoten.MoveLast();
    EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDay(), setPoten.m_Day);
    setPoten.Close();
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestSchedulingTaskPerSecond) {
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerSecond(1, 10000));
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestSchedulingTaskPer10Second) {
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->SchedulingTaskPer10Second(10, 10000));
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->SchedulingTaskPer10Second(9, 11000));
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->SchedulingTaskPer10Second(1, 11000));
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestSchedulingTaskPerMinute) {
    EXPECT_CALL(*gl_pPotenDailyBriefingWebInquiry, StartReadingThread).Times(1);
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerMinute(60, 10000));
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerMinute(59, 11000));
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->SchedulingTaskPerMinute(1, 11000));
    EXPECT_TRUE(gl_pPotenDailyBriefingWebInquiry->IsReadingWebData()) << _T("Ԥ�����õĴ˱�ʶ������Mock��û������֮���ʶ�������������״̬\n");
    gl_pPotenDailyBriefingWebInquiry->SetReadingWebData(false);
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestChoiceNextInquiringDay) {
    gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDay(20190101, true);
    gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDay(20190102, false);
    gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDay(20190103, true);
    gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDay(20190104, false);
    gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDay(20190105, true); // ����������Ϊ��
    gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDay(20190106, true); // �����գ���Ϊ��
    gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDay(20190107, true);
    gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDay(20190108, false);
    gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDay(20190109, true);
    gl_pPotenDailyBriefingMarket->__TEST_SetLoadedDay(20190110, true);
    gl_pPotenDailyBriefingMarket->SetCurrentInquiringDay(20190101);
    gl_pPotenDailyBriefingMarket->ChoiceNextInquiringDay();
    EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDay(), 20190102);
    gl_pPotenDailyBriefingMarket->ChoiceNextInquiringDay();
    EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDay(), 20190104);
    gl_pPotenDailyBriefingMarket->ChoiceNextInquiringDay();
    EXPECT_EQ(gl_pPotenDailyBriefingMarket->GetCurrentInquiringDay(), 20190108);
  }

  TEST_F(CPotenDailyBriefingMarketTest, TestIsTodayDataUpdated) {
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
    gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(true);
    EXPECT_TRUE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
    gl_pPotenDailyBriefingMarket->SetTodayDataUpdated(false);
    EXPECT_FALSE(gl_pPotenDailyBriefingMarket->IsTodayDataUpdated());
  }
}