#include"pch.h"

#include"globedef.h"
#include"ChinaStock.h"
#include"CrweberIndexMarket.h"
#include"WebInquirer.h"

// CVirtualMarket�޷�����ʵ�����ʶ��亯���Ĳ��Է������
namespace StockAnalysisTest {
  class CCrweberIndexMarketTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
    }

    static void TearDownTestSuite(void) {
      while (gl_ThreadStatus.IsWorkingThreadRunning()) Sleep(1);
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_pCrweberIndexMarket->SetDatabaseLoaded(false);
      gl_pCrweberIndexMarket->SetPermitResetMarket(true);
      gl_pCrweberIndexMarket->SetReadyToRun(true);
      gl_pCrweberIndexMarket->SetResetMarket(true);
      gl_pCrweberIndexMarket->SetNewestUpdateDate(0);
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
      gl_pCrweberIndexMarket->SetDatabaseLoaded(false);
      gl_pCrweberIndexMarket->SetPermitResetMarket(true);
      gl_pCrweberIndexMarket->SetReadyToRun(true);
      gl_pCrweberIndexMarket->SetResetMarket(true);
      gl_pCrweberIndexMarket->SetNewestUpdateDate(0);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }
  };

  TEST_F(CCrweberIndexMarketTest, TestInitialize) {
    EXPECT_FALSE(gl_pCrweberIndexMarket->IsDatabaseLoaded());
    EXPECT_EQ(gl_pCrweberIndexMarket->GetNewestUpdateDate(), 0);
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsPermitResetMarket());
    EXPECT_EQ(gl_pCrweberIndexMarket->GetMarketTimeZone(), 0);
  }

  TEST_F(CCrweberIndexMarketTest, TestResetMarket) {
    size_t l = gl_systemMessage.GetInformationDequeSize();
    CString strCmp = _T("����Crweber.com�ڸ������α�׼ʱ�䣺");
    gl_pCrweberIndexMarket->ResetMarket();
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1);
    CString str = gl_systemMessage.PopInformationMessage();
    //EXPECT_THAT(str, StartsWith(_T("����Crweber.com�ڸ������α�׼ʱ�䣺")));
    CString strLeft = str.Left(35);
    EXPECT_STREQ(strLeft, strCmp);
  }

  TEST_F(CCrweberIndexMarketTest, TestTaskResetMarket) {
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsResetMarket());
    gl_pCrweberIndexMarket->SetResetMarket(false);
    gl_pCrweberIndexMarket->TaskResetMarket(69999);
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsPermitResetMarket());
    EXPECT_FALSE(gl_pCrweberIndexMarket->IsResetMarket());
    gl_pCrweberIndexMarket->TaskResetMarket(71000);
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsPermitResetMarket());
    EXPECT_FALSE(gl_pCrweberIndexMarket->IsResetMarket());
    gl_pCrweberIndexMarket->TaskResetMarket(70000);
    EXPECT_FALSE(gl_pCrweberIndexMarket->IsPermitResetMarket());
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsResetMarket());
    gl_pCrweberIndexMarket->SetPermitResetMarket(true);
  }

  TEST_F(CCrweberIndexMarketTest, TestSetNewestDatabaseDate) {
    gl_pCrweberIndexMarket->Reset();
    gl_pCrweberIndexMarket->GetNewestDatabaseDateFromDB();

    CSetCrweberIndex setCrweberIndex;
    long lDate = 0;

    setCrweberIndex.m_strSort = _T("[Date]");
    setCrweberIndex.Open();
    if (!setCrweberIndex.IsEOF()) {
      setCrweberIndex.MoveLast();
      lDate = setCrweberIndex.m_Date;
    }
    setCrweberIndex.Close();

    EXPECT_EQ(gl_pCrweberIndexMarket->GetNewestDatabaseDate(), lDate);
  }

  TEST_F(CCrweberIndexMarketTest, TestSetNewestUpdateDate) {
    EXPECT_EQ(gl_pCrweberIndexMarket->GetNewestUpdateDate(), 0);
    gl_pCrweberIndexMarket->SetNewestUpdateDate(20201220);
    EXPECT_EQ(gl_pCrweberIndexMarket->GetNewestUpdateDate(), 20201220);
  }

  TEST_F(CCrweberIndexMarketTest, TestSetDatabaseLoaded) {
    EXPECT_FALSE(gl_pCrweberIndexMarket->IsDatabaseLoaded());
    gl_pCrweberIndexMarket->SetDatabaseLoaded(true);
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsDatabaseLoaded());
    gl_pCrweberIndexMarket->SetDatabaseLoaded(false);
    EXPECT_FALSE(gl_pCrweberIndexMarket->IsDatabaseLoaded());
  }

  TEST_F(CCrweberIndexMarketTest, TestSchedulingTaskPer1Minute) {
    EXPECT_TRUE(gl_pCrweberIndexMarket->SchedulingTaskPer1Minute(60, 90000));
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsDatabaseLoaded());
    //gl_pCrweberIndexMarket->SetDatabaseLoaded(false); // ���ô˱�ʶ����������ĺ�����ȥ����������
    EXPECT_FALSE(gl_pCrweberIndexMarket->SchedulingTaskPer1Minute(59, 90000)) << "��һ��δ��ִ��ʱ��";
    EXPECT_CALL(*gl_pCrweberIndexWebInquiry, StartReadingThread).Times(1);
    EXPECT_TRUE(gl_pCrweberIndexMarket->SchedulingTaskPer1Minute(1, 90000));
    EXPECT_TRUE(gl_pCrweberIndexMarket->IsDatabaseLoaded());
    EXPECT_TRUE(gl_pCrweberIndexWebInquiry->IsReadingWebData()) << _T("����Mock��ʱû�����ô˱�ʶ\n");
    gl_pCrweberIndexWebInquiry->SetReadingWebData(false);
  }

  TEST_F(CCrweberIndexMarketTest, TestTaskProcessWebRTDataGetFromCrweberdotcom) {
    EXPECT_TRUE(gl_pCrweberIndexMarket->TaskProcessWebRTDataGetFromCrweberdotcom()) << "Ŀǰû�����ݣ�ֻ�Ǽ�����һ��";
  }
}