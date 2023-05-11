#include"pch.h"

#include"GeneralCheck.h"

#include"MockDataChinaDayLine.h"

#include<memory>
using std::make_shared;

using namespace testing;

namespace FireBirdTest {
	CMockDataChinaDayLinePtr s_pMockDayLine;

	class CMockDataChinaDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
			SCOPED_TRACE(""); GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
			s_pMockDayLine = make_shared<CMockDataChinaDayLine>();
		}

		void TearDown() override {
			// clearUp
			s_pMockDayLine = nullptr;

			SCOPED_TRACE(""); GeneralCheck();
		}

	protected:
	};

	TEST_F(CMockDataChinaDayLineTest, TestCalculateDayLineRS) {
		InSequence Seq;
		EXPECT_CALL(*s_pMockDayLine, CalculateRS1(3))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRS1(5))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRS1(10))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRS1(30))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRS1(60))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRS1(120))
		.Times(1);
		s_pMockDayLine->CalculateRS0();
	}

	TEST_F(CMockDataChinaDayLineTest, TestCalculateDayLineRSLogarithm) {
		InSequence Seq;
		EXPECT_CALL(*s_pMockDayLine, CalculateRSLogarithm1(3))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRSLogarithm1(5))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRSLogarithm1(10))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRSLogarithm1(30))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRSLogarithm1(60))
		.Times(1)
		.WillOnce(Return(true));
		EXPECT_CALL(*s_pMockDayLine, CalculateRSLogarithm1(120))
		.Times(1);
		s_pMockDayLine->CalculateRSLogarithm0();
	}

	TEST_F(CMockDataChinaDayLineTest, TestCalculateDayLineRSIndex) {
		InSequence Seq;
		EXPECT_CALL(*s_pMockDayLine, CalculateRSIndex1(3))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRSIndex1(5))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRSIndex1(10))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRSIndex1(30))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRSIndex1(60))
		.Times(1);
		EXPECT_CALL(*s_pMockDayLine, CalculateRSIndex1(120))
		.Times(1);
		s_pMockDayLine->CalculateRSIndex0();
	}
}
