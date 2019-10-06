#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"ThreadStatus.h"

using namespace testing;

namespace StockAnalysisTest {
  TEST(SystemStatusTest, TestInitialize) {
    ASSERT_FALSE(gl_fNormalMode);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculateRSInProcess());
    EXPECT_FALSE(gl_ThreadStatus.IsCalculatingRTData());
    EXPECT_FALSE(gl_ThreadStatus.IsDayLineDataReady());
    EXPECT_FALSE(gl_ThreadStatus.IsExitingClientThreadInProcess());
    EXPECT_FALSE(gl_ThreadStatus.IsDayLineReadingInProcess());
    EXPECT_FALSE(gl_ThreadStatus.IsRTDataNeedCalculate());
    EXPECT_FALSE(gl_ThreadStatus.IsSinaRTDataReadingInProcess());
    EXPECT_FALSE(gl_ThreadStatus.IsSinaRTDataReceived());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingDayLineInProcess());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingStockCodeData());
    EXPECT_FALSE(gl_ThreadStatus.IsSavingTempData());

    long l = gl_systemMessage.GetInformationDequeSize();
    CThreadStutus threadStatus; // 生成第二个实例（第一个为全局变量，系统启动时就生成了）
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // 系统报警队列
    for (int i = 0; i < l + 1; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // 清除信息队列
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }

  TEST(SystemStatusTest, TestIsExitingClientThreadInProcess) {
    gl_ThreadStatus.SetExitingClientThreadInProcess(true);
    EXPECT_TRUE(gl_ThreadStatus.IsExitingClientThreadInProcess());
    gl_ThreadStatus.SetExitingClientThreadInProcess(false);
    EXPECT_FALSE(gl_ThreadStatus.IsExitingClientThreadInProcess());
  }

  TEST(SystemStatusTest, TestIsCalculateRSInProcess) {
    gl_ThreadStatus.SetCalculateRSInProcess(true);
    EXPECT_TRUE(gl_ThreadStatus.IsCalculateRSInProcess());
    gl_ThreadStatus.SetCalculateRSInProcess(false);
    EXPECT_FALSE(gl_ThreadStatus.IsCalculateRSInProcess());
  }

  TEST(SystemStatusTest, TestReadingInProcess) {
    gl_ThreadStatus.SetDayLineReadingInProcess(true);
    EXPECT_TRUE(gl_ThreadStatus.IsDayLineReadingInProcess());
    gl_ThreadStatus.SetDayLineReadingInProcess(false);
    EXPECT_FALSE(gl_ThreadStatus.IsDayLineReadingInProcess());
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
}