#include"pch.h"

#include"GeneralCheck.h"

#include"VirtualWebProduct.h"
#include"TiingoDataSource.h"
#include"WorldMarket.h"

#include"MockTiingoDataSource.h"

using namespace testing;

namespace {
	CMockTiingoDataSourcePtr s_pTiingoDataSource;
}

namespace FireBirdTest {
	class CMockTiingoDataSourceTest : public Test {
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
			s_pTiingoDataSource = make_shared<CMockTiingoDataSource>();
		}

		void TearDown() override {
			// clearUp
			s_pTiingoDataSource = nullptr;
			SCOPED_TRACE("");
			GeneralCheck();
		}

	protected:
	};

	TEST_F(CMockTiingoDataSourceTest, TestGenerateInquiryMessage1) {
		auto timePoint = chrono::time_point<chrono::steady_clock>();
		auto p = make_shared<CVirtualWebProduct>();

		EXPECT_FALSE(s_pTiingoDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_TRUE(gl_systemConfiguration.IsPaidTypeTiingoAccount());

		InSequence Seq;
		EXPECT_CALL(*s_pTiingoDataSource, GetTickCount()).Times(2)
		.WillOnce(Return(timePoint + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()))
		.WillOnce(Return(timePoint + 1ms + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateMarketNews()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateFundamentalDefinition()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateCompanySymbol()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateCryptoSymbol()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateIEXTopOfBook()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateChosenStockDayLine()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateDayLine()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateStockDailyMeta()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateFinancialState()).Times(1)
		.WillRepeatedly(DoAll([p]() {
			s_pTiingoDataSource->SetInquiring(true);
			s_pTiingoDataSource->StoreInquiry(p);
		}, Return(true)));

		EXPECT_FALSE(s_pTiingoDataSource->GenerateInquiryMessage(toLocalTime(120500))) << "时间未到，继续等待";
		EXPECT_FALSE(s_pTiingoDataSource->IsInquiring());
		EXPECT_FALSE(s_pTiingoDataSource->HaveInquiry());
		EXPECT_TRUE(s_pTiingoDataSource->GenerateInquiryMessage(toLocalTime(120500))) << "已过五分钟，申请数据";

		EXPECT_TRUE(s_pTiingoDataSource->IsInquiring());
		EXPECT_TRUE(s_pTiingoDataSource->HaveInquiry());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 0) << gl_systemMessage.PopInformationMessage();

		//恢复原状
		//gl_systemMessage.PopInformationMessage();
	}

	TEST_F(CMockTiingoDataSourceTest, TestGenerateInquiryMessage2) {
		auto timePoint = chrono::time_point<chrono::steady_clock>();

		auto p = make_shared<CVirtualWebProduct>();

		EXPECT_FALSE(s_pTiingoDataSource->IsInquiring());
		EXPECT_TRUE(gl_pWorldMarket->IsSystemReady());
		EXPECT_TRUE(gl_systemConfiguration.IsPaidTypeTiingoAccount());
		gl_systemConfiguration.ChangeTiingoAccountTypeToFree();

		InSequence Seq;
		EXPECT_CALL(*s_pTiingoDataSource, GetTickCount()).Times(2)
		.WillOnce(Return(timePoint + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()))
		.WillOnce(Return(timePoint + 1ms + gl_systemConfiguration.GetWorldMarketTiingoInquiryTime()));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateMarketNews()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateFundamentalDefinition()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateCompanySymbol()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateCryptoSymbol()).Times(1)
		.WillOnce(Return(false));
		EXPECT_CALL(*s_pTiingoDataSource, GenerateIEXTopOfBook()).Times(1)
		.WillRepeatedly(DoAll([p]() {
			s_pTiingoDataSource->SetInquiring(true);
			s_pTiingoDataSource->StoreInquiry(p);
		}, Return(true)));

		EXPECT_FALSE(s_pTiingoDataSource->GenerateInquiryMessage(toLocalTime(120000) )) << "时间未到，继续等待";
		EXPECT_FALSE(s_pTiingoDataSource->IsInquiring());
		EXPECT_FALSE(s_pTiingoDataSource->HaveInquiry());
		EXPECT_TRUE(s_pTiingoDataSource->GenerateInquiryMessage(toLocalTime(120500))) << "已过五分钟，申请数据";

		EXPECT_TRUE(s_pTiingoDataSource->IsInquiring());
		EXPECT_TRUE(s_pTiingoDataSource->HaveInquiry());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 0) << gl_systemMessage.PopInformationMessage();

		//恢复原状
		gl_systemConfiguration.ChangeTiingoAccountTypeToPaid();
		gl_systemConfiguration.SetUpdateDB(false);
		//gl_systemMessage.PopInformationMessage();
	}
}
