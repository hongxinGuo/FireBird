#include"stdafx.h"
#include"pch.h"
#include"globedef.h"

#include"MockDayLineContainer.h"

using namespace testing;
using namespace testing;

namespace StockAnalysisTest {
  class CMockDLContainerTest : public ::testing::Test
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
      pDL = make_shared<CMockDLContainer>();
    }

    virtual void TearDown(void) override {
      // clearup
    }

  protected:
    CMockDLContainerPtr pDL;
  };

  TEST_F(CMockDLContainerTest, TestCalculateDLRS) {
    InSequence Seq;
    EXPECT_CALL(*pDL, CalculateRS1(3))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRS1(5))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRS1(10))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRS1(30))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRS1(60))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRS1(120))
      .Times(1);
    pDL->CalculateRS();
  }

  TEST_F(CMockDLContainerTest, TestCalculateDLRSLogarithm) {
    InSequence Seq;
    EXPECT_CALL(*pDL, CalculateRSLogarithm1(3))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRSLogarithm1(5))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRSLogarithm1(10))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRSLogarithm1(30))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRSLogarithm1(60))
      .Times(1)
      .WillOnce(Return(true));
    EXPECT_CALL(*pDL, CalculateRSLogarithm1(120))
      .Times(1);
    pDL->CalculateRSLogarithm();
  }

  TEST_F(CMockDLContainerTest, TestCalculateDLRSIndex) {
    InSequence Seq;
    EXPECT_CALL(*pDL, CalculateRSIndex1(3))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRSIndex1(5))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRSIndex1(10))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRSIndex1(30))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRSIndex1(60))
      .Times(1);
    EXPECT_CALL(*pDL, CalculateRSIndex1(120))
      .Times(1);
    pDL->CalculateRSIndex();
  }
}