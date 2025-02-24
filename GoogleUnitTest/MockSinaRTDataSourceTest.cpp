#include"pch.h"

#include"ThreadStatus.h"
#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockSinaRTDataSource.h"
#include "WebData.h"

using namespace testing;

namespace FireBirdTest {
	CMockSinaRTDataSourcePtr m_pMockSinaRTDataSource; // 新浪实时数据采集

	class CMockSinaRTDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			gl_dataContainerChinaStock.SetSinaRTDataInquiringIndex(0);
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
			gl_dataContainerChinaStock.SetSinaRTDataInquiringIndex(0);
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

	TEST_F(CMockSinaRTDataSourceTest, TestGenerateInquiryMessage) {
		auto timePoint = chrono::time_point<chrono::steady_clock>();

		EXPECT_FALSE(m_pMockSinaRTDataSource->IsInquiring());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false); // 保证快速申请数据

		m_pMockSinaRTDataSource->SetWebError(true);
		EXPECT_CALL(*m_pMockSinaRTDataSource, GetTickCount()).Times(2)
		.WillOnce(Return(timePoint))
		.WillOnce(Return(timePoint + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()));
		//.WillOnce(Return(timePoint + 1ms + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()));

		EXPECT_FALSE(m_pMockSinaRTDataSource->GenerateInquiryMessage(120000));
		EXPECT_FALSE(m_pMockSinaRTDataSource->IsInquiring());
		EXPECT_FALSE(m_pMockSinaRTDataSource->HaveInquiry());

		EXPECT_TRUE(m_pMockSinaRTDataSource->GenerateInquiryMessage(120600)) << "申请数据";

		EXPECT_TRUE(m_pMockSinaRTDataSource->IsInquiring());
		EXPECT_TRUE(m_pMockSinaRTDataSource->HaveInquiry());

		// 恢复原状
		gl_pChinaMarket->SetSystemReady(true);
	}

	TEST_F(CMockSinaRTDataSourceTest, TestIsAErrorMessageData1) {
		CWebDataPtr pwd = make_shared<CWebData>();
		pwd->Test_SetBuffer_(_T("Forbidden"));
		EXPECT_EQ(SinaDataSource.GetHTTPStatusCode(), 0);
		SinaDataSource.SetHTTPStatusCode(200);

		EXPECT_EQ(ERROR_NO_ERROR__, SinaDataSource.IsAErrorMessageData(pwd)) << "HTTPStatusCode == 200，无视错误代码，正常返回";

		// 恢复原状
		SinaDataSource.SetHTTPStatusCode(0);
	}

	TEST_F(CMockSinaRTDataSourceTest, TestIsAErrorMessageData2) {
		CWebDataPtr pwd = make_shared<CWebData>();
		pwd->Test_SetBuffer_(_T("Forbidden"));
		EXPECT_EQ(SinaDataSource.GetHTTPStatusCode(), 0);

		EXPECT_EQ(ERROR_SINA_HEADER_NEEDED__, SinaDataSource.IsAErrorMessageData(pwd));
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		// 恢复原状
		gl_systemMessage.PopInnerSystemInformationMessage();
	}
}
