#include"pch.h"

#include"ChinaMarket.h"
#include"GeneralCheck.h"

#include"MockNeteaseRTDataSource.h"
#include "ProductNeteaseRT.h"

using namespace testing;

namespace {
	CMockNeteaseRTDataSourcePtr s_pMockNeteaseRTDataSource; // 网易实时数据采集
}

namespace FireBirdTest {
	class CMockNeteaseRTDataSourceTest : public ::testing::Test {
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
			s_pMockNeteaseRTDataSource = make_shared<CMockNeteaseRTDataSource>();
		}

		void TearDown() override {
			// clearUp
			s_pMockNeteaseRTDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockNeteaseRTDataSourceTest, TestInitialize) {
		EXPECT_EQ(s_pMockNeteaseRTDataSource->GetInquiryFunction(), "http://api.money.126.net/data/feed/");
		EXPECT_EQ(s_pMockNeteaseRTDataSource->GetInquiryToken(), "");
		EXPECT_EQ(s_pMockNeteaseRTDataSource->GetInquiringNumber(), 900) << "DEBUG模式下网易默认值";
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestGenerateInquiryMessage) {
		auto timePoint = chrono::time_point<chrono::steady_clock>();
		auto p = make_shared<CVirtualWebProduct>();

		EXPECT_FALSE(s_pMockNeteaseRTDataSource->IsInquiring());
		EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		gl_pChinaMarket->SetSystemReady(false); // 保证快速申请数据

		EXPECT_CALL(*s_pMockNeteaseRTDataSource, GetTickCount()).Times(3)
		.WillOnce(Return(timePoint))
		.WillOnce(Return(timePoint + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()))
		.WillOnce(Return(timePoint + 1ms + gl_systemConfiguration.GetChinaMarketRTDataInquiryTime()));

		EXPECT_FALSE(s_pMockNeteaseRTDataSource->GenerateInquiryMessage(toTimeOfDay(120000)));
		EXPECT_FALSE(s_pMockNeteaseRTDataSource->IsInquiring());

		EXPECT_FALSE(s_pMockNeteaseRTDataSource->GenerateInquiryMessage(toTimeOfDay(120100))) << "时间未到，继续等待";
		EXPECT_FALSE(s_pMockNeteaseRTDataSource->IsInquiring());
		EXPECT_TRUE(s_pMockNeteaseRTDataSource->GenerateInquiryMessage(toTimeOfDay(120600))) << "申请数据";

		EXPECT_TRUE(s_pMockNeteaseRTDataSource->HaveInquiry());

		// 恢复原状
		gl_pChinaMarket->SetSystemReady(true);
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestInquireRTData) {
		s_pMockNeteaseRTDataSource->SetInquiring(false);

		s_pMockNeteaseRTDataSource->Inquire(toTimeOfDay(1010));

		EXPECT_EQ(s_pMockNeteaseRTDataSource->InquiryQueueSize(), 1);
		const auto pProduct = s_pMockNeteaseRTDataSource->GetCurrentProduct();
		EXPECT_STREQ(typeid(*pProduct).name(), "class CProductNeteaseRT");
	}

	TEST_F(CMockNeteaseRTDataSourceTest, TestPrepareNextInquiringStr) {
		gl_pChinaMarket->SetSystemReady(true);
		const auto p = make_shared<CProductNeteaseRT>();
		p->SetInquiryFunction("http://api.money.126.net/data/feed/");
		s_pMockNeteaseRTDataSource->SetCurrentInquiry(p);
		s_pMockNeteaseRTDataSource->CreateCurrentInquireString();
		const string str = s_pMockNeteaseRTDataSource->GetInquiringString();
		EXPECT_EQ(str.substr(0,35), "http://api.money.126.net/data/feed/");
	}
}
