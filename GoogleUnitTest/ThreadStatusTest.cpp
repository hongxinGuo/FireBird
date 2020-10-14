#include"stdafx.h"
#include"pch.h"

#include"globedef.h"
#include"ThreadStatus.h"

#include"WebInquirer.h"

namespace StockAnalysisTest {
  class ThreadStatusTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) {
      EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
    }
    virtual void SetUp(void) override {
      EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
    }

    virtual void TearDown(void) override {
      // clearup
      EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
    }
  };

  TEST_F(ThreadStatusTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
    EXPECT_FALSE(gl_ThreadStatus.IsBackGroundthreadsWorking());
    EXPECT_EQ(gl_ThreadStatus.HowManyBackGroundThreadsWorking(), 0);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
    int iRunningThread = gl_ThreadStatus.GetNumberOfRunningThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), iRunningThread);

    size_t l = gl_systemMessage.GetInformationDequeSize();
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

  TEST_F(ThreadStatusTest, TestIsCalculatingWeekLineRS) {
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingWeekLineRS());
    gl_ThreadStatus.SetCalculatingWeekLineRS(true);
    EXPECT_TRUE(gl_ThreadStatus.IsCalculatingWeekLineRS());
    gl_ThreadStatus.SetCalculatingWeekLineRS(false);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingWeekLineRS());
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

  TEST_F(ThreadStatusTest, TestIsCreateWeekLine) {
    EXPECT_FALSE(gl_ThreadStatus.IsCreatingWeekLine());
    gl_ThreadStatus.SetCreatingWeekLine(true);
    EXPECT_TRUE(gl_ThreadStatus.IsCreatingWeekLine());
    gl_ThreadStatus.SetCreatingWeekLine(false);
    EXPECT_FALSE(gl_ThreadStatus.IsCreatingWeekLine());
  }

  TEST_F(ThreadStatusTest, TestIsCalculatingRS) {
    EXPECT_FALSE(gl_ThreadStatus.IsBackGroundthreadsWorking());
    for (int i = 0; i < cMaxBackGroundTaskThreads; i++) {  // 目前采用最多8个线程
      gl_ThreadStatus.IncreaseBackGroundWorkingthreads();
    }
    for (int i = 0; i < cMaxBackGroundTaskThreads - 1; i++) {
      gl_ThreadStatus.DecreaseBackGroundWorkingthreads();
      EXPECT_TRUE(gl_ThreadStatus.IsBackGroundthreadsWorking());
    }
    gl_ThreadStatus.DecreaseBackGroundWorkingthreads();
    EXPECT_FALSE(gl_ThreadStatus.IsBackGroundthreadsWorking());
  }

  TEST_F(ThreadStatusTest, TestIsSavingDayLine) {
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
    for (int i = 0; i < gl_cMaxSavingOneDayLineThreads; i++) {  // 目前采用最多4个线程
      gl_ThreadStatus.IncreaseSavingDayLineThreads();
    }
    for (int i = 0; i < gl_cMaxSavingOneDayLineThreads - 1; i++) {
      gl_ThreadStatus.DecreaseSavingDayLineThreads();
      EXPECT_TRUE(gl_ThreadStatus.IsSavingDayLine());
    }
    gl_ThreadStatus.DecreaseSavingDayLineThreads();
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
  }

  TEST_F(ThreadStatusTest, TestIsWorkingThreadRunning) {
    int iRunningThread = gl_ThreadStatus.GetNumberOfRunningThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), iRunningThread);
    EXPECT_FALSE(gl_ThreadStatus.IsWorkingThreadRunning());
    gl_ThreadStatus.IncreaseRunningThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), iRunningThread + 1);
    EXPECT_TRUE(gl_ThreadStatus.IsWorkingThreadRunning());
    gl_ThreadStatus.DecreaseRunningThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), iRunningThread);
    EXPECT_FALSE(gl_ThreadStatus.IsWorkingThreadRunning());
    gl_ThreadStatus.DecreaseRunningThread();
    if (iRunningThread <= 0) {
      EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), iRunningThread);
      EXPECT_FALSE(gl_ThreadStatus.IsWorkingThreadRunning());
    }
    else {
      EXPECT_EQ(gl_ThreadStatus.GetNumberOfRunningThread(), iRunningThread - 1);
    }
  }
}