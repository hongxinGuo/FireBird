///////////////////////////////////////////////////////////////////////////////////////////
//
// 所有使用Mock类测试ChinaMarket的函数皆位于此文件中。
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include"Thread.h"

#include"MockWorldMarket.h"

#include"FinnhubDataSource.h"
#include "TiingoDataSource.h"

using namespace testing;

namespace FireBirdTest {
	CMockWorldMarketPtr s_pMockWorldMarket;

	class CMockWorldMarketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();

			//EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());

			s_pMockWorldMarket = make_shared<CMockWorldMarket>(); // 在此生成，
			s_pMockWorldMarket->ResetMarket();
			EXPECT_FALSE(s_pMockWorldMarket->IsSystemReady()) << "市场默认为尚未准备好";
			s_pMockWorldMarket->SetSystemReady(true);// 测试系统默认为准备好了

			EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
			gl_systemMessage.PopInformationMessage();
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();

			while (!s_pMockWorldMarket->IsMarketTaskEmpty()) s_pMockWorldMarket->DiscardCurrentMarketTask();
		}

		static void TearDownTestSuite() {
			s_pMockWorldMarket = nullptr;

			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			//EXPECT_EQ(gl_dataContainerChinaStock.GetDayLineNeedUpdateNumber(), gl_dataContainerChinaStock.Size());
			SCOPED_TRACE("");
			GeneralCheck();
			gl_systemConfiguration.SetExitingSystem(false);
			gl_pFinnhubDataSource->SetUpdateStockProfile(true);
			EXPECT_EQ(gl_pFinnhubDataSource->InquiryQueueSize(), 0);
			EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());
		}

		void TearDown() override {
			// clearUp
			EXPECT_EQ(gl_pFinnhubDataSource->InquiryQueueSize(), 0);

			EXPECT_TRUE(s_pMockWorldMarket->IsSystemReady());
			EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());

			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CMockWorldMarketTest, TestUpdateToken) {
		s_pMockWorldMarket->UpdateToken();
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateTiingoIndustry) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateTiingoIndustry)
		.Times(1);
		EXPECT_EQ(ThreadUpdateTiingoIndustry(s_pMockWorldMarket), static_cast<UINT>(45));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateSICIndustry) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateSICIndustry)
		.Times(1);
		EXPECT_EQ(ThreadUpdateSICIndustry(s_pMockWorldMarket), static_cast<UINT>(46));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateNaicsIndustry) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateNaicsIndustry)
		.Times(1);
		EXPECT_EQ(ThreadUpdateNaicsIndustry(s_pMockWorldMarket), static_cast<UINT>(47));
	}

	TEST_F(CMockWorldMarketTest, TestTaskStartAllWebSocket) {
		EXPECT_TRUE(s_pMockWorldMarket->IsSystemReady());
		EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());
		//gl_pTiingoDataSource->SetErrorCode(10020); // 确保不调用tiingo WebSocket
		EXPECT_TRUE(gl_pFinnhubWebSocket->IsIdle());
		EXPECT_TRUE(gl_pTiingoIEXWebSocket->IsIdle());
		EXPECT_TRUE(gl_pTiingoCryptoWebSocket->IsIdle());
		EXPECT_TRUE(gl_pTiingoForexWebSocket->IsIdle());
		EXPECT_CALL(*s_pMockWorldMarket, StartFinnhubWebSocket).Times(1);
		EXPECT_CALL(*s_pMockWorldMarket, StartTiingoIEXWebSocket).Times(1);
		EXPECT_CALL(*s_pMockWorldMarket, StartTiingoCryptoWebSocket).Times(1);
		EXPECT_CALL(*s_pMockWorldMarket, StartTiingoForexWebSocket).Times(1);

		s_pMockWorldMarket->TaskStartAllWebSocket(10000);

		EXPECT_FALSE(s_pMockWorldMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_START_ALL_WEB_SOCKET__);
		EXPECT_EQ(pTask->GetTime(), 10100);

		// 恢复原状
		s_pMockWorldMarket->DiscardCurrentMarketTask();
		EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());
		gl_pTiingoDataSource->SetErrorCode(0);
	}
}
