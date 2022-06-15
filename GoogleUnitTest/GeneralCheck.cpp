#include"pch.h"

using namespace testing;

#include"globedef.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"
#include"SystemMessage.h"
#include"WebInquirer.h"

#include"WebRTDataContainer.h"

#include "GeneralCheck.h"

namespace StockAnalysisTest {
	void SystemMessageCheck(void) {
		EXPECT_THAT(gl_systemMessage.InformationSize(), 0) << gl_systemMessage.PopInformationMessage();
		EXPECT_THAT(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
		EXPECT_THAT(gl_systemMessage.DayLineInfoSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_THAT(gl_systemMessage.ErrorMessageSize(), 0) << gl_systemMessage.PopErrorMessage();
	}

	void EnvironmentCheck(void) {
		ASSERT_FALSE(gl_fNormalMode);

		EXPECT_FALSE(gl_fExitingSystem);
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
			EXPECT_FALSE(pStock->IsProfileUpdated());
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			EXPECT_FALSE(pStock->IsDayLineNeedSaving());
			EXPECT_TRUE(pStock->IsDayLineNeedUpdate());

			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 14) << "最初装载了14个";
			EXPECT_EQ(gl_pWorldMarket->GetForexExchangeSize(), 10) << "最初装载了10个";
		}
	}

	void WebInquirerCheck(void) {
		EXPECT_THAT(gl_pDataSourceFinnhub->GetReceivedDataSize(), 0);
		EXPECT_EQ(gl_pDataSourceTiingo->GetReceivedDataSize(), 0);
		EXPECT_THAT(gl_WebInquirer.TiingoCryptoWebSocketDataSize(), 0);
		EXPECT_THAT(gl_WebInquirer.TiingoForexWebSocketDataSize(), 0);
		EXPECT_THAT(gl_WebInquirer.TiingoIEXWebSocketDataSize(), 0);

		EXPECT_FALSE(gl_pDataSourceFinnhub->IsCountryListUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsCryptoDayLineUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsForexDayLineUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsStockDayLineUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsStockProfileUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsCryptoExchangeUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsForexExchangeUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsCryptoSymbolUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsForexSymbolUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsSymbolUpdated());

		EXPECT_FALSE(gl_pDataSourceFinnhub->IsCryptoExchangeUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsForexExchangeUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsCryptoSymbolUpdated());
		EXPECT_FALSE(gl_pDataSourceFinnhub->IsForexSymbolUpdated());

		EXPECT_FALSE(gl_pDataSourceFinnhub->IsStockProfileUpdated());

		EXPECT_FALSE(gl_pDataSourceTiingo->IsStockSymbolUpdated());
		EXPECT_FALSE(gl_pDataSourceTiingo->IsCryptoSymbolUpdated());
	}

	void DataContainerCheck(void) {
		EXPECT_EQ(gl_WebRTDataContainer.NeteaseDataSize(), 0);
		EXPECT_EQ(gl_WebRTDataContainer.TengxunDataSize(), 0);
		EXPECT_EQ(gl_WebRTDataContainer.SinaDataSize(), 0);
		EXPECT_EQ(gl_WebInquirer.GetParsedNeteaseDayLineDataSize(), 0);
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