#include"stdafx.h"
#include"pch.h"

#include"globedef.h"
#include"ThreadStatus.h"

#include"WebInquirer.h"

namespace StockAnalysisTest {
  class ThreadStatusTest : public ::testing::Test
  {
  protected:
    virtual void SetUp(void) override {
    }

    virtual void TearDown(void) override {
      // clearup
    }
  };

  TEST_F(ThreadStatusTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRS());
    EXPECT_EQ(gl_ThreadStatus.HowManyThreadsCalculatingDayLineRS(), 0);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);

    long l = gl_systemMessage.GetInformationDequeSize();
    CThreadStatus threadStatus; // 生成第二个实例（第一个为全局变量，系统启动时就生成了）
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // 系统报警队列
    for (int i = 0; i < l + 1; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // 清除信息队列
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }

  TEST_F(ThreadStatusTest, TestIsCalculatingDayLineRS) {
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
    gl_ThreadStatus.SetCalculatingDayLineRS(true);
    EXPECT_TRUE(gl_ThreadStatus.IsCalculatingDayLineRS());
    gl_ThreadStatus.SetCalculatingDayLineRS(false);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
  }

  TEST_F(ThreadStatusTest, TestIsRTDataNeedCalculate) {
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
    gl_ThreadStatus.SetRTDataNeedCalculate(true);
    EXPECT_TRUE(gl_ThreadStatus.IsRTDataNeedCalculate());
    gl_ThreadStatus.SetRTDataNeedCalculate(false);
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
  }

  TEST_F(ThreadStatusTest, TestIsCalculatingRTData) {
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
    gl_ThreadStatus.SetCalculatingRTData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRTData());
    gl_ThreadStatus.SetCalculatingRTData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
  }

  TEST_F(ThreadStatusTest, TestIsSavingTempData) {
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
  }

  TEST_F(ThreadStatusTest, TestIsCalculatingRS) {
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRS());
    for (int i = 0; i < gl_cMaxCalculatingRSThreads; i++) {  // 目前采用最多8个线程
      gl_ThreadStatus.IncreaseNunberOfCalculatingRSThreads();
    }
    for (int i = 0; i < gl_cMaxCalculatingRSThreads - 1; i++) {
      gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads();
      EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRS());
    }
    gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads();
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRS());
  }

  TEST_F(ThreadStatusTest, TestIsSavingDayLine) {
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
    for (int i = 0; i < gl_cMaxSavingOneDayLineThreads; i++) {  // 目前采用最多6个线程
      gl_ThreadStatus.IncreaseNunberOfSavingDayLineThreads();
    }
    for (int i = 0; i < gl_cMaxSavingOneDayLineThreads - 1; i++) {
      gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();
      EXPECT_TRUE(gl_ThreadStatus.IsSavingDayLine());
    }
    gl_ThreadStatus.DecreaseNumberOfSavingDayLineThreads();
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
  }

  TEST_F(ThreadStatusTest, TestIsWorkingThreadRunning) {
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_FALSE(gl_ThreadStatus.IsWorkingThreadRunning());
    gl_ThreadStatus.IncreaseNumberOfRunningThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 1);
    EXPECT_TRUE(gl_ThreadStatus.IsWorkingThreadRunning());
    gl_ThreadStatus.DecreaseNumberOfRunningThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_FALSE(gl_ThreadStatus.IsWorkingThreadRunning());
    gl_ThreadStatus.DecreaseNumberOfRunningThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), 0);
    EXPECT_FALSE(gl_ThreadStatus.IsWorkingThreadRunning());
  }
}