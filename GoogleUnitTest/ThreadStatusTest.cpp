#include"pch.h"

#include"globedef.h"
#include"ThreadStatus.h"

#include"WebInquirer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
    EXPECT_FALSE(gl_ThreadStatus.IsBackGroundthreadsWorking());
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfBackGroundWorkingThread(), 0);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingDayLineRS());
    int iRunningThread = gl_ThreadStatus.GetNumberOfSavingThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread);

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

  TEST_F(ThreadStatusTest, TestIsSavThreadRunning) {
    int iRunningThread = gl_ThreadStatus.GetNumberOfSavingThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread);
    EXPECT_FALSE(gl_ThreadStatus.IsSavingThreadRunning());
    gl_ThreadStatus.IncreaseSavingThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread + 1);
    EXPECT_TRUE(gl_ThreadStatus.IsSavingThreadRunning());
    gl_ThreadStatus.DecreaseSavingThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread);
    EXPECT_FALSE(gl_ThreadStatus.IsSavingThreadRunning());
    gl_ThreadStatus.DecreaseSavingThread();
    if (iRunningThread <= 0) {
      EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread);
      EXPECT_FALSE(gl_ThreadStatus.IsSavingThreadRunning());
    }
    else {
      EXPECT_EQ(gl_ThreadStatus.GetNumberOfSavingThread(), iRunningThread - 1);
    }
  }

  TEST_F(ThreadStatusTest, TestIsWebInquiringThreadRunning) {
    int iRunningThread = gl_ThreadStatus.GetNumberOfWebInquiringThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iRunningThread);
    EXPECT_FALSE(gl_ThreadStatus.IsWebInquiringThreadRunning());
    gl_ThreadStatus.IncreaseWebInquiringThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iRunningThread + 1);
    EXPECT_TRUE(gl_ThreadStatus.IsWebInquiringThreadRunning());
    gl_ThreadStatus.DecreaseWebInquiringThread();
    EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iRunningThread);
    EXPECT_FALSE(gl_ThreadStatus.IsWebInquiringThreadRunning());
    gl_ThreadStatus.DecreaseWebInquiringThread();
    if (iRunningThread <= 0) {
      EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iRunningThread);
      EXPECT_FALSE(gl_ThreadStatus.IsWebInquiringThreadRunning());
    }
    else {
      EXPECT_EQ(gl_ThreadStatus.GetNumberOfWebInquiringThread(), iRunningThread - 1);
    }
  }
}