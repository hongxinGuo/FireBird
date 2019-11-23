#include"pch.h"

#include"CriticalSectionCounter.h"

namespace StockAnalysisTest {
  TEST(CriticalSectionCounter, TestInitinialize) {
    CCriticalSectionCounter CCCounter;
    EXPECT_TRUE(CCCounter.DecreasingCounter());
    EXPECT_FALSE(CCCounter.IsActive());
    EXPECT_TRUE(CCCounter.IncreasingCounter());
    CCCounter.IncreasingCounter();
    EXPECT_TRUE(CCCounter.IsActive());
    EXPECT_TRUE(CCCounter.IncreasingCounter());
  }

  TEST(CriticalSectionCounter, TestFunctions) {
    CCriticalSectionCounter CCCounter;
    for (int i = 0; i < 4; i++) {
      EXPECT_FALSE(CCCounter.IsActive());
    }
  }
}