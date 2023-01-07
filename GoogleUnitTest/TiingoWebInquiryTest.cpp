#include"pch.h"

#include"GeneralCheck.h"
#include"WorldMarket.h"

#include"TiingoDataSource.h"
#include"MockTiingoWebInquiry.h"

using namespace testing;

namespace FireBirdTest {
	static bool m_fSystemStatus;

	class CTiingoWebInquiryTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			m_fSystemStatus = gl_pWorldMarket->IsSystemReady();
			gl_pWorldMarket->SetSystemReady(true); // Tiingo引擎必须等待系统初始化后才可使用。
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			gl_pWorldMarket->SetSystemReady(m_fSystemStatus);
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			m_TiingoWebInquiry.SetDataSource(gl_pTiingoDataSource.get());
			gl_pWorldMarket->CalculateTime();
		}

		void TearDown(void) override {
			// clearUp
			gl_pWorldMarket->SetResetMarket(true);
			GeneralCheck();
		}

		CMockTiingoWebInquiry m_TiingoWebInquiry; // 网易日线历史数据
	};

	TEST_F(CTiingoWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_TiingoWebInquiry.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(m_TiingoWebInquiry.GetInquiryToken(), _T(""));
		EXPECT_STREQ(m_TiingoWebInquiry.GetConnectionName(), _T("Tiingo"));
		EXPECT_THAT(m_TiingoWebInquiry.GetInquiringNumber(), 1) << "默认每次一个数据";
	}

	TEST_F(CTiingoWebInquiryTest, TestGetWebData) {
		m_TiingoWebInquiry.SetReadingWebData(true);
		EXPECT_FALSE(m_TiingoWebInquiry.GetWebData());
		m_TiingoWebInquiry.SetReadingWebData(false);
		gl_pWorldMarket->SetSystemReady(true);
		EXPECT_CALL(m_TiingoWebInquiry, StartReadingThread)
			.Times(1);
		m_TiingoWebInquiry.GetWebData();
		EXPECT_TRUE(m_TiingoWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	TEST_F(CTiingoWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_TiingoWebInquiry.ReportStatus(1));
	}

	TEST_F(CTiingoWebInquiryTest, TestPrepareNextInquiringStr) {
		CString str;
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < 4; i++) {
			if (m_TiingoWebInquiry.PrepareNextInquiringString()) {
				str = m_TiingoWebInquiry.GetInquiringString();
				EXPECT_STREQ(str.Right(47), _T(""));
			}
			else
				EXPECT_EQ(str.GetLength(), 0);
		}
		gl_pWorldMarket->SetSystemReady(false);
	}

	TEST_F(CTiingoWebInquiryTest, TestGetNextInquiringMiddleStr) {
		EXPECT_STREQ(m_TiingoWebInquiry.GetNextInquiringMiddleString(2, true), _T("")) << "此函数不执行任何动作";
	}
}
