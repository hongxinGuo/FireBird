#include"pch.h"

#include"GeneralCheck.h"

#include"InfoReport.h"

namespace FireBirdTest {
	class InfoReportTest : public ::testing::Test {
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			gl_systemConfiguration.SetWorkingMode(false);

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(InfoReportTest, TestReportWebError) {
		DWORD dwErrorNo = 1234567890;
		string strInputMessage = "This is a test error message.";
		string s = std::format("Net Error # {:Ld} message: {}", dwErrorNo, strInputMessage);

		ReportWebError(dwErrorNo, strInputMessage);
		EXPECT_STREQ(s.c_str(), gl_systemMessage.PopErrorMessage().c_str());
	}

	TEST_F(InfoReportTest, TestReportWebError2) {
		DWORD dwErrorNo = 876543210;
		long long llTime = 54321;
		string strInputMessage = "Another test error message.";
		string s = std::format("Net Error # {:Ld} time：{:Ld}ms message: {}", dwErrorNo, llTime, strInputMessage);

		ReportWebError(dwErrorNo, llTime, strInputMessage);
		EXPECT_STREQ(s.c_str(), gl_systemMessage.PopErrorMessage().c_str());
	}
}
