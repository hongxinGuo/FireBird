#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockSinaRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockSinaRTDataSourcePtr m_pMockSinaRTDataSource; // ����ʵʱ���ݲɼ�

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
			m_pMockSinaRTDataSource = make_shared<CMockSinaRTDataSource>();
		}

		void TearDown(void) override {
			// clearUp
			m_pMockSinaRTDataSource = nullptr;
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
			GeneralCheck();
		}

	public:
		CSinaRTDataSource SinaDataSource;
	};

	TEST_F(CMockSinaRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(SinaDataSource.GetHeaders(), _T("Referer:https://finance.sina.com.cn\r\n")) << "����ʵʱ���ݷ�������Ҫ�ṩ�˱�ͷ��Ϣ��RefererΪ���ã�User-Agent����ֻ����˵����ʽ";
		EXPECT_STREQ(SinaDataSource.GetInquiryFunction(), _T("https://hq.sinajs.cn/list=")) << "����ʵʱ���ݷ�������ʹ��https";
		EXPECT_STREQ(SinaDataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(SinaDataSource.GetInquiringNumber(), 850) << _T("����Ĭ��ֵ");
	}

	TEST_F(CMockSinaRTDataSourceTest, TestStartReadingThread) {
		EXPECT_FALSE(SinaDataSource.IsInquiringWebData());
		EXPECT_EQ(SinaDataSource.GetByteRead(), 0);
	}

	TEST_F(CMockSinaRTDataSourceTest, TestGetWebData) {
		m_pMockSinaRTDataSource->SetInquiringWebData(false);
		gl_pChinaMarket->SetSystemReady(true);
		EXPECT_CALL(*m_pMockSinaRTDataSource, StartReadingThread)
		.Times(1);
		m_pMockSinaRTDataSource->ProcessInquiryMessage();
		EXPECT_TRUE(m_pMockSinaRTDataSource->IsInquiringWebData()) << _T("�˱�־�ɹ����̸߳������á��˴����õ���Mock�࣬�ʶ��˱�ʶû������");
	}
}
