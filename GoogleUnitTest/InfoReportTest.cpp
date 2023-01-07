#include"pch.h"

#include"GeneralCheck.h"

namespace FireBirdTest {
	class InfoReportTest : public ::testing::Test {
		void SetUp(void) override {
			GeneralCheck();
		}

		void TearDown(void) override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};
}
