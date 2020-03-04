#include"stdafx.h"
#include"pch.h"

#include"Semaphore.h"

using namespace MyLib;

namespace StockAnalysisTest {
  Semaphore s_TestSemaphore(10);

  TEST(SemaphoreTest, TestSetMaxCount) {
    EXPECT_EQ(s_TestSemaphore.GetMaxCount(), 10);
    s_TestSemaphore.SetMaxCount(5);
    EXPECT_EQ(s_TestSemaphore.GetMaxCount(), 5);
    s_TestSemaphore.SetMaxCount();
    EXPECT_EQ(s_TestSemaphore.GetMaxCount(), 1);
  }
}