#include"pch.h"

#include"GeneralCheck.h"

#include"InfoReport.h"
#include "CharSetTransfer.h"
#include "SystempublicDeclaration.h"

namespace FireBirdTest {
	class InfoReportTest : public ::testing::Test {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			while (gl_systemMessage.InnerSystemInfoSize() > 0) gl_systemMessage.PopInnerSystemInformationMessage();

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

	TEST_F(InfoReportTest, TestReportErrorToSystemMessage) {
		std::runtime_error ex("this is a runtime error");
		ReportErrorToSystemMessage("PREFIX:", ex);
		EXPECT_STREQ("PREFIX:this is a runtime error", gl_systemMessage.PopErrorMessage().c_str());
	}

	TEST_F(InfoReportTest, TestReportInformationAndDeleteException_Null) {
		size_t before = gl_systemMessage.InnerSystemInfoSize();
		ReportInformationAndDeleteException(nullptr);
		EXPECT_EQ(before, gl_systemMessage.InnerSystemInfoSize());
	}

	TEST_F(InfoReportTest, TestReportInformationAndDeleteException_WithException) {
		CException* p = new CMemoryException();
		ReportInformationAndDeleteException(p);

		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		//恢复原状
		gl_systemMessage.PopInnerSystemInformationMessage();
	}

	TEST_F(InfoReportTest, TestReportInformation_WithExceptionRef) {
		CMemoryException ex;
		ReportInformation(ex);

		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		//恢复原状
		gl_systemMessage.PopInnerSystemInformationMessage();
	}

	TEST_F(InfoReportTest, TestWatchdogReporting_WhenWindowExists) {
		// Create a dummy window with the watchdog title so FindWindow can locate it.
		std::wstring wsTitle = Utf8ToW(sWatchDogApp);
		HWND hWnd = CreateWindowW(L"STATIC", wsTitle.c_str(), WS_OVERLAPPEDWINDOW,
		                          CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
		ASSERT_NE(hWnd, nullptr);

		// Calls should return 0 when the watchdog window exists.
		EXPECT_EQ(0, ReportRunningToWatchdog());
		EXPECT_EQ(0, ReportExitToWatchdog());
		EXPECT_EQ(0, ReportSchedulingExitToWatchdog());
		EXPECT_EQ(0, ReportCheckRunningExitToWatchdog());

		DestroyWindow(hWnd);
	}
}
