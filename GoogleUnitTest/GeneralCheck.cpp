#include"pch.h"

using namespace testing;

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

#include "GeneralCheck.h"

namespace FireBirdTest {
	void SystemMessageCheck(void) {
		EXPECT_THAT(gl_systemMessage.InformationSize(), 0) << gl_systemMessage.PopInformationMessage();
		EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
		EXPECT_THAT(gl_systemMessage.DayLineInfoSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 0) << gl_systemMessage.PopErrorMessage();
	}

	void EnvironmentCheck(void) {
		ASSERT_FALSE(gl_systemStatus.IsWorkingMode());

		EXPECT_FALSE(gl_systemStatus.IsExitingSystem());
	}

	void SystemConfigurationCheck() {
		//EXPECT_FALSE(gl_systemConfiguration.IsNeedUpdate());
	}

	void ChinaMarketCheck(void) {
		if (gl_pChinaMarket != nullptr) {
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse());
			//EXPECT_THAT(gl_pChinaMarket->)
			EXPECT_THAT(gl_pChinaMarket->IsCalculatingDayLineRS(), IsFalse());
			EXPECT_THAT(gl_pChinaMarket->IsCalculatingWeekLineRS(), IsFalse());
			EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		}
	}

	void WorldMarketCheck(void) {
		if (gl_pWorldMarket != nullptr) {
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(_T("AAPL"));
			EXPECT_TRUE(pStock->IsUpdateCompanyProfile());
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			EXPECT_FALSE(pStock->IsDayLineNeedSaving());
			EXPECT_TRUE(pStock->IsDayLineNeedUpdate());

			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 14) << "最初装载了14个";
			EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), 10) << "最初装载了10个";

			EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
		}
	}

	void WebInquirerCheck(void) {
		EXPECT_THAT(gl_pFinnhubDataSource->GetReceivedDataSize(), 0);
		EXPECT_EQ(gl_pTiingoDataSource->GetReceivedDataSize(), 0);
		EXPECT_THAT(gl_tiingoCryptoWebSocket.DataSize(), 0);
		EXPECT_THAT(gl_tiingoForexWebSocket.DataSize(), 0);
		EXPECT_THAT(gl_tiingoIEXWebSocket.DataSize(), 0);

		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCountryList());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoDayLine());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateForexDayLine());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateStockDayLine());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateStockProfile());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateForexExchange());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoSymbol());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateForexSymbol());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateSymbol());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdatePeer());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCompanyNews());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateInsiderSentiment());
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateInsiderTransaction());

		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateStockSymbol());
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateCryptoSymbol());

		EXPECT_EQ(gl_pTiingoDataSource->GetInquiryQueueSize(), 0);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 0);
	}

	void DataContainerCheck(void) {
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->DayLineSize(), 0);
	}

	void GeneralCheck(void) {
		EnvironmentCheck();
		SystemConfigurationCheck();
		SystemMessageCheck();
		ChinaMarketCheck();
		WorldMarketCheck();
		WebInquirerCheck();
		DataContainerCheck();
	}
}
