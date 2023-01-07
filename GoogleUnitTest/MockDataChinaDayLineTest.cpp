#include"pch.h"

#include"GeneralCheck.h"

#include"MockDataChinaDayLine.h"

#include<memory>
using std::make_shared;

using namespace testing;

namespace FireBirdTest {
	class CMockDataChinaDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() { GeneralCheck(); }

		void SetUp(void) override {
			GeneralCheck();
			pDayLine = make_shared<CMockDataChinaDayLine>();
		}

		void TearDown(void) override {
			// clearUp
		}

	protected:
		CMockDataChinaDayLinePtr pDayLine;
	};

	TEST_F(CMockDataChinaDayLineTest, TestCalculateDayLineRS) {
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

	TEST_F(CMockDataChinaDayLineTest, TestCalculateDayLineRSLogarithm) {
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

	TEST_F(CMockDataChinaDayLineTest, TestCalculateDayLineRSIndex) {
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
