#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockSinaRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
	class CMockSinaRTDataSourceTest : public Test {
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
		}

		void TearDown(void) override {
			// clearUp
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
		}

	public:
		CMockSinaRTDataSource m_MockSinaRTDataSource; // ����ʵʱ���ݲɼ�
	};

	TEST_F(CMockSinaRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(m_MockSinaRTDataSource.GetHeaders(), _T("Referer:https://finance.sina.com.cn\r\n")) << "����ʵʱ���ݷ�������Ҫ�ṩ�˱�ͷ��Ϣ��RefererΪ���ã�User-Agent����ֻ����˵����ʽ";
		EXPECT_STREQ(m_MockSinaRTDataSource.GetInquiryFunction(), _T("https://hq.sinajs.cn/list=")) << "����ʵʱ���ݷ�������ʹ��https";
		EXPECT_STREQ(m_MockSinaRTDataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(m_MockSinaRTDataSource.GetInquiringNumber(), 850) << _T("����Ĭ��ֵ");
	}

	TEST_F(CMockSinaRTDataSourceTest, TestStartReadingThread) {
		EXPECT_FALSE(m_MockSinaRTDataSource.IsInquiringWebData());
		EXPECT_EQ(m_MockSinaRTDataSource.GetByteRead(), 0);
	}

	TEST_F(CMockSinaRTDataSourceTest, TestGetWebData) {
		m_MockSinaRTDataSource.SetInquiringWebData(true);
		EXPECT_FALSE(m_MockSinaRTDataSource.GetWebData());
		m_MockSinaRTDataSource.SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(m_MockSinaRTDataSource, StartReadingThread)
			.Times(1);
		m_MockSinaRTDataSource.GetWebData();
		EXPECT_TRUE(m_MockSinaRTDataSource.IsInquiringWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
	}
}
