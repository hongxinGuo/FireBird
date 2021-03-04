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
    EXPECT_CALL(*pDayLine, CalculateRS1(3))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRS1(5))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRS1(10))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRS1(30))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRS1(60))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRS1(120))
      .Times(1);
    pDayLine->CalculateRS0();
  }

  TEST_F(CMockDayLineContainerTest, TestCalculateDayLineRSLogarithm) {
    InSequence Seq;
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm1(3))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm1(5))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm1(10))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm1(30))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm1(60))
      .Times(1)
      .WillOnce(Return(true));
    EXPECT_CALL(*pDayLine, CalculateRSLogarithm1(120))
      .Times(1);
    pDayLine->CalculateRSLogarithm0();
  }

  TEST_F(CMockDayLineContainerTest, TestCalculateDayLineRSIndex) {
    InSequence Seq;
    EXPECT_CALL(*pDayLine, CalculateRSIndex1(3))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSIndex1(5))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSIndex1(10))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSIndex1(30))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSIndex1(60))
      .Times(1);
    EXPECT_CALL(*pDayLine, CalculateRSIndex1(120))
      .Times(1);
    pDayLine->CalculateRSIndex0();
  }
}