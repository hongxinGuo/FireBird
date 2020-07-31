#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"MockWeekLineContainer.h"

using namespace testing;
using namespace testing;

namespace StockAnalysisTest {
  class CMockWeekLineContainerTest : public ::testing::Test
  {
  protected:
    static void SetUpTestSuite(void) { // 本测试类的初始化函数
      ASSERT_FALSE(gl_fNormalMode);
    }

    static void TearDownTestSuite() {
    }

    virtual void SetUp(void) override {
      ASSERT_FALSE(gl_fNormalMode);
      ASSERT_TRUE(gl_fTestMode);
      pWeekLine = make_shared<CMockWeekLineContainer>();
    }

    virtual void TearDown(void) override {
      // clearup
    }

  protected:
    CMockWeekLineContainerPtr pWeekLine;
  };

  TEST_F(CMockWeekLineContainerTest, TestCalculateRS) {
    InSequence Seq;
    EXPECT_CALL(*pWeekLine, CalculateRS(3))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRS(5))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRS(10))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRS(30))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRS(60))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRS(120))
      .Times(1);
    pWeekLine->CalculateRelativeStrong();
  }

  TEST_F(CMockWeekLineContainerTest, TestCalculateRSLogarithm) {
    InSequence Seq;
    EXPECT_CALL(*pWeekLine, CalculateRSLogarithm(3))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRSLogarithm(5))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRSLogarithm(10))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRSLogarithm(30))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRSLogarithm(60))
      .Times(1)
      .WillOnce(Return(true));
    EXPECT_CALL(*pWeekLine, CalculateRSLogarithm(120))
      .Times(1);
    pWeekLine->CalculateRelativeStrongLogarithm();
  }

  TEST_F(CMockWeekLineContainerTest, TestCalculateRSIndex) {
    InSequence Seq;
    EXPECT_CALL(*pWeekLine, CalculateRSIndex(3))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRSIndex(5))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRSIndex(10))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRSIndex(30))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRSIndex(60))
      .Times(1);
    EXPECT_CALL(*pWeekLine, CalculateRSIndex(120))
      .Times(1);
    pWeekLine->CalculateRelativeStrongIndex();
  }
}