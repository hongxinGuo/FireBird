#include"pch.h"

#include"GeneralCheck.h"

namespace FireBirdTest {
	class InfoReportTest : public ::testing::Test {
		void SetUp() override {
			SCOPED_TRACE(""); GeneralCheck();
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);

			SCOPED_TRACE(""); GeneralCheck();
		}
	};
}
