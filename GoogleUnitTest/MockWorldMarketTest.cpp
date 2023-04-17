///////////////////////////////////////////////////////////////////////////////////////////
//
// ����ʹ��Mock�����ChinaMarket�ĺ�����λ�ڴ��ļ��С�
//
///////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"

#include"GeneralCheck.h"

#include"Thread.h"

#include"MockWorldMarket.h"

#include"ProductFinnhubCompanyInsiderTransaction.h"

#include"PRoductFinnhubForexSymbol.h"
#include"FinnhubDataSource.h"
#include "TiingoDataSource.h"

using namespace testing;
#include<memory>

namespace FireBirdTest {
	CMockWorldMarketPtr s_pMockWorldMarket;

	class CMockWorldMarketTest : public Test {
	protected:
		static void SetUpTestSuite() {
			GeneralCheck();

			//EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());

			s_pMockWorldMarket = make_shared<CMockWorldMarket>(); // �ڴ����ɣ�
			s_pMockWorldMarket->ResetMarket();
			EXPECT_FALSE(s_pMockWorldMarket->IsSystemReady()) << "�г�Ĭ��Ϊ��δ׼����";
			s_pMockWorldMarket->SetSystemReady(true);// ����ϵͳĬ��Ϊ׼������

			EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
			gl_systemMessage.PopInformationMessage();
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();

			while (!s_pMockWorldMarket->IsMarketTaskEmpty()) s_pMockWorldMarket->DiscardMarketTask();
		}

		static void TearDownTestSuite() {
			s_pMockWorldMarket = nullptr;

			GeneralCheck();
		}

