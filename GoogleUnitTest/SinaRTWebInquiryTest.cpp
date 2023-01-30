#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"SinaRTDataSource.h"
#include"MockSinaRTWebInquiry.h"

using namespace testing;

namespace FireBirdTest {
	class CSinaRTWebInquiryTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
		}

		static void TearDownTestSuite(void) {
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			GeneralCheck();
		}

		void SetUp(void) override {
			GeneralCheck();

			m_SinaRTWebInquiry.SetDataSource(gl_pSinaRTDataSource.get());
		}

		void TearDown(void) override {
			// clearUp
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
		}

	public:
		CMockSinaRTWebInquiry m_SinaRTWebInquiry; // ����ʵʱ���ݲɼ�
	};

	TEST_F(CSinaRTWebInquiryTest, TestInitialize) {
		EXPECT_STREQ(m_SinaRTWebInquiry.GetHeaders(), _T("Referer:https://finance.sina.com.cn\r\n")) << "����ʵʱ���ݷ�������Ҫ�ṩ�˱�ͷ��Ϣ��RefererΪ���ã�User-Agent����ֻ����˵����ʽ";
		EXPECT_STREQ(m_SinaRTWebInquiry.GetInquiryFunction(), _T("https://hq.sinajs.cn/list=")) << "����ʵʱ���ݷ�������ʹ��https";
		EXPECT_STREQ(m_SinaRTWebInquiry.GetInquiryToken(), _T(""));
		EXPECT_FALSE(m_SinaRTWebInquiry.IsReportStatus());
		EXPECT_EQ(m_SinaRTWebInquiry.GetInquiringNumber(), 850) << _T("����Ĭ��ֵ");
	}

	TEST_F(CSinaRTWebInquiryTest, TestStartReadingThread) {
		EXPECT_FALSE(m_SinaRTWebInquiry.IsInquiringWebData());
		EXPECT_EQ(m_SinaRTWebInquiry.GetByteRead(), 0);
	}

	TEST_F(CSinaRTWebInquiryTest, TestGetWebData) {
		m_SinaRTWebInquiry.SetInquiringWebData(true);
		EXPECT_FALSE(m_SinaRTWebInquiry.GetWebData());
		m_SinaRTWebInquiry.SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_SinaRTWebInquiry, StartReadingThread)
			.Times(1);
		m_SinaRTWebInquiry.GetWebData();
		EXPECT_TRUE(m_SinaRTWebInquiry.IsInquiringWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
	}

	TEST_F(CSinaRTWebInquiryTest, TestReportStatus) {
		EXPECT_TRUE(m_SinaRTWebInquiry.ReportStatus(1));
	}
}
