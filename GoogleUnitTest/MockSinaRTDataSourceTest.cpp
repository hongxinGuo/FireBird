#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockSinaRTDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockSinaRTDataSourcePtr m_pMockSinaRTDataSource; // 新浪实时数据采集

	class CMockSinaRTDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
		}

		static void TearDownTestSuite() {
			while (gl_ThreadStatus.IsSavingThreadRunning()) Sleep(1);
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			m_pMockSinaRTDataSource = make_shared<CMockSinaRTDataSource>();
		}

		void TearDown() override {
			// clearUp
			m_pMockSinaRTDataSource = nullptr;
			gl_pChinaMarket->SetSinaStockRTDataInquiringIndex(0);
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		CSinaRTDataSource SinaDataSource;
	};

	TEST_F(CMockSinaRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(SinaDataSource.GetHeaders(), _T("Referer:https://finance.sina.com.cn\r\n")) << "新浪实时数据服务器需要提供此报头信息，Referer为有用，User-Agent部分只用于说明格式";
		EXPECT_STREQ(SinaDataSource.GetInquiryFunction(), _T("https://hq.sinajs.cn/list=")) << "新浪实时数据服务器已使用https";
		EXPECT_STREQ(SinaDataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(SinaDataSource.GetInquiringNumber(), 850) << _T("新浪默认值");
	}

	TEST_F(CMockSinaRTDataSourceTest, TestStartReadingThread) {
		EXPECT_EQ(SinaDataSource.GetByteRead(), 0);
	}

	TEST_F(CMockSinaRTDataSourceTest, TestGenerateInquiryMessage) {
		EXPECT_FALSE(m_pMockSinaRTDataSource->IsInquiring());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false); // 保证快速申请数据

		m_pMockSinaRTDataSource->SetErrorCode(12002);
		EXPECT_CALL(*m_pMockSinaRTDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(0))
		.WillOnce(Return(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))
		.WillOnce(Return(1 + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()));

		EXPECT_FALSE(m_pMockSinaRTDataSource->GenerateInquiryMessage(120000));

		EXPECT_FALSE(m_pMockSinaRTDataSource->GenerateInquiryMessage(120100)) << "继续等待";
		EXPECT_FALSE(m_pMockSinaRTDataSource->IsInquiring());
		EXPECT_FALSE(m_pMockSinaRTDataSource->HaveInquiry());
		EXPECT_TRUE(m_pMockSinaRTDataSource->GenerateInquiryMessage(120600)) << "申请数据";

		// 恢复原状
		gl_pChinaMarket->SetSystemReady(true);
	}
}
