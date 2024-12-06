#include"pch.h"

////#include"gtest/gtest.h"

#include"GeneralCheck.h"

#include"MockDataChinaWeekLine.h"

using namespace testing;

namespace FireBirdTest {
	class CMockContainerChinaDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			pWeekLine = make_shared<CMockDataChinaWeekLine>();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CMockDataChinaWeekLinePtr pWeekLine;
	};

	TEST_F(CMockContainerChinaDayLineTest, TestCalculateRS) {
		InSequence Seq;
		EXPECT_CALL(*pWeekLine, CalculateRS1(3))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRS1(5))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRS1(10))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRS1(30))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRS1(60))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRS1(120))
		.Times(1);
		pWeekLine->CalculateRS0();
	}

	TEST_F(CMockContainerChinaDayLineTest, TestCalculateRSLogarithm) {
		InSequence Seq;
		EXPECT_CALL(*pWeekLine, CalculateRSLogarithm1(3))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRSLogarithm1(5))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRSLogarithm1(10))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRSLogarithm1(30))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRSLogarithm1(60))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*pWeekLine, CalculateRSLogarithm1(120))
		.Times(1);
		pWeekLine->CalculateRSLogarithm0();
	}

	TEST_F(CMockContainerChinaDayLineTest, TestCalculateRSIndex) {
		InSequence Seq;
		EXPECT_CALL(*pWeekLine, CalculateRSIndex1(3))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRSIndex1(5))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRSIndex1(10))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRSIndex1(30))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRSIndex1(60))
		.Times(1);
		EXPECT_CALL(*pWeekLine, CalculateRSIndex1(120))
		.Times(1);
		pWeekLine->CalculateRSIndex0();
	}
}
