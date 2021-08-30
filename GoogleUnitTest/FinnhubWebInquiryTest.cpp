#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"MockFinnhubWebInquiry.h"

using namespace std;
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
			gl_pWorldMarket->CalculateTime();
		}

		virtual void TearDown(void) override {
			// clearup
			gl_pWorldMarket->SetResetMarket(true);
			gl_pWorldMarket->SetSystemReady(false);
			GeneralCheck();
		}
		CMockFinnhubWebInquiry m_FinnhubWebInquiry; // 网易日线历史数据
	};

	TEST_F(CFinnhubWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_FinnhubWebInquiry.GetInquiringStringPrefix(), _T(""));
#ifdef  DEBUG
		EXPECT_STREQ(m_FinnhubWebInquiry.GetInquiringStringSuffix(), _T("&token=c1i57rv48v6vit20lrc0"));
#else
		EXPECT_STREQ(m_FinnhubWebInquiry.GetInquiringStringSuffix(), _T("&token=bv985d748v6ujthqfke0"));
#endif //  DEBUG
		EXPECT_STREQ(m_FinnhubWebInquiry.GetConnectionName(), _T("Finnhub"));
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
			if (m_FinnhubWebInquiry.PrepareNextInquiringStr()) {
				str = m_FinnhubWebInquiry.GetInquiringString();
#ifdef  DEBUG
				EXPECT_STREQ(str.Right(27), _T("&token=c1i57rv48v6vit20lrc0"));
#else
				EXPECT_STREQ(str.Right(27), _T("&token=bv985d748v6ujthqfke0"));
#endif //  DEBUG
			}
			else EXPECT_EQ(str.GetLength(), 0);
		}
		gl_pWorldMarket->SetSystemReady(false);
	}

	TEST_F(CFinnhubWebInquiryTest, TestGetNextInquiringMiddleStr) {
		EXPECT_STREQ(m_FinnhubWebInquiry.GetNextInquiringMiddleStr(), _T("")) << "此函数不执行任何动作";
	}
}