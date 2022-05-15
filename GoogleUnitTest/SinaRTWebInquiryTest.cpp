#include"pch.h"

#include"globedef.h"
#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockSinaRTWebInquiry.h"

using namespace std;
#include<atomic>

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CSinaRTWebInquiryTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
		}

		static void TearDownTestSuite(void) {
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			GeneralCheck();
		}

		virtual void SetUp(void) override {
			GeneralCheck();
		}

		virtual void TearDown(void) override {
			// clearup
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
		}
	public:
		CMockSinaRTWebInquiry m_SinaRTWebInquiry; // ����ʵʱ���ݲɼ�
	};

	TEST_F(CSinaRTWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_SinaRTWebInquiry.GetHeaders(), _T("User-Agent:FireBird\r\nReferer:https://finance.sina.com.cn\r\n")) << "����ʵʱ���ݷ�������Ҫ�ṩ�˱�ͷ��Ϣ��RefererΪ���ã�User-Agent����ֻ����˵����ʽ";
		EXPECT_STREQ(m_SinaRTWebInquiry.GetInquiringStringPrefix(), _T("https://hq.sinajs.cn/list=")) << "����ʵʱ���ݷ�������ʹ��https";
		EXPECT_STREQ(m_SinaRTWebInquiry.GetInquiringStringSuffix(), _T(""));
		EXPECT_FALSE(m_SinaRTWebInquiry.IsReportStatus());
		EXPECT_EQ(m_SinaRTWebInquiry.GetInquiringNumber(), 850) << _T("����Ĭ��ֵ");
	}

	TEST_F(CSinaRTWebInquiryTest, TestStartReadingThread) {
		EXPECT_FALSE(m_SinaRTWebInquiry.IsReadingWebData());
		EXPECT_EQ(m_SinaRTWebInquiry.GetByteReaded(), 0);
	}

	TEST_F(CSinaRTWebInquiryTest, TestGetWebData) {
		m_SinaRTWebInquiry.SetReadingWebData(true);
		EXPECT_FALSE(m_SinaRTWebInquiry.GetWebData());
		m_SinaRTWebInquiry.SetReadingWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_SinaRTWebInquiry, StartReadingThread)
			.Times(1);
		m_SinaRTWebInquiry.GetWebData();
		EXPECT_TRUE(m_SinaRTWebInquiry.IsReadingWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
	}

	TEST_F(CSinaRTWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_SinaRTWebInquiry.ReportStatus(1));
	}
}