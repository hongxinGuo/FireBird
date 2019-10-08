#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ThreadStatus.h"

using namespace testing;

namespace StockAnalysisTest {
  TEST(SystemStatusTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
    EXPECT_FALSE(gl_ThreadStatus.IsDayLineDataReady());
    EXPECT_FALSE(gl_ThreadStatus.IsExitingThreadInProcess());
    EXPECT_FALSE(gl_ThreadStatus.IsNeteaseDayLineReadingInProcess());
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
    EXPECT_FALSE(gl_ThreadStatus.IsSinaRTDataReadingInProcess());
    EXPECT_FALSE(gl_ThreadStatus.IsSinaRTDataReceived());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLineInProcess());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingStockCodeData());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRSThreadRunning());
    EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRSThreadAvailable());

    long l = gl_systemMessage.GetInformationDequeSize();
    CThreadStatus threadStatus; // 生成第二个实例（第一个为全局变量，系统启动时就生成了）
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // 系统报警队列
    for (int i = 0; i < l + 1; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // 清除信息队列
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }

  TEST(SystemStatusTest, TestIsExitingThreadInProcess) {
    gl_ThreadStatus.SetExitingThreadInProcess(true);
    EXPECT_TRUE(gl_ThreadStatus.IsExitingThreadInProcess());
    gl_ThreadStatus.SetExitingThreadInProcess(false);
    EXPECT_FALSE(gl_ThreadStatus.IsExitingThreadInProcess());
  }

  TEST(SystemStatusTest, TestReadingInProcess) {
    gl_ThreadStatus.SetNeteaseDayLineReadingInProcess(true);
    EXPECT_TRUE(gl_ThreadStatus.IsNeteaseDayLineReadingInProcess());
    gl_ThreadStatus.SetNeteaseDayLineReadingInProcess(false);
    EXPECT_FALSE(gl_ThreadStatus.IsNeteaseDayLineReadingInProcess());
  }

  TEST(SystemStatusTest, TestIsDataBaseInProcess) {
    gl_ThreadStatus.SetSavingDayLineInProcess(true);
    EXPECT_TRUE(gl_ThreadStatus.IsSavingDayLineInProcess());
    gl_ThreadStatus.SetSavingDayLineInProcess(false);
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLineInProcess());
  }

  TEST(SystemStatusTest, TestIsRTDataReadInProcess) {
    gl_ThreadStatus.SetSinaRTDataReadingInProcess(true);
    EXPECT_TRUE(gl_ThreadStatus.IsSinaRTDataReadingInProcess());
    gl_ThreadStatus.SetSinaRTDataReadingInProcess(false);
    EXPECT_FALSE(gl_ThreadStatus.IsSinaRTDataReadingInProcess());
  }

  TEST(SystemStatusTest, TestIsSinaRTDataReceived) {
    gl_ThreadStatus.SetSinaRTDataReceived(true);
    EXPECT_TRUE(gl_ThreadStatus.IsSinaRTDataReceived());
    gl_ThreadStatus.SetSinaRTDataReceived(false);
    EXPECT_FALSE(gl_ThreadStatus.IsSinaRTDataReceived());
  }

  TEST(SystemStatusTest, TestIsRTDataNeedCalculate) {
    gl_ThreadStatus.SetRTDataNeedCalculate(true);
    EXPECT_TRUE(gl_ThreadStatus.IsRTDataNeedCalculate());
    gl_ThreadStatus.SetRTDataNeedCalculate(false);
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
  }

  TEST(SystemStatusTest, TestIsDayLineDataReady) {
    gl_ThreadStatus.SetDayLineDataReady(true);
    EXPECT_TRUE(gl_ThreadStatus.IsDayLineDataReady());
    gl_ThreadStatus.SetDayLineDataReady(false);
    EXPECT_FALSE(gl_ThreadStatus.IsDayLineDataReady());
  }

  TEST(SystemStatusTest, TestIsCalculatingRTData) {
    gl_ThreadStatus.SetCalculatingRTData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRTData());
    gl_ThreadStatus.SetCalculatingRTData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
  }

  TEST(SystemStatusTest, TestIsSavingTempData) {
    gl_ThreadStatus.SetSavingTempData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsSavingTempData());
    gl_ThreadStatus.SetSavingTempData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());
  }

  TEST(SystemStatusTest, TestIsSavingStockCodeData) {
    gl_ThreadStatus.SetSavingStockCodeData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsSavingStockCodeData());
    gl_ThreadStatus.SetSavingStockCodeData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsSavingStockCodeData());
  }

  TEST(SystemStatusTest, TestIsCalculatingRS) {
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRSThreadRunning());
    for (int i = 0; i < 8; i++) {  // 目前采用最多8个线程
     EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRSThreadAvailable());
     gl_ThreadStatus.IncreaseNunberOfCalculatingRSThreads();
    }
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRSThreadAvailable());
    for (int i = 0; i < 7; i++) {
      gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads();
      EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRSThreadAvailable());
      EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRSThreadRunning());
    }
    gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads();
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRSThreadRunning());
  }
}