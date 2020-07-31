#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"MockDayLineContainer.h"

using namespace testing;
using namespace testing;

namespace StockAnalysisTest {
  class CMockDayLineContainerTest : public ::testing::Test
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
      pDayLine = make_shared<CMockDayLineContainer>();
    }

    virtual void TearDown(void) override {
      // clearup
    }

  protected:
    CMockDayLineContainerPtr pDayLine;
  };

  TEST_F(CMockDayLineContainerTest, TestCalculateDayLineRS) {
    InSequence Seq;
    EXPECT_CALL(*pDayLine, CalculateRS(3))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRS(5))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRS(10))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRS(30))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRS(60))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRS(120))
      .Times(1);
    pDayLine->CalculateRelativeStrong();
  }

  TEST_F(CMockDayLineContainerTest, TestCalculateDayLineRSLogarithm) {
    InSequence Seq;
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm(3))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm(5))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm(10))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm(30))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm(60))
      .Times(1)
      .WillOnce(Return(true));
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm(120))
      .Times(1);
    pDayLine->CalculateRelativeStrongLogarithm();
  }

  TEST_F(CMockDayLineContainerTest, TestCalculateDayLineRSIndex) {
    InSequence Seq;
    EXPECT_CALL(*pDayLine, CalculateRSIndex(3))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSIndex(5))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSIndex(10))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSIndex(30))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSIndex(60))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSIndex(120))
      .Times(1);
    pDayLine->CalculateRelativeStrongIndex();
  }
}