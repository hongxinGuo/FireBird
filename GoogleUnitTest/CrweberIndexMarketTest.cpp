#include"pch.h"

#include"globedef.h"
#include"ChinaStock.h"
#include"CrweberIndexMarket.h"

// CVirtualMarket无法生成实例，故而其函数的测试放在这里。
namespace StockAnalysisTest {
  class CCrweberIndexMarketTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      gl_CrweberIndexMarket.SetDatabaseLoaded(false);
      gl_CrweberIndexMarket.SetPermitResetMarket(true);
      gl_CrweberIndexMarket.SetReadyToRun(true);
      gl_CrweberIndexMarket.SetResetMarket(true);
      gl_CrweberIndexMarket.SetNewestUpdateDay(0);
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
    }

    virtual void TearDown(void) override {
      // clearup
      gl_CrweberIndexMarket.SetDatabaseLoaded(false);
      gl_CrweberIndexMarket.SetPermitResetMarket(true);
      gl_CrweberIndexMarket.SetReadyToRun(true);
      gl_CrweberIndexMarket.SetResetMarket(true);
      gl_CrweberIndexMarket.SetNewestUpdateDay(0);
      while (gl_systemMessage.GetInformationDequeSize() > 0) gl_systemMessage.PopInformationMessage();
    }
  };

  TEST_F(CCrweberIndexMarketTest, TestInitialize) {
    EXPECT_FALSE(gl_CrweberIndexMarket.IsDatabaseLoaded());
    EXPECT_EQ(gl_CrweberIndexMarket.GetNewestUpdateDay(), 0);
    EXPECT_TRUE(gl_CrweberIndexMarket.IsPermitResetMarket());
    EXPECT_EQ(gl_CrweberIndexMarket.GetTimeZoneOffset(), 0);
  }

  TEST_F(CCrweberIndexMarketTest, TestResetMarket) {
    long l = gl_systemMessage.GetInformationDequeSize();
    gl_CrweberIndexMarket.ResetMarket();
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1);
    CString str = gl_systemMessage.PopInformationMessage();
    CString strLeft = str.Left(35);
    EXPECT_STREQ(strLeft, _T("重置Crweber.com于格林威治标准时间："));
  }

  TEST_F(CCrweberIndexMarketTest, TestMaintainDatabase) {
    EXPECT_TRUE(gl_CrweberIndexMarket.IsMaintainDatabase());
    EXPECT_FALSE(gl_CrweberIndexMarket.TaskMaintainDatabase(10000));
    EXPECT_TRUE(gl_CrweberIndexMarket.IsMaintainDatabase());
    EXPECT_FALSE(gl_CrweberIndexMarket.TaskMaintainDatabase(11000));
    EXPECT_TRUE(gl_CrweberIndexMarket.IsMaintainDatabase());
    EXPECT_TRUE(gl_CrweberIndexMarket.TaskMaintainDatabase(10001));
    EXPECT_FALSE(gl_CrweberIndexMarket.IsMaintainDatabase());
    EXPECT_FALSE(gl_CrweberIndexMarket.TaskMaintainDatabase(10001)) << _T("标识为假时永远不执行");
    EXPECT_FALSE(gl_CrweberIndexMarket.TaskMaintainDatabase(11001)) << _T("标识为假时永远不执行");

    gl_CrweberIndexMarket.SetMaintainDatabase(true);
    EXPECT_TRUE(gl_CrweberIndexMarket.IsMaintainDatabase());
    gl_CrweberIndexMarket.SetMaintainDatabase(false);
    EXPECT_FALSE(gl_CrweberIndexMarket.IsMaintainDatabase());
  }

  TEST_F(CCrweberIndexMarketTest, TestTaskResetMarket) {
    EXPECT_TRUE(gl_CrweberIndexMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_CrweberIndexMarket.IsResetMarket());
    gl_CrweberIndexMarket.SetResetMarket(false);
    gl_CrweberIndexMarket.TaskResetMarket(9999);
    EXPECT_TRUE(gl_CrweberIndexMarket.IsPermitResetMarket());
    EXPECT_FALSE(gl_CrweberIndexMarket.IsResetMarket());
    gl_CrweberIndexMarket.TaskResetMarket(13001);
    EXPECT_FALSE(gl_CrweberIndexMarket.IsPermitResetMarket());
    EXPECT_FALSE(gl_CrweberIndexMarket.IsResetMarket());
    gl_CrweberIndexMarket.SetPermitResetMarket(true);
    gl_CrweberIndexMarket.TaskResetMarket(10000);
    EXPECT_FALSE(gl_CrweberIndexMarket.IsPermitResetMarket());
    EXPECT_TRUE(gl_CrweberIndexMarket.IsResetMarket());
  }

  TEST_F(CCrweberIndexMarketTest, TestSetNewestDatabaseDay) {
    gl_CrweberIndexMarket.Reset();
    gl_CrweberIndexMarket.GetNewestDatabaseDayFromDB();

    CSetCrweberIndex setCrweberIndex;
    long lDay = 0;

    setCrweberIndex.m_strSort = _T("[Day]");
    setCrweberIndex.Open();
    if (!setCrweberIndex.IsEOF()) {
      setCrweberIndex.MoveLast();
      lDay = setCrweberIndex.m_Day;
    }
    setCrweberIndex.Close();

    EXPECT_EQ(gl_CrweberIndexMarket.GetNewestDatabaseDay(), lDay);
  }

  TEST_F(CCrweberIndexMarketTest, TestSetNewestUpdateDay) {
    EXPECT_EQ(gl_CrweberIndexMarket.GetNewestUpdateDay(), 0);
    gl_CrweberIndexMarket.SetNewestUpdateDay(20201220);
    EXPECT_EQ(gl_CrweberIndexMarket.GetNewestUpdateDay(), 20201220);
  }

  TEST_F(CCrweberIndexMarketTest, TestSetDatabaseLoaded) {
    EXPECT_FALSE(gl_CrweberIndexMarket.IsDatabaseLoaded());
    gl_CrweberIndexMarket.SetDatabaseLoaded(true);
    EXPECT_TRUE(gl_CrweberIndexMarket.IsDatabaseLoaded());
    gl_CrweberIndexMarket.SetDatabaseLoaded(false);
    EXPECT_FALSE(gl_CrweberIndexMarket.IsDatabaseLoaded());
  }

  TEST_F(CCrweberIndexMarketTest, TestSchedulingTaskPer1Minute) {
    EXPECT_TRUE(gl_CrweberIndexMarket.SchedulingTaskPer1Minute(60, 90000));
    EXPECT_TRUE(gl_CrweberIndexMarket.IsDatabaseLoaded());
    gl_CrweberIndexMarket.SetDatabaseLoaded(false); // 重置此标识，否则下面的函数会去读网络数据
    EXPECT_FALSE(gl_CrweberIndexMarket.SchedulingTaskPer1Minute(59, 90000)) << "差一秒未到执行时间";
    EXPECT_FALSE(gl_CrweberIndexMarket.IsDatabaseLoaded());
    gl_CrweberIndexMarket.SetDatabaseLoaded(false); // 重置此标识，否则下面的函数会去读网络数据
    EXPECT_TRUE(gl_CrweberIndexMarket.SchedulingTaskPer1Minute(1, 90000));
    EXPECT_TRUE(gl_CrweberIndexMarket.IsDatabaseLoaded());
  }

  TEST_F(CCrweberIndexMarketTest, TestTaskProcessWebRTDataGetFromCrweberdotcom) {
    EXPECT_TRUE(gl_CrweberIndexMarket.TaskProcessWebRTDataGetFromCrweberdotcom()) << "目前没有数据，只是简单运行一下";
  }
}