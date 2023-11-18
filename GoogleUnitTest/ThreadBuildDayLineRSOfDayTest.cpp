#include"pch.h"

#include"GeneralCheck.h"

#include"MockChinaMarket.h"

#include"thread.h"

using namespace testing;

namespace FireBirdTest {
	class CThreadBuildDayLineRSOfDateTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
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
			gl_systemConfiguration.SetExitingSystem(false);
			SCOPED_TRACE("");
			GeneralCheck();
		}

		CMockChinaMarket market;
	};

	TEST_F(CThreadBuildDayLineRSOfDateTest, TestThreadBuildDayLineRSOfDate) {
		EXPECT_CALL(market, BuildDayLineRS(20200101))
		.Times(1)
		.WillOnce(Return(true));
		gl_systemConfiguration.SetExitingCalculatingRS(false);
		gl_systemConfiguration.SetExitingSystem(false);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), 12);

		EXPECT_CALL(market, BuildDayLineRS(20200101))
		.Times(0);
		gl_systemConfiguration.SetExitingCalculatingRS(true);
		gl_systemConfiguration.SetExitingSystem(false);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), 12);

		EXPECT_CALL(market, BuildDayLineRS(20200101))
		.Times(0);
		gl_systemConfiguration.SetExitingCalculatingRS(false);
		gl_systemConfiguration.SetExitingSystem(true);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), 12);

		EXPECT_CALL(market, BuildDayLineRS(20200101))
		.Times(0);
		gl_systemConfiguration.SetExitingCalculatingRS(true);
		gl_systemConfiguration.SetExitingSystem(true);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(&market, 20200101), 12);
	}
}