		void SetUp() override {
			//EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			GeneralCheck();
			gl_systemStatus.SetExitingSystem(false);
			gl_pFinnhubDataSource->SetUpdateStockProfile(true);
			EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 0);
			EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());
		}

		void TearDown() override {
			// clearUp
			EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 0);

			EXPECT_TRUE(s_pMockWorldMarket->IsSystemReady());
			EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());

			GeneralCheck();
		}
	};

	TEST_F(CMockWorldMarketTest, TestUpdateToken) {
		s_pMockWorldMarket->UpdateToken();
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateCountryListDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateCountryListDB).Times(1);

		EXPECT_EQ(ThreadUpdateCountryListDB(s_pMockWorldMarket.get()), static_cast<UINT>(40));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateStockProfileDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateStockProfileDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateWorldMarketStockProfileDB(s_pMockWorldMarket.get()), static_cast<UINT>(37));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateStockDayLineDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateStockDayLineDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateWorldStockDayLineDB(s_pMockWorldMarket.get()), static_cast<UINT>(42));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateForexSymbolDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateForexSymbolDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateForexSymbolDB(s_pMockWorldMarket.get()), static_cast<UINT>(39));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateForexExchangedDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateForexExchangeDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateForexExchangeDB(s_pMockWorldMarket.get()), static_cast<UINT>(49));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateFinnhubCryptoSymbolDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateFinnhubCryptoSymbolDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateFinnhubCryptoSymbolDB(s_pMockWorldMarket.get()), static_cast<UINT>(52));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateCryptoExchangedDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateCryptoExchangeDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateCryptoExchangeDB(s_pMockWorldMarket.get()), static_cast<UINT>(51));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateInsiderTransactionDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateInsiderTransactionDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateInsiderTransactionDB(s_pMockWorldMarket.get()), static_cast<UINT>(48));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateInsiderSentimentDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateInsiderSentimentDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateInsiderSentimentDB(s_pMockWorldMarket.get()), static_cast<UINT>(58));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateTiingoStockDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateTiingoStockDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateTiingoStockDB(s_pMockWorldMarket.get()), static_cast<UINT>(44));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateTiingoIndustry) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateTiingoIndustry)
		.Times(1);
		EXPECT_EQ(ThreadUpdateTiingoIndustry(s_pMockWorldMarket.get()), static_cast<UINT>(45));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateSICIndustry) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateSICIndustry)
		.Times(1);
		EXPECT_EQ(ThreadUpdateSICIndustry(s_pMockWorldMarket.get()), static_cast<UINT>(46));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateNaicsIndustry) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateNaicsIndustry)
		.Times(1);
		EXPECT_EQ(ThreadUpdateNaicsIndustry(s_pMockWorldMarket.get()), static_cast<UINT>(47));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateDayLineStartEndDate) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateStockDayLineStartEndDate)
		.Times(1);
		EXPECT_EQ(ThreadUpdateWorldStockDayLineStartEndDate(s_pMockWorldMarket.get()), static_cast<UINT>(43));
	}

	TEST_F(CMockWorldMarketTest, TestThreadUpdateEconmicCalendarDB) {
		EXPECT_CALL(*s_pMockWorldMarket, UpdateEconomicCalendarDB)
		.Times(1);
		EXPECT_EQ(ThreadUpdateEconomicCalendarDB(s_pMockWorldMarket.get()), static_cast<UINT>(50));
	}

	TEST_F(CMockWorldMarketTest, TestTaskMonitoringWebSocketStatus) {
		EXPECT_TRUE(s_pMockWorldMarket->IsSystemReady());
		EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());
		//gl_pTiingoDataSource->SetErrorCode(10020); // ȷ��������tiingo WebSocket
		EXPECT_TRUE(gl_finnhubWebSocket.IsIdle());
		EXPECT_TRUE(gl_tiingoIEXWebSocket.IsIdle());
		EXPECT_TRUE(gl_tiingoCryptoWebSocket.IsIdle());
		EXPECT_TRUE(gl_tiingoForexWebSocket.IsIdle());
		EXPECT_CALL(*s_pMockWorldMarket, StartFinnhubWebSocket).Times(1);
		EXPECT_CALL(*s_pMockWorldMarket, StartTiingoIEXWebSocket).Times(1);
		EXPECT_CALL(*s_pMockWorldMarket, StartTiingoCryptoWebSocket).Times(1);
		EXPECT_CALL(*s_pMockWorldMarket, StartTiingoForexWebSocket).Times(1);

		s_pMockWorldMarket->TaskMonitoringWebSocketStatus(10000);

		EXPECT_FALSE(s_pMockWorldMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_MONITORING_WEB_SOCKET_STATUS__);
		EXPECT_EQ(pTask->GetTime(), 10100);

		// �ָ�ԭ״
		s_pMockWorldMarket->DiscardMarketTask();
		EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());
		gl_pTiingoDataSource->SetErrorCode(0);
	}

	TEST_F(CMockWorldMarketTest, TestTaskUpdateStockProfileDB1) {
		EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());

		EXPECT_CALL(*s_pMockWorldMarket, CreateThreadUpdateCountryListDB).Times(0);
		EXPECT_CALL(*s_pMockWorldMarket, CreateThreadUpdateForexExchangeDB).Times(0);
		EXPECT_CALL(*s_pMockWorldMarket, CreateThreadUpdateForexSymbolDB).Times(0);
		EXPECT_CALL(*s_pMockWorldMarket, CreateThreadUpdateStockProfileDB).Times(0);

		s_pMockWorldMarket->TaskUpdateStockProfileDB(165259);

		EXPECT_FALSE(s_pMockWorldMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__);
		EXPECT_EQ(pTask->GetTime(), 165759) << "ÿ����Ӹ���һ��";

		s_pMockWorldMarket->DiscardMarketTask();
	}

	TEST_F(CMockWorldMarketTest, TestTaskUpdateStockProfileDB2) {
		EXPECT_TRUE(s_pMockWorldMarket->IsMarketTaskEmpty());

		EXPECT_CALL(*s_pMockWorldMarket, CreateThreadUpdateCountryListDB).Times(0);
		EXPECT_CALL(*s_pMockWorldMarket, CreateThreadUpdateForexExchangeDB).Times(0);
		EXPECT_CALL(*s_pMockWorldMarket, CreateThreadUpdateForexSymbolDB).Times(0);
		EXPECT_CALL(*s_pMockWorldMarket, CreateThreadUpdateStockProfileDB).Times(0);

		s_pMockWorldMarket->TaskUpdateStockProfileDB(165400);

		EXPECT_FALSE(s_pMockWorldMarket->IsMarketTaskEmpty());
		const auto pTask = s_pMockWorldMarket->GetMarketTask();
		EXPECT_EQ(pTask->GetType(), WORLD_MARKET_UPDATE_STOCK_PROFILE_DB__);
		EXPECT_EQ(pTask->GetTime(), 170510) << "170000ϵͳ��Ҫ�������Ƴ���170510���¿�ʼ����";

		s_pMockWorldMarket->DiscardMarketTask();
	}
}
