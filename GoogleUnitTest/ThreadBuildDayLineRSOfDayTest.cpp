#include"pch.h"

#include"GeneralCheck.h"

#include"MockChinaMarket.h"

#include"thread.h"

using namespace testing;

namespace FireBirdTest {
	class CThreadBuildDayLineRSOfDateTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemStatus.SetExitingSystem(false);
			GeneralCheck();
		}

		CMockChinaMarket market;
	};

	TEST_F(CThreadBuildDayLineRSOfDateTest, TestThreadBuildDayLineRSOfDate) {
		EXPECT_CALL(market, BuildDayLineRS(20200101))
		.Times(1)
		.WillOnce(Return(true));
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(false);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), 12);

		EXPECT_CALL(market, BuildDayLineRS(20200101))
		.Times(0);
		gl_systemStatus.SetExitingCalculatingRS(true);
		gl_systemStatus.SetExitingSystem(false);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), 12);

		EXPECT_CALL(market, BuildDayLineRS(20200101))
		.Times(0);
		gl_systemStatus.SetExitingCalculatingRS(false);
		gl_systemStatus.SetExitingSystem(true);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), 12);

		EXPECT_CALL(market, BuildDayLineRS(20200101))
		.Times(0);
		gl_systemStatus.SetExitingCalculatingRS(true);
		gl_systemStatus.SetExitingSystem(true);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), 12);
	}
}
