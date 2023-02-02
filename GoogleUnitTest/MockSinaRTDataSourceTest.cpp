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
		CMockSinaRTDataSource m_MockSinaRTDataSource; // 新浪实时数据采集
	};

	TEST_F(CMockSinaRTDataSourceTest, TestInitialize) {
		EXPECT_STREQ(m_MockSinaRTDataSource.GetHeaders(), _T("Referer:https://finance.sina.com.cn\r\n")) << "新浪实时数据服务器需要提供此报头信息，Referer为有用，User-Agent部分只用于说明格式";
		EXPECT_STREQ(m_MockSinaRTDataSource.GetInquiryFunction(), _T("https://hq.sinajs.cn/list=")) << "新浪实时数据服务器已使用https";
		EXPECT_STREQ(m_MockSinaRTDataSource.GetInquiryToken(), _T(""));
		EXPECT_EQ(m_MockSinaRTDataSource.GetInquiringNumber(), 850) << _T("新浪默认值");
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
		EXPECT_TRUE(m_MockSinaRTDataSource.IsInquiringWebData()) << _T("此标志由工作线程负责重置。此处调用的是Mock类，故而此标识没有重置");
	}
}
