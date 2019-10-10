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
    EXPECT_FALSE(gl_ThreadStatus.IsExitingThread());
    EXPECT_FALSE(gl_ThreadStatus.IsReadingNeteaseDayLine());
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
    EXPECT_FALSE(gl_ThreadStatus.IsReadingSinaRTData());
    EXPECT_FALSE(gl_ThreadStatus.IsSinaRTDataReceived());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
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

  TEST(SystemStatusTest, TestIsExitingThread) {
    gl_ThreadStatus.SetExitingThread(true);
    EXPECT_TRUE(gl_ThreadStatus.IsExitingThread());
    gl_ThreadStatus.SetExitingThread(false);
    EXPECT_FALSE(gl_ThreadStatus.IsExitingThread());
  }

  TEST(SystemStatusTest, TestReading) {
    gl_ThreadStatus.SetReadingNeteaseDayLine(true);
    EXPECT_TRUE(gl_ThreadStatus.IsReadingNeteaseDayLine());
    gl_ThreadStatus.SetReadingNeteaseDayLine(false);
    EXPECT_FALSE(gl_ThreadStatus.IsReadingNeteaseDayLine());
  }

  TEST(SystemStatusTest, TestIsDataBase) {
    gl_ThreadStatus.SetSavingDayLine(true);
    EXPECT_TRUE(gl_ThreadStatus.IsSavingDayLine());
    gl_ThreadStatus.SetSavingDayLine(false);
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLine());
  }

  TEST(SystemStatusTest, TestIsReadingRTData) {
    gl_ThreadStatus.SetReadingSinaRTData(true);
    EXPECT_TRUE(gl_ThreadStatus.IsReadingSinaRTData());
    gl_ThreadStatus.SetReadingSinaRTData(false);
    EXPECT_FALSE(gl_ThreadStatus.IsReadingSinaRTData());
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
    for (int i = 0; i < gl_cMaxCalculatingRSThreads; i++) {  // 目前采用最多8个线程
     EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRSThreadAvailable());
     gl_ThreadStatus.IncreaseNunberOfCalculatingRSThreads();
    }
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRSThreadAvailable());
    for (int i = 0; i < gl_cMaxCalculatingRSThreads - 1; i++) {
      gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads();
      EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRSThreadAvailable());
      EXPECT_TRUE(gl_ThreadStatus.IsCalculatingRSThreadRunning());
    }
    gl_ThreadStatus.DecreaseNumberOfCalculatingRSThreads();
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRSThreadRunning());
  }
}