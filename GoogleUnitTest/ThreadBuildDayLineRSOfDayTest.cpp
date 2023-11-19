#include"pch.h"

#include"GeneralCheck.h"

#include"MockChinaMarket.h"

#include"thread.h"

using namespace testing;

namespace FireBirdTest {
	CMockChinaMarketPtr s_pMarket;

	class CThreadBuildDayLineRSOfDateTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			s_pMarket = make_shared<CMockChinaMarket>();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			s_pMarket = nullptr;
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
	};

	TEST_F(CThreadBuildDayLineRSOfDateTest, TestThreadBuildDayLineRSOfDate) {
		EXPECT_CALL(*s_pMarket, BuildDayLineRS(20200101))
		.Times(1)
		.WillOnce(Return(true));
		gl_systemConfiguration.SetExitingCalculatingRS(false);
		gl_systemConfiguration.SetExitingSystem(false);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(s_pMarket, 20200101), 12);

		EXPECT_CALL(*s_pMarket, BuildDayLineRS(20200101))
		.Times(0);
		gl_systemConfiguration.SetExitingCalculatingRS(true);
		gl_systemConfiguration.SetExitingSystem(false);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(s_pMarket, 20200101), 12);

		EXPECT_CALL(*s_pMarket, BuildDayLineRS(20200101))
		.Times(0);
		gl_systemConfiguration.SetExitingCalculatingRS(false);
		gl_systemConfiguration.SetExitingSystem(true);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(s_pMarket, 20200101), 12);

		EXPECT_CALL(*s_pMarket, BuildDayLineRS(20200101))
		.Times(0);
		gl_systemConfiguration.SetExitingCalculatingRS(true);
		gl_systemConfiguration.SetExitingSystem(true);
		EXPECT_EQ(ThreadBuildDayLineRSOfDate(s_pMarket, 20200101), 12);
	}
}
