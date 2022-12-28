#include"pch.h"

#include"GeneralCheck.h"

#include"MockDataChinaWeekLine.h"

#include<memory>
using std::make_shared;

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CMockDataChinaDayLineTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() { GeneralCheck(); }

		void SetUp(void) override {
			GeneralCheck();
			pWeekLine = make_shared<CMockDataChinaWeekLine>();
		}

		void TearDown(void) override {
			// clearUp
		}

	protected:
		CMockDataChinaWeekLinePtr pWeekLine;
	};

	TEST_F(CMockDataChinaDayLineTest, TestCalculateRS) {
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

	TEST_F(CMockDataChinaDayLineTest, TestCalculateRSLogarithm) {
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

	TEST_F(CMockDataChinaDayLineTest, TestCalculateRSIndex) {
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
