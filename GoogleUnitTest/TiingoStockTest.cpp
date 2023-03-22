#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoStock.h"

namespace FireBirdTest {
	class CTiingoStockTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			GeneralCheck();
		}

		void SetUp() override {
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			GeneralCheck();
		}
	};

	TEST_F(CTiingoStockTest, TestInitialize) {
		CTiingoStock tiingo;

		EXPECT_STREQ(tiingo.m_strTiingoPermaTicker, _T(""));
		EXPECT_STREQ(tiingo.m_strTicker, _T(""));
		EXPECT_STREQ(tiingo.m_strName, _T(""));
		EXPECT_FALSE(tiingo.m_fIsActive);
		EXPECT_FALSE(tiingo.m_fIsADR);
		EXPECT_EQ(tiingo.m_iSICCode, 0);
		EXPECT_STREQ(tiingo.m_strSICIndustry, _T(""));
		EXPECT_STREQ(tiingo.m_strSICSector, _T(""));
		EXPECT_STREQ(tiingo.m_strTiingoIndustry, _T(""));
		EXPECT_STREQ(tiingo.m_strTiingoSector, _T(""));
		EXPECT_STREQ(tiingo.m_strReportingCurrency, _T(""));
		EXPECT_STREQ(tiingo.m_strLocation, _T(""));
		EXPECT_STREQ(tiingo.m_strCompanyWebSite, _T(""));
		EXPECT_STREQ(tiingo.m_strSECFilingWebSite, _T(""));
		EXPECT_EQ(tiingo.m_lStatementUpdateDate, 19800101);
		EXPECT_EQ(tiingo.m_lDailyDataUpdateDate, 19800101);
	}

	TEST_F(CTiingoStockTest, TestSave) {
		CTiingoStock stock, stock2;
		CSetTiingoStock setTiingoStock;

		stock.m_strTiingoPermaTicker = _T("aasdfasdfj");
		stock.m_strTicker = _T("000001.US");
		stock.m_strName = _T("adkjkf");
		stock.m_fIsActive = true;
		stock.m_fIsADR = true;
		stock.m_iSICCode = 1234;
		stock.m_strSICIndustry = _T("defg");
		stock.m_strSICSector = _T("efg");
		stock.m_strTiingoIndustry = _T("ghi");
		stock.m_strTiingoSector = _T("defghijk");
		stock.m_strReportingCurrency = _T("US");
		stock.m_strLocation = _T("Irvine");
		stock.m_strCompanyWebSite = _T("ijk");
		stock.m_strSECFilingWebSite = _T("https://def.com");
		stock.m_lDailyDataUpdateDate = 20202020;
		stock.m_lStatementUpdateDate = 10101010;

		setTiingoStock.Open();
		stock.Append(setTiingoStock);
		setTiingoStock.Close();

		setTiingoStock.m_strFilter = _T("[Ticker] = '000001.US'");
		setTiingoStock.Open();
		stock2.Load(setTiingoStock);
		setTiingoStock.Delete();
		setTiingoStock.Close();

		EXPECT_STREQ(stock.m_strTiingoPermaTicker, stock2.m_strTiingoPermaTicker);
		EXPECT_STREQ(stock.m_strTicker, stock2.m_strTicker);
		EXPECT_STREQ(stock.m_strName, stock2.m_strName);
		EXPECT_TRUE(stock.m_fIsActive == stock2.m_fIsActive);
		EXPECT_TRUE(stock.m_fIsADR == stock2.m_fIsADR);
		EXPECT_EQ(stock.m_iSICCode, stock2.m_iSICCode);
		EXPECT_STREQ(stock.m_strSICIndustry, stock2.m_strSICIndustry);
		EXPECT_STREQ(stock.m_strSICSector, stock2.m_strSICSector);
		EXPECT_STREQ(stock.m_strTiingoIndustry, stock2.m_strTiingoIndustry);
		EXPECT_STREQ(stock.m_strTiingoSector, stock2.m_strTiingoSector);
		EXPECT_STREQ(stock.m_strReportingCurrency, stock2.m_strReportingCurrency);
		EXPECT_STREQ(stock.m_strLocation, stock2.m_strLocation);
		EXPECT_STREQ(stock.m_strCompanyWebSite, stock2.m_strCompanyWebSite);
		EXPECT_STREQ(stock.m_strSECFilingWebSite, stock2.m_strSECFilingWebSite);
		EXPECT_EQ(stock.m_lDailyDataUpdateDate, stock2.m_lDailyDataUpdateDate);
		EXPECT_EQ(stock.m_lStatementUpdateDate, stock2.m_lStatementUpdateDate);
	}
}
