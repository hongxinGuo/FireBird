#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"NeteaseRTDataSource.h"
#include"MockNeteaseRTWebInquiry.h"


#include<atomic>

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CNeteaseRTWebInquiryTest : public ::testing::Test
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

			m_NeteaseRTWebInquiry.SetDataSource(gl_pNeteaseRTDataSource.get());
			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
		}

		virtual void TearDown(void) override {
			// clearup
			gl_pChinaMarket->SetSystemReady(false);
			GeneralCheck();
		}
		CMockNeteaseRTWebInquiry m_NeteaseRTWebInquiry; // ����ʵʱ���ݲɼ�
	};

	TEST_F(CNeteaseRTWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiryFunction(), _T("http://api.money.126.net/data/feed/"));
		EXPECT_STREQ(m_NeteaseRTWebInquiry.GetInquiryToken(), _T(""));
		EXPECT_STREQ(m_NeteaseRTWebInquiry.GetConnectionName(), _T("NeteaseRT"));
		EXPECT_EQ(m_NeteaseRTWebInquiry.GetInquiringNumber(), 900) << _T("DEBUGģʽ������Ĭ��ֵ");
	}

	TEST_F(CNeteaseRTWebInquiryTest, TestGetWebData) {
		m_NeteaseRTWebInquiry.SetReadingWebData(true);
		EXPECT_FALSE(m_NeteaseRTWebInquiry.GetWebData());
		m_NeteaseRTWebInquiry.SetReadingWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_NeteaseRTWebInquiry, StartReadingThread)
			.Times(1);
		m_NeteaseRTWebInquiry.GetWebData();
		EXPECT_TRUE(m_NeteaseRTWebInquiry.IsReadingWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
	}

	TEST_F(CNeteaseRTWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_NeteaseRTWebInquiry.ReportStatus(1));
	}

	TEST_F(CNeteaseRTWebInquiryTest, TestPrepareNextInquiringStr) {
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_TRUE(m_NeteaseRTWebInquiry.PrepareNextInquiringString());
		CString str = m_NeteaseRTWebInquiry.GetInquiringString();
		EXPECT_STREQ(str.Left(35), _T("http://api.money.126.net/data/feed/"));
	}
}