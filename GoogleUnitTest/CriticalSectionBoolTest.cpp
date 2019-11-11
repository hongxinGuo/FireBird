#include"pch.h"

#include"CriticalSectionBool.h"

namespace StockAanlysisTest {
  TEST(CriticalSectionBoolTest, TestInitialize) {
    CCriticalSectionBool CCBool;
    EXPECT_FALSE(CCBool.IsTrue());
  }

  TEST(CriticalSectionBoolTest, TestFunctions) {
    CCriticalSectionBool CCBool;
    EXPECT_FALSE(CCBool.IsTrue());
    CCBool.SetFlag(true);
    EXPECT_TRUE(CCBool.IsTrue());
    CCBool.SetFlag(false);
    EXPECT_FALSE(CCBool.IsTrue());
    EXPECT_TRUE(CCBool.CheckFalseAndThenSetIt());
    EXPECT_TRUE(CCBool.IsTrue());
    EXPECT_TRUE(CCBool.CheckTrueAndThenClearIt());
    EXPECT_FALSE(CCBool.IsTrue());
  }
}

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}