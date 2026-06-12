#include"pch.h"

#include"WorldMarket.h"

#include"MockQuandlDataSource.h"
#include"VirtualWebProduct.h"

#include"GeneralCheck.h"

using namespace testing;

namespace {
	bool s_fSystemStatus;
	CMockQuandlDataSourcePtr s_pMockQuandlDataSource; // 网易日线历史数据
}

namespace FireBirdTest {
	class CMockQuandlDataSourceTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
			s_fSystemStatus = gl_pWorldMarket->IsSystemReady();
			gl_pWorldMarket->SetSystemReady(true); // Quandl引擎必须等待系统初始化后才可使用。
		}

		static void TearDownTestSuite() {
			gl_pWorldMarket->SetSystemReady(s_fSystemStatus);
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			gl_pWorldMarket->CalculateTime();
			s_pMockQuandlDataSource = make_shared<CMockQuandlDataSource>();
		}

		void TearDown() override {
			// clearUp
			s_pMockQuandlDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
		CQuandlDataSource QuandlDataSource;
	};

	TEST_F(CMockQuandlDataSourceTest, TestInitialize) {
		EXPECT_EQ(QuandlDataSource.GetInquiryFunction(), "");
		EXPECT_EQ(QuandlDataSource.GetInquirySuffix(), "&api_key=");
		EXPECT_EQ(QuandlDataSource.GetInquiryToken(), "") << "默认令牌在系统配置文件中，此处初始化时为空";
	}

	TEST_F(CMockQuandlDataSourceTest, TestGenerateInquiryMessage1) {
		auto timePoint = chrono::time_point<chrono::steady_clock>();
		auto p = make_shared<CVirtualWebProduct>();

		EXPECT_FALSE(s_pMockQuandlDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());

		s_pMockQuandlDataSource->SetWebError(true);
		EXPECT_CALL(*s_pMockQuandlDataSource, GetTickCount()).Times(2)
		.WillOnce(Return(timePoint + gl_systemConfiguration.GetWorldMarketQuandlInquiryTime()))
		.WillOnce(Return(timePoint + 1ms + gl_systemConfiguration.GetWorldMarketQuandlInquiryTime()));
		EXPECT_CALL(*s_pMockQuandlDataSource, Inquire()).Times(1)
		.WillRepeatedly(DoAll([p]() {
			s_pMockQuandlDataSource->SetInquiring(true);
			s_pMockQuandlDataSource->StoreInquiry(p);
		}, Return(true)));

		EXPECT_FALSE(s_pMockQuandlDataSource->GenerateInquiryMessage(toTimeOfDay(120500))) << "时间未到，继续等待";
		EXPECT_FALSE(s_pMockQuandlDataSource->IsInquiring());
		EXPECT_FALSE(s_pMockQuandlDataSource->HaveInquiry());
		EXPECT_TRUE(s_pMockQuandlDataSource->GenerateInquiryMessage(toTimeOfDay(120500))) << "申请数据";

		EXPECT_TRUE(s_pMockQuandlDataSource->IsInquiring());
		EXPECT_TRUE(s_pMockQuandlDataSource->HaveInquiry());
	}
}
