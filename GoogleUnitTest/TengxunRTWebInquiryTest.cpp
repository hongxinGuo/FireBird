#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"TengxunRTDataSource.h"
#include"MockTengxunRTWebInquiry.h"

using namespace testing;

namespace FireBirdTest {
	class CTengxunRTWebInquiryTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			m_TengxunRTWebInquiry.SetDataSource(gl_pTengxunRTDataSource.get());
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
		}

		void TearDown(void) override {
			// clearUp
			gl_pChinaMarket->SetSystemReady(false);
			gl_pChinaMarket->SetTengxunRTDataInquiringIndex(0);
		}

	public:
		CMockTengxunRTWebInquiry m_TengxunRTWebInquiry; // 腾讯实时数据采集
	};

	TEST_F(CTengxunRTWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiryFunction(), _T("http://qt.gtimg.cn/q="));
		EXPECT_STREQ(m_TengxunRTWebInquiry.GetInquiryToken(), _T(""));
		EXPECT_STREQ(m_TengxunRTWebInquiry.GetConnectionName(), _T("TengxunRT"));
		EXPECT_EQ(m_TengxunRTWebInquiry.GetInquiringNumber(), 900) << _T("腾讯默认值");
	}

	TEST_F(CTengxunRTWebInquiryTest, TestGetWebData) {
		m_TengxunRTWebInquiry.SetInquiringWebData(true);
		EXPECT_FALSE(m_TengxunRTWebInquiry.GetWebData());
		m_TengxunRTWebInquiry.SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_TengxunRTWebInquiry, StartReadingThread)
			.Times(1);
		m_TengxunRTWebInquiry.GetWebData();
		EXPECT_TRUE(m_TengxunRTWebInquiry.IsInquiringWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}

	TEST_F(CTengxunRTWebInquiryTest, TestGetNextInquiryStr) {
		gl_pChinaMarket->SetSystemReady(true);
		CString str = m_TengxunRTWebInquiry.GetNextInquiringMiddleString(1, false);
		EXPECT_STREQ(str, _T("sh000001"));
	}

	TEST_F(CTengxunRTWebInquiryTest, TestPrepareNextInquiringStr) {
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(m_TengxunRTWebInquiry.PrepareNextInquiringString());
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
