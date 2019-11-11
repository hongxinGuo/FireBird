#include"pch.h"

#include"CriticalSectionCounter.h"

namespace StockAanlysisTest {
  TEST(CriticalSectionCounter, TestInitinialize) {
    CCriticalSectionCounter CCCounter;
    EXPECT_FALSE(CCCounter.SecceedDecreasingCounter());
    EXPECT_FALSE(CCCounter.IsActive());
    EXPECT_TRUE(CCCounter.SecceedIncreasingCounter());
    EXPECT_TRUE(CCCounter.IsActive());
    EXPECT_FALSE(CCCounter.IsCounterAvailable());
    EXPECT_FALSE(CCCounter.SecceedIncreasingCounter());
  }

  TEST(CriticalSectionCounter, TestFunctions) {
    CCriticalSectionCounter CCCounter;
    CCCounter.SetMaxCounter(4);
    for (int i = 0; i < 4; i++) {
      EXPECT_TRUE(CCCounter.IsCounterAvailable());
      EXPECT_TRUE(CCCounter.SecceedIncreasingCounter());
      EXPECT_TRUE(CCCounter.IsActive());
    }
  }
}