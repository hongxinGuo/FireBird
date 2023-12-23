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
		EXPECT_FALSE(gl_systemConfiguration.IsNeedUpdate()) << "不允许更新系统配置";
		EXPECT_FALSE(gl_finnhubInaccessibleExchange.IsNeedUpdate()) << "不允许更新禁入交易所名单";
		EXPECT_TRUE(gl_systemConfiguration.IsUsingSinaRTServer());
		EXPECT_TRUE(gl_systemConfiguration.IsUsingNeteaseDayLineServer());
		if (gl_UpdateChinaMarketDB.try_acquire()) {
			gl_UpdateChinaMarketDB.release();
		}
		else {
			EXPECT_TRUE(false) << "gl_UpdateChinaMarketDB被锁住了";
		}
		if (gl_UpdateWorldMarketDB.try_acquire()) {
			gl_UpdateWorldMarketDB.release();
		}
		else {
			EXPECT_TRUE(false) << "gl_UpdateWorldMarketDB被锁住了";
		}

		EXPECT_FALSE(gl_systemConfiguration.IsDebugMode());
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountName(), _T("hxguo"));
		EXPECT_STREQ(gl_systemConfiguration.GetDatabaseAccountPassword(), _T("hxguo"));
		EXPECT_EQ(gl_systemConfiguration.GetBackgroundThreadPermittedNumber(), 8);

		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRealtimeServer(), 0) << "默认使用新浪实时数据服务器";
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketDayLineServer(), 0) << "默认使用网易日线数据服务器";
		EXPECT_EQ(gl_systemConfiguration.GetChinaMarketRTDataInquiryTime(), 200) << "默认查询时间为200毫秒";
		EXPECT_EQ(gl_systemConfiguration.GetSavingChinaMarketStockDayLineThread(), 4) << "默认查询股票历史数据工作线程数为4";
		EXPECT_EQ(gl_systemConfiguration.GetNumberOfRTDataSource(), 4) << "测试文件中的数值";
		EXPECT_EQ(gl_systemConfiguration.GetSinaRTDataInquiryPerTime(), 850) << "测试文件中的数值";
		EXPECT_EQ(gl_systemConfiguration.GetNeteaseRTDataInquiryPerTime(), 900) << "测试文件中的数值";
		EXPECT_EQ(gl_systemConfiguration.GetTengxunRTDataInquiryPerTime(), 900) << "测试文件中的数值";

		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketFinnhubInquiryTime(), 1100) << "默认每次查询时间为1100毫秒";
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketTiingoInquiryTime(), 3600000 / 400) << "默认每小时查询最大数量为400";
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketQuandlInquiryTime(), 3600000 / 100) << "默认每小时查询最大数量为100";

		EXPECT_EQ(gl_systemConfiguration.GetInsideTransactionUpdateRate(), 30);
		EXPECT_EQ(gl_systemConfiguration.GetStockProfileUpdateRate(), 365);
		EXPECT_EQ(gl_systemConfiguration.GetStockPeerUpdateRate(), 90);
		EXPECT_EQ(gl_systemConfiguration.GetStockBasicFinancialUpdateRate(), 45);
	}

	void ChinaMarketCheck() {
		if (gl_pChinaMarket != nullptr) {
			ASSERT_FALSE(gl_pChinaMarket->IsUpdateStockProfileDB()) << "不允许更新股票代码库";
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
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
			ASSERT_FALSE(gl_pWorldMarket->IsUpdateStockProfileDB()) << "不允许更新股票代码库";
			const CWorldStockPtr pStock = gl_pWorldMarket->GetStock(_T("AAPL"));
			EXPECT_TRUE(pStock->IsUpdateCompanyProfile());
			ASSERT_FALSE(pStock->IsUpdateProfileDB()) << "不允许更新股票代码库";
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
		EXPECT_EQ(gl_qSinaRT.Size(), 0);
		EXPECT_EQ(gl_qNeteaseRT.Size(), 0);
		EXPECT_EQ(gl_qTengxunRT.Size(), 0);
		EXPECT_EQ(gl_qDayLine.Size(), 0);
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
