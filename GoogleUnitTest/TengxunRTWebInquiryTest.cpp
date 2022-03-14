#include"pch.h"

#include"globedef.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockTengxunRTWebInquiry.h"

using namespace std;
using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CTengxunRTWebInquiryTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
		}

		virtual void TearDown(void) override {
			// clearup
			gl_pChinaMarket->SetSystemReady(false);
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
		}
	public:
		CMockTengxunRTWebInquiry m_TengxunRTWebInquiry; // 腾讯实时数据采集
	};

	TEST_F(CTengxunRTWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiringStringPrefix(), _T("http://qt.gtimg.cn/q="));
		EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiringStringSuffix(), _T(""));
		EXPECT_STREQ(m_TengxunRTWebInquiry.GetConnectionName(), _T("TengxunRT"));
		EXPECT_THAT(m_TengxunRTWebInquiry.GetShortestInquiringInterval(), 100) << "默认100毫秒查询一次";
		EXPECT_EQ(m_TengxunRTWebInquiry.GetInquiringNumber(), 900) << _T("腾讯默认值");
	}

	TEST_F(CTengxunRTWebInquiryTest, TestGetWebData) {
		m_TengxunRTWebInquiry.SetReadingWebData(true);
		EXPECT_FALSE(m_TengxunRTWebInquiry.GetWebData());
		m_TengxunRTWebInquiry.SetReadingWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_TengxunRTWebInquiry, StartReadingThread)
			.Times(1);
		m_TengxunRTWebInquiry.GetWebData();
		EXPECT_TRUE(m_TengxunRTWebInquiry.IsReadingWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	TEST_F(CTengxunRTWebInquiryTest, TestGetNextInquiryStr) {
		gl_pChinaMarket->SetSystemReady(true);
		CString str = m_TengxunRTWebInquiry.GetNextInquiringMiddleStr(1);
		EXPECT_STREQ(str, _T("sh000001"));
	}

	TEST_F(CTengxunRTWebInquiryTest, TestPrepareNextInquiringStr) {
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(m_TengxunRTWebInquiry.PrepareNextInquiringStr());
		CString str = m_TengxunRTWebInquiry.GetInquiringString();
		EXPECT_STREQ(str.Left(21), _T("http://qt.gtimg.cn/q="));
		gl_pChinaMarket->SetSystemReady(false);
	}

	TEST_F(CTengxunRTWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_TengxunRTWebInquiry.ReportStatus(1));
	}

	TEST_F(CTengxunRTWebInquiryTest, TestIsTengxunRTDataInValid) {
		CWebDataPtr pWebDataReceived;
		pWebDataReceived = make_shared<CWebData>();
		CString str = _T("v_pv_none_match=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_TRUE(m_TengxunRTWebInquiry.IsTengxunRTDataInvalid(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);

		str = _T("v_pv_none_mtch=\"1\";\n");
		pWebDataReceived->SetData(str.GetBuffer(), str.GetLength(), 0);
		pWebDataReceived->SetBufferLength(str.GetLength());
		pWebDataReceived->ResetCurrentPos();

		EXPECT_FALSE(m_TengxunRTWebInquiry.IsTengxunRTDataInvalid(*pWebDataReceived));
		EXPECT_EQ(pWebDataReceived->GetCurrentPos(), 0);
	}
}