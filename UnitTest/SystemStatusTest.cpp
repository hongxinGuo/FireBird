#include"stdafx.h"
#include"pch.h"

#include"SystemStatus.h"

namespace StockAnalysisTest {
  TEST(SystemStatusTest, TestIsCalculateRSInProcess) {
    CSystemStatus status;
    status.SetCalculateRSInProcess(true);
    EXPECT_TRUE(status.IsCalculateRSInProcess());
    status.SetCalculateRSInProcess(false);
    EXPECT_FALSE(status.IsCalculateRSInProcess());
  }
  
  TEST(SystemStatusTest, TestReadingInProcess) {
    CSystemStatus status;
    status.SetReadingInProcess(true);
    EXPECT_TRUE(status.IsReadingInProcess());
    status.SetReadingInProcess(false);
    EXPECT_FALSE(status.IsReadingInProcess());
  }

  TEST(SystemStatusTest, TestIsDataBaseInProcess) {
    CSystemStatus status;
    status.SetDataBaseInProcess(true);
    EXPECT_TRUE(status.IsDataBaseInProcess());
    status.SetDataBaseInProcess(false);
    EXPECT_FALSE(status.IsDataBaseInProcess());
  }

  TEST(SystemStatusTest, TestIsReceiveFromWebInProcess) {
    CSystemStatus status;
    status.SetReceiveFromWebInProcess(true);
    EXPECT_TRUE(status.IsReceiveFromWebInProcess());
    status.SetReceiveFromWebInProcess(false);
    EXPECT_FALSE(status.IsReceiveFromWebInProcess());
  }

  TEST(SystemStatusTest, TestIsRTDataReady) {
    CSystemStatus status;
    status.SetRTDataReady(true);
    EXPECT_TRUE(status.IsRTDataReady());
    status.SetRTDataReady(false);
    EXPECT_FALSE(status.IsRTDataReady());
  }
 
  TEST(SystemStatusTest, TestIsDayLineDataReady) {
    CSystemStatus status;
    status.SetDayLineDataReady(true);
    EXPECT_TRUE(status.IsDayLineDataReady());
    status.SetDayLineDataReady(false);
    EXPECT_FALSE(status.IsDayLineDataReady());
  }

}
