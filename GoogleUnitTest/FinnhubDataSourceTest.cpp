#include"pch.h"

#include"GeneralCheck.h"

#include"FinnhubInquiryType.h"
#include"MockFinnhubDataSource.h"

#include"ProductFinnhubCompanyProfile.h"
#include"ProductFinnhubCompanyProfileConcise.h"
//#include"ProductFinnhubForexExchange.h"
//#include"ProductFinnhubEconomicCountryList.h"
//#include"ProductFinnhubEconomicCalendar.h"
//#include"ProductFinnhubCompanyProfileConcise.h"
#include"ProductFinnhubCompanyPeer.h"
//#include"ProductFinnhubCompanyInsiderTransaction.h"
#include"ProductFinnhubCryptoDayLine.h"
#include"ProductFinnhubForexDayLine.h"
#include"ProductFinnhubStockDayLine.h"
#include"ProductFinnhubForexSymbol.h"
#include"ProductFinnhubCryptoSymbol.h"
#include"ProductFinnhubStockSymbol.h"
#include"ProductFinnhubStockEstimatesEPSSurprise.h"
#include"ProductFinnhubStockPriceQuote.h"

#include"ProductTiingoStockSymbol.h"
#include"ProductDummy.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubDataSourceTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
			ASSERT_THAT(gl_pFinnhubDataSource, NotNull());
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override { }

		void TearDown(void) override {
			// clearUp

			GeneralCheck();
		}

	protected:
		CFinnhubDataSource m_FinnhubDataSource;
	};

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCountryList) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCountryList());
		m_FinnhubDataSource.SetUpdateCountryList(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCountryList());
		m_FinnhubDataSource.SetUpdateCountryList(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCountryList());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateSymbol) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateSymbol());
		m_FinnhubDataSource.SetUpdateSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateSymbol());
		m_FinnhubDataSource.SetUpdateSymbol(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateSymbol());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateStockProfile) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockProfile());
		m_FinnhubDataSource.SetUpdateStockProfile(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockProfile());
		m_FinnhubDataSource.SetUpdateStockProfile(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockProfile());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCompanyNews) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCompanyNews());
		m_FinnhubDataSource.SetUpdateCompanyNews(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCompanyNews());
		m_FinnhubDataSource.SetUpdateCompanyNews(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCompanyNews());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCompanyPriceMetrics) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCompanyPriceMetrics());
		m_FinnhubDataSource.SetUpdateCompanyPriceMetrics(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCompanyPriceMetrics());
		m_FinnhubDataSource.SetUpdateCompanyPriceMetrics(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCompanyPriceMetrics());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateStockBasicFinancial) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockBasicFinancial());
		m_FinnhubDataSource.SetUpdateStockBasicFinancial(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockBasicFinancial());
		m_FinnhubDataSource.SetUpdateStockBasicFinancial(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockBasicFinancial());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateStockDayLine) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockDayLine());
		m_FinnhubDataSource.SetUpdateStockDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateStockDayLine());
		m_FinnhubDataSource.SetUpdateStockDayLine(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateStockDayLine());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateForexExchange) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexExchange());
		m_FinnhubDataSource.SetUpdateForexExchange(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateForexExchange());
		m_FinnhubDataSource.SetUpdateForexExchange(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexExchange());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateForexSymbol) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexSymbol());
		m_FinnhubDataSource.SetUpdateForexSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateForexSymbol());
		m_FinnhubDataSource.SetUpdateForexSymbol(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexSymbol());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateForexDayLine) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexDayLine());
		m_FinnhubDataSource.SetUpdateForexDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateForexDayLine());
		m_FinnhubDataSource.SetUpdateForexDayLine(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateForexDayLine());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCryptoSymbol) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoSymbol());
		m_FinnhubDataSource.SetUpdateCryptoSymbol(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCryptoSymbol());
		m_FinnhubDataSource.SetUpdateCryptoSymbol(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoSymbol());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCryptoExchange) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoExchange());
		m_FinnhubDataSource.SetUpdateCryptoExchange(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCryptoExchange());
		m_FinnhubDataSource.SetUpdateCryptoExchange(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoExchange());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateCryptoDayLine) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoDayLine());
		m_FinnhubDataSource.SetUpdateCryptoDayLine(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateCryptoDayLine());
		m_FinnhubDataSource.SetUpdateCryptoDayLine(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateCryptoDayLine());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdatePeer) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdatePeer());
		m_FinnhubDataSource.SetUpdatePeer(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdatePeer());
		m_FinnhubDataSource.SetUpdatePeer(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdatePeer());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateInsiderTransaction) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateInsiderTransaction());
		m_FinnhubDataSource.SetUpdateInsiderTransaction(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateInsiderTransaction());
		m_FinnhubDataSource.SetUpdateInsiderTransaction(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateInsiderTransaction());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateInsiderSentiment) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateInsiderSentiment());
		m_FinnhubDataSource.SetUpdateInsiderSentiment(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateInsiderSentiment());
		m_FinnhubDataSource.SetUpdateInsiderSentiment(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateInsiderSentiment());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateEconomicCalendar) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateEconomicCalendar());
		m_FinnhubDataSource.SetUpdateEconomicCalendar(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateEconomicCalendar());
		m_FinnhubDataSource.SetUpdateEconomicCalendar(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateEconomicCalendar());
	}

	TEST_F(CFinnhubDataSourceTest, TestIsUpdateEPSSurprise) {
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateEPSSurprise());
		m_FinnhubDataSource.SetUpdateEPSSurprise(false);
		EXPECT_FALSE(m_FinnhubDataSource.IsUpdateEPSSurprise());
		m_FinnhubDataSource.SetUpdateEPSSurprise(true);
		EXPECT_TRUE(m_FinnhubDataSource.IsUpdateEPSSurprise());
	}
}
