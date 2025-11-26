#include"pch.h"

#include"GeneralCheck.h"

#include"MockChinaMarket.h"

//#include"thread.h"

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
}
