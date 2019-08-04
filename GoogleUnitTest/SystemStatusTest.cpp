#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

#include"SystemStatus.h"

using namespace testing;

namespace StockAnalysisTest {
  
  TEST(SystemStatusTest, TestInitialize) {
    ASSERT_TRUE(gl_fTestMode);
    EXPECT_FALSE(gl_systemStatus.IsCalculateRSInProcess());
    EXPECT_FALSE(gl_systemStatus.IsCalculatingRTData());
    EXPECT_FALSE(gl_systemStatus.IsDayLineDataReady());
    EXPECT_FALSE(gl_systemStatus.IsExitingClientThreadInProcess());
    EXPECT_FALSE(gl_systemStatus.IsReadingInProcess());
    EXPECT_FALSE(gl_systemStatus.IsRTDataNeedCalculate());
    EXPECT_FALSE(gl_systemStatus.IsRTDataReadingInProcess());
    EXPECT_FALSE(gl_systemStatus.IsRTDataReceived());
    EXPECT_FALSE(gl_systemStatus.IsSavingDayLineInProcess());
    EXPECT_FALSE(gl_systemStatus.IsSavingStockCodeData());
    EXPECT_FALSE(gl_systemStatus.IsSavingTempData());

    long l = gl_systemMessage.GetInformationDequeSize();
    CSystemStatus systemStatus; // 生成第二个实例（第一个为全局变量，系统启动时就生成了）
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), l + 1); // 系统报警队列
    for (int i = 0; i < l+1; i++) {
      CString str = gl_systemMessage.PopInformationMessage(); // 清除信息队列
    }
    EXPECT_EQ(gl_systemMessage.GetInformationDequeSize(), 0);
  }
  
  TEST(SystemStatusTest, TestIsExitingClientThreadInProcess) {
    gl_systemStatus.SetExitingClientThreadInProcess(true);
    EXPECT_TRUE(gl_systemStatus.IsExitingClientThreadInProcess());
    gl_systemStatus.SetExitingClientThreadInProcess(false);
    EXPECT_FALSE(gl_systemStatus.IsExitingClientThreadInProcess());
  }

  TEST(SystemStatusTest, TestIsCalculateRSInProcess) {
    gl_systemStatus.SetCalculateRSInProcess(true);
    EXPECT_TRUE(gl_systemStatus.IsCalculateRSInProcess());
    gl_systemStatus.SetCalculateRSInProcess(false);
    EXPECT_FALSE(gl_systemStatus.IsCalculateRSInProcess());
  }

  TEST(SystemStatusTest, TestReadingInProcess) {
    gl_systemStatus.SetReadingInProcess(true);
    EXPECT_TRUE(gl_systemStatus.IsReadingInProcess());
    gl_systemStatus.SetReadingInProcess(false);
    EXPECT_FALSE(gl_systemStatus.IsReadingInProcess());
  }

  TEST(SystemStatusTest, TestIsDataBaseInProcess) {
    gl_systemStatus.SetSavingDayLineInProcess(true);
    EXPECT_TRUE(gl_systemStatus.IsSavingDayLineInProcess());
    gl_systemStatus.SetSavingDayLineInProcess(false);
    EXPECT_FALSE(gl_systemStatus.IsSavingDayLineInProcess());
  }

  TEST(SystemStatusTest, TestIsRTDataReadInProcess) {
    gl_systemStatus.SetRTDataReadingInProcess(true);
    EXPECT_TRUE(gl_systemStatus.IsRTDataReadingInProcess());
    gl_systemStatus.SetRTDataReadingInProcess(false);
    EXPECT_FALSE(gl_systemStatus.IsRTDataReadingInProcess());
  }

  TEST(SystemStatusTest, TestIsRTDataReceived) {
    gl_systemStatus.SetRTDataReceived(true);
    EXPECT_TRUE(gl_systemStatus.IsRTDataReceived());
    gl_systemStatus.SetRTDataReceived(false);
    EXPECT_FALSE(gl_systemStatus.IsRTDataReceived());
  }

  TEST(SystemStatusTest, TestIsRTDataNeedCalculate) {
    gl_systemStatus.SetRTDataNeedCalculate(true);
    EXPECT_TRUE(gl_systemStatus.IsRTDataNeedCalculate());
    gl_systemStatus.SetRTDataNeedCalculate(false);
    EXPECT_FALSE(gl_systemStatus.IsRTDataNeedCalculate());
  }

  TEST(SystemStatusTest, TestIsDayLineDataReady) {
    gl_systemStatus.SetDayLineDataReady(true);
    EXPECT_TRUE(gl_systemStatus.IsDayLineDataReady());
    gl_systemStatus.SetDayLineDataReady(false);
    EXPECT_FALSE(gl_systemStatus.IsDayLineDataReady());
  }

  TEST(SystemStatusTest, TestIsCalculatingRTData) {
    gl_systemStatus.SetCalculatingRTData(true);
    EXPECT_TRUE(gl_systemStatus.IsCalculatingRTData());
    gl_systemStatus.SetCalculatingRTData(false);
    EXPECT_FALSE(gl_systemStatus.IsCalculatingRTData());
  }

  TEST(SystemStatusTest, TestIsSavingTempData) {
    gl_systemStatus.SetSavingTempData(true);
    EXPECT_TRUE(gl_systemStatus.IsSavingTempData());
    gl_systemStatus.SetSavingTempData(false);
    EXPECT_FALSE(gl_systemStatus.IsSavingTempData());
  }

  TEST(SystemStatusTest, TestIsSavingStockCodeData) {
    gl_systemStatus.SetSavingStockCodeData(true);
    EXPECT_TRUE(gl_systemStatus.IsSavingStockCodeData());
    gl_systemStatus.SetSavingStockCodeData(false);
    EXPECT_FALSE(gl_systemStatus.IsSavingStockCodeData());
  }

}
