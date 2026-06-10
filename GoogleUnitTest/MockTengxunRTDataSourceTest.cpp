#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockTengxunRTDataSource.h"

using namespace testing;

namespace {
	CMockTengxunRTDataSourcePtr s_pMockTengxunRTDataSource; // 腾讯实时数据采集
}

namespace FireBirdTest {
	class CMockTengxunRTDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			gl_dataContainerChinaStock.SetTengxunRTDataInquiringIndex(0);
			s_pMockTengxunRTDataSource = make_shared<CMockTengxunRTDataSource>();
		}

		void TearDown() override {
			// clearUp
			s_pMockTengxunRTDataSource = nullptr;
			gl_pChinaMarket->SetSystemReady(true);
			gl_dataContainerChinaStock.SetTengxunRTDataInquiringIndex(0);

			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CTengxunRTDataSource TengxunRTDataSource;
	};

	TEST_F(CMockTengxunRTDataSourceTest, TestInitialize) {
		EXPECT_EQ(TengxunRTDataSource.GetInquiryFunction(), "http://qt.gtimg.cn/q=");
		EXPECT_EQ(TengxunRTDataSource.GetInquiryToken(), "");
		EXPECT_EQ(TengxunRTDataSource.GetInquiringNumber(), 900) << "腾讯默认值";
	}

	TEST_F(CMockTengxunRTDataSourceTest, TestGenerateInquiryMessage) {
		auto timePoint = chrono::time_point<chrono::steady_clock>();

		EXPECT_FALSE(s_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false); // 保证快速申请数据

		s_pMockTengxunRTDataSource->SetWebError(true);
		EXPECT_CALL(*s_pMockTengxunRTDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(timePoint))
		.WillOnce(Return(timePoint + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))
		.WillOnce(Return(timePoint + 1ms + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()));

		EXPECT_FALSE(s_pMockTengxunRTDataSource->GenerateInquiryMessage(120000));

		EXPECT_FALSE(s_pMockTengxunRTDataSource->GenerateInquiryMessage(120100)) << "继续等待";
		EXPECT_FALSE(s_pMockTengxunRTDataSource->IsInquiring());
		EXPECT_FALSE(s_pMockTengxunRTDataSource->HaveInquiry());

		EXPECT_TRUE(s_pMockTengxunRTDataSource->GenerateInquiryMessage(120600)) << "申请数据";

		EXPECT_TRUE(s_pMockTengxunRTDataSource->HaveInquiry());

		// 恢复原状
		gl_pChinaMarket->SetSystemReady(true);
	}
}
