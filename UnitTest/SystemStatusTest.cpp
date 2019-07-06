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
}
