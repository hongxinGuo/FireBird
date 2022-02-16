#include"pch.h"

using namespace testing;

#include"globedef.h"
#include"WorldMarket.h"
#include"ChinaMarket.h"
#include"SystemMessage.h"
#include"WebInquirer.h"

#include "GeneralCheck.h"

namespace StockAnalysisTest {
	void SystemMessageCheck(void) {
		EXPECT_THAT(gl_systemMessage.GetInformationDequeSize(), 0) << gl_systemMessage.PopInformationMessage();
		EXPECT_THAT(gl_systemMessage.GetInnerSystemInformationDequeSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
		EXPECT_THAT(gl_systemMessage.GetDayLineInfoDequeSize(), 0) << gl_systemMessage.PopDayLineInfoMessage();
		EXPECT_THAT(gl_systemMessage.GetErrorMessageDequeSize(), 0) << gl_systemMessage.PopErrorMessage();
	}

	void EnvironmentCheck(void) {
		ASSERT_FALSE(gl_fNormalMode);
		ASSERT_TRUE(gl_fTestMode);

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
			EXPECT_FALSE(gl_pWorldMarket->IsFinnhubCryptoExchangeUpdated());
			EXPECT_FALSE(gl_pWorldMarket->IsFinnhubForexExchangeUpdated());
			EXPECT_FALSE(gl_pWorldMarket->IsFinnhubCryptoSymbolUpdated());
			EXPECT_FALSE(gl_pWorldMarket->IsFinnhubForexSymbolUpdated());

			EXPECT_FALSE(gl_pWorldMarket->IsStockProfileNeedUpdate());
			EXPECT_FALSE(gl_pWorldMarket->IsTiingoStockSymbolUpdated());
			EXPECT_FALSE(gl_pWorldMarket->IsTiingoCryptoSymbolUpdated());

			EXPECT_TRUE(gl_pWorldMarket->IsRecordFinnhubWebSocket());
			EXPECT_TRUE(gl_pWorldMarket->IsRecordTiingoIEXWebSocket());
			EXPECT_TRUE(gl_pWorldMarket->IsRecordTiingoForexWebSocket());
			EXPECT_TRUE(gl_pWorldMarket->IsRecordTiingoCryptoWebSocket());

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
		EXPECT_EQ(gl_WebInquirer.GetNeteaseDayLineDataSize(), 0);
		EXPECT_EQ(gl_WebInquirer.GetSinaRTDataSize(), 0);
		EXPECT_EQ(gl_WebInquirer.GetNeteaseRTDataSize(), 0);
		EXPECT_THAT(gl_WebInquirer.GetFinnhubDataSize(), 0);
		EXPECT_THAT(gl_WebInquirer.GetTiingoDataSize(), 0);
		EXPECT_THAT(gl_WebInquirer.GetTiingoCryptoWebSocketDataSize(), 0);
		EXPECT_THAT(gl_WebInquirer.GetTiingoForexWebSocketDataSize(), 0);
		EXPECT_THAT(gl_WebInquirer.GetTiingoIEXWebSocketDataSize(), 0);

		EXPECT_EQ(gl_pSinaRTWebInquiry->GetShortestInquiringInterval(), 100);
	}

	void GeneralCheck(void) {
		EnvironmentCheck();
		SystemMessageCheck();
		ChinaMarketCheck();
		WorldMarketCheck();
		WebInquirerCheck();
	}
}