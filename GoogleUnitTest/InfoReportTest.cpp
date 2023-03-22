#include"pch.h"

#include"GeneralCheck.h"

namespace FireBirdTest {
	class InfoReportTest : public ::testing::Test {
		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemStatus.SetWorkingMode(false);

			GeneralCheck();
		}
	};
}
