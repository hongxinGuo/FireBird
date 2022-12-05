#include"pch.h"

using namespace testing;

#include"WorldMarket.h"
#include"ChinaMarket.h"

#include"FinnhubDataSource.h"
#include"TiingoDataSource.h"

#include"SinaRTDataSource.h"
#include"NeteaseRTDataSource.h"
#include"TengxunRTDataSource.h"
#include"NeteaseDaylineDataSource.h"

#include "GeneralCheck.h"

namespace StockAnalysisTest {
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

	void ChinaMarketCheck(void) {
		if (gl_pChinaMarket != nullptr) {
			EXPECT_EQ(gl_pChinaMarket->GetCurrentStock(), nullptr) << gl_pChinaMarket->GetCurrentStock()->GetSymbol();
			EXPECT_THAT(gl_pChinaMarket->IsUpdateStockCodeDB(), IsFalse());
			//EXPECT_THAT(gl_pChinaMarket->)
		}
	}

	void WorldMarketCheck(void) {
		if (gl_pWorldMarket != nullptr) {
			CWorldStockPtr pStock = gl_pWorldMarket->GetStock(_T("AAPL"));
			EXPECT_FALSE(pStock->IsCompanyProfileUpdated());
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			EXPECT_FALSE(pStock->IsDayLineNeedSaving());
			EXPECT_TRUE(pStock->IsDayLineNeedUpdate());

			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 14) << "最初装载了14个";
			EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), 10) << "最初装载了10个";
		}
	}

	void WebInquirerCheck(void) {
		EXPECT_THAT(gl_pFinnhubDataSource->GetReceivedDataSize(), 0);
		EXPECT_EQ(gl_pTiingoDataSource->GetReceivedDataSize(), 0);
		EXPECT_THAT(gl_tiingoCryptoWebSocket.DataSize(), 0);
		EXPECT_THAT(gl_tiingoForexWebSocket.DataSize(), 0);
		EXPECT_THAT(gl_tiingoIEXWebSocket.DataSize(), 0);

		EXPECT_FALSE(gl_pFinnhubDataSource->IsCountryListUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsCryptoDayLineUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsForexDayLineUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsStockDayLineUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsStockProfileUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsCryptoExchangeUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsForexExchangeUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsCryptoSymbolUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsForexSymbolUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsSymbolUpdated());

		EXPECT_FALSE(gl_pFinnhubDataSource->IsCryptoExchangeUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsForexExchangeUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsCryptoSymbolUpdated());
		EXPECT_FALSE(gl_pFinnhubDataSource->IsForexSymbolUpdated());

		EXPECT_FALSE(gl_pFinnhubDataSource->IsStockProfileUpdated());

		EXPECT_FALSE(gl_pTiingoDataSource->IsStockSymbolUpdated());
		EXPECT_FALSE(gl_pTiingoDataSource->IsCryptoSymbolUpdated());

		EXPECT_EQ(gl_pTiingoDataSource->GetInquiryQueueSize(), 0);
		EXPECT_EQ(gl_pFinnhubDataSource->GetInquiryQueueSize(), 0);
	}

	void DataContainerCheck(void) {
		EXPECT_EQ(gl_pChinaMarket->SinaRTSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->NeteaseRTSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->TengxunRTSize(), 0);
		EXPECT_EQ(gl_pChinaMarket->NeteaseDayLineSize(), 0);
	}

	void GeneralCheck(void) {
		EnvironmentCheck();
		SystemMessageCheck();
		ChinaMarketCheck();
		WorldMarketCheck();
		WebInquirerCheck();
		DataContainerCheck();
	}
}