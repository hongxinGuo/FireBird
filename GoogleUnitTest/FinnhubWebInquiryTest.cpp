#include"pch.h"

#include"WorldMarket.h"
#include"GeneralCheck.h"

#include"FinnhubDataSource.h"
#include"MockFinnhubWebInquiry.h"


using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubWebInquiryTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();

			m_FinnhubWebInquiry.SetDataSource(gl_pFinnhubDataSource.get());
			gl_pWorldMarket->CalculateTime();
		}

		virtual void TearDown(void) override {
			// clearUp
			gl_pWorldMarket->SetResetMarket(true);
			gl_pWorldMarket->SetSystemReady(false);
			GeneralCheck();
		}
		CMockFinnhubWebInquiry m_FinnhubWebInquiry; // 网易日线历史数据
	};

	TEST_F(CFinnhubWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_FinnhubWebInquiry.GetInquiryFunction(), _T(""));
		EXPECT_STREQ(m_FinnhubWebInquiry.GetInquiryToken(), _T(""));
		EXPECT_STREQ(m_FinnhubWebInquiry.GetConnectionName(), _T("Finnhub"));
		EXPECT_THAT(m_FinnhubWebInquiry.GetInquiringNumber(), 1) << "默认每次一个数据";
	}

	TEST_F(CFinnhubWebInquiryTest, TestGetWebData) {
		m_FinnhubWebInquiry.SetReadingWebData(true);
		EXPECT_FALSE(m_FinnhubWebInquiry.GetWebData());
		m_FinnhubWebInquiry.SetReadingWebData(false);
		gl_pWorldMarket->SetSystemReady(true);
		EXPECT_CALL(m_FinnhubWebInquiry, StartReadingThread)
			.Times(1);
		m_FinnhubWebInquiry.GetWebData();
		EXPECT_TRUE(m_FinnhubWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	TEST_F(CFinnhubWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_FinnhubWebInquiry.ReportStatus(1));
	}

	TEST_F(CFinnhubWebInquiryTest, TestPrepareNextInquiringStr) {
		CString str;
		gl_pWorldMarket->SetSystemReady(true);
		for (int i = 0; i < 4; i++) {
			if (m_FinnhubWebInquiry.PrepareNextInquiringString()) {
				str = m_FinnhubWebInquiry.GetInquiringString();
				EXPECT_STREQ(str.Right(27), _T(""));
			}
			gl_pWorldMarket->SetSystemReady(false);
		}
	}

	TEST_F(CFinnhubWebInquiryTest, TestGetNextInquiringMiddleStr) {
		EXPECT_STREQ(m_FinnhubWebInquiry.GetNextInquiringMiddleString(1, true), _T("")) << "此函数不执行任何动作";
	}
}