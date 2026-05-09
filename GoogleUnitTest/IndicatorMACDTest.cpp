#include"pch.h"

#include"GeneralCheck.h"
#include "MovingAverageConvergenceDivergence.h"

namespace FireBirdTest {
	class CMACDResultTest : public ::testing::Test {
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
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMACDResultTest, TestMax) {
		MACDResult r1{ 1.0, 2.0, 3.0 };
		EXPECT_DOUBLE_EQ(r1.Max(), 3.0);
		MACDResult r2{ 3.0, 2.0, 1.0 };
		EXPECT_DOUBLE_EQ(r2.Max(), 3.0);
		MACDResult r3{ 2.0, 3.0, 1.0 };
		EXPECT_DOUBLE_EQ(r3.Max(), 3.0);
	}

	TEST_F(CMACDResultTest, TestMin) {
		MACDResult r1{ 1.0, 2.0, 3.0 };
		EXPECT_DOUBLE_EQ(r1.Min(), 1.0);
		MACDResult r2{ 3.0, 2.0, 1.0 };
		EXPECT_DOUBLE_EQ(r2.Min(), 1.0);
		MACDResult r3{ 2.0, 3.0, 1.0 };
		EXPECT_DOUBLE_EQ(r3.Min(), 1.0);
	}
}
