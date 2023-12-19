#include"pch.h"

#include "TengxunDayLineDataSource.h"
#include "TengxunRTDataSource.h"

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

#include "GeneralCheck.h"

#include "FinnhubInaccessibleExchange.h"

using namespace testing;

namespace FireBirdTest {
	void SystemMessageCheck() {
		EXPECT_THAT(gl_systemMessage.InformationSize(), 0) << gl_systemMessage.PopInformationMessage();
		EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
		EXPECT_THAT(gl_systemMessage.DayLineInfoSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 0) << gl_systemMessage.PopErrorMessage();
	}

	void EnvironmentCheck() {
		ASSERT_FALSE(gl_systemConfiguration.IsWorkingMode());
		EXPECT_FALSE(gl_systemConfiguration.IsExitingSystem());
		time_t tt;
		time(&tt);
		EXPECT_TRUE((gl_tUTCTime + 3600) > tt) << gl_tUTCTime;
	}

	void SystemConfigurationCheck() {
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsNeedUpdate());
		EXPECT_FALSE(gl_systemConfiguration.IsNeedUpdate());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseDayLineServer());
		if (gl_UpdateChinaMarketDB.try_acquire()) {
			gl_UpdateChinaMarketDB.release();
		}
		else
			EXPECT_TRUE(false);
		if (gl_UpdateWorldMarketDB.try_acquire()) {
			gl_UpdateWorldMarketDB.release();
		}
		else
			EXPECT_TRUE(false);
	}

	void ChinaMarketCheck() {
		if (gl_pChinaMarket != nullptr) {
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_THAT(gl_pChinaMarket->IsUpdateStockProfileDB(), IsFalse());
			//EXPECT_THAT(gl_pChinaMarket->)
			EXPECT_THAT(gl_pChinaMarket->IsCalculatingDayLineRS(), IsFalse());
			EXPECT_THAT(gl_pChinaMarket->IsCalculatingWeekLineRS(), IsFalse());
			EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());
			EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040);
			EXPECT_EQ(gl_pChinaMarket->GetDayLineNeedUpdateNumber(), gl_pChinaMarket->GetTotalStock());
			EXPECT_EQ(gl_pChinaMarket->GetTotalStock(), 5040) << "测试数据库装载的股票数";

			EXPECT_TRUE(gl_pChinaMarket->IsMarketTaskEmpty());

			EXPECT_TRUE(gl_pChinaMarket->IsResetMarket());
			EXPECT_FALSE(gl_pChinaMarket->IsTodayTempRTDataLoaded());

			EXPECT_EQ(gl_pChinaMarket->GetStock(0)->GetDayLineEndDate(), 20210430) << "第一个股票（上海指数）的结束日期为20210430";
			EXPECT_TRUE(gl_pChinaMarket->GetStock(0)->IsIPOed()) << "第一个股票（上海指数）为IPOed（255）";
		}
	}

	void WorldMarketCheck() {
		if (gl_pWorldMarket != nullptr) {
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(_T("AAPL"));
			EXPECT_TRUE(pStock->IsUpdateCompanyProfile());
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			EXPECT_FALSE(pStock->IsDayLineNeedSaving());
			EXPECT_TRUE(pStock->IsDayLineNeedUpdate());

			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 15) << "最初装载了15个";
			EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), 11) << "最初装载了11个";

			EXPECT_TRUE(gl_pChinaMarket->IsSystemReady());

			EXPECT_TRUE(gl_pWorldMarket->IsMarketTaskEmpty()) << gl_pWorldMarket->GetMarketTask()->GetType();

			EXPECT_EQ(gl_pWorldMarket->GetStockSize(), 4847) << "默认状态下数据库总数为4847(全部上海股票和小部分美国股票)";
		}
	}

	void WebInquirerCheck() {
		EXPECT_TRUE(gl_pSinaRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pNeteaseRTDataSource->IsEnable());
		EXPECT_TRUE(gl_pTengxunRTDataSource->IsEnable());

		EXPECT_THAT(gl_pFinnhubDataSource->GetReceivedDataSize(), 0);
		EXPECT_EQ(gl_pTiingoDataSource->GetReceivedDataSize(), 0);
		EXPECT_EQ(gl_pSinaRTDataSource->GetReceivedDataSize(), 0);
		EXPECT_EQ(gl_pNeteaseDayLineDataSource->GetReceivedDataSize(), 0);
		EXPECT_EQ(gl_pNeteaseRTDataSource->GetReceivedDataSize(), 0);
		EXPECT_EQ(gl_pTengxunRTDataSource->GetReceivedDataSize(), 0);
		EXPECT_EQ(gl_pTengxunDayLineDataSource->GetReceivedDataSize(), 0);

		EXPECT_THAT(gl_pFinnhubDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pTiingoDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pSinaRTDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pNeteaseDayLineDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pNeteaseRTDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pTengxunRTDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pTengxunDayLineDataSource->InquiryQueueSize(), 0);

		EXPECT_FALSE(gl_pFinnhubDataSource->IsWebError());
		EXPECT_FALSE(gl_pTiingoDataSource->IsWebError());
		EXPECT_FALSE(gl_pSinaRTDataSource->IsWebError());
		EXPECT_FALSE(gl_pNeteaseDayLineDataSource->IsWebError());
		EXPECT_FALSE(gl_pSinaRTDataSource->IsWebError());
		EXPECT_FALSE(gl_pNeteaseRTDataSource->IsWebError());
		EXPECT_FALSE(gl_pTengxunRTDataSource->IsWebError());
		EXPECT_FALSE(gl_pTengxunDayLineDataSource->IsWebError());

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

		EXPECT_EQ(gl_pTiingoDataSource->InquiryQueueSize(), 0);
		EXPECT_EQ(gl_pFinnhubDataSource->InquiryQueueSize(), 0);

		EXPECT_THAT(gl_pTiingoCryptoWebSocket->DataSize(), 0);
		EXPECT_THAT(gl_pTiingoForexWebSocket->DataSize(), 0);
		EXPECT_THAT(gl_pTiingoIEXWebSocket->DataSize(), 0);
	}

	void DataContainerCheck() {
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->DayLineQueueSize(), 0);
	}

	void GeneralCheck() {
		SCOPED_TRACE("");

		EnvironmentCheck();
		SystemConfigurationCheck();
		SystemMessageCheck();
		ChinaMarketCheck();
		WorldMarketCheck();
		WebInquirerCheck();
		DataContainerCheck();
	}
}
