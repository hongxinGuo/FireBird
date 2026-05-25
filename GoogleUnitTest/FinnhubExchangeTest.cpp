#include"pch.h"

#include"GeneralCheck.h"

#include"StockExchange.h"

namespace FireBirdTest {
	class CExchangeTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // 本测试类的初始化函数
			SCOPED_TRACE("");
			GeneralCheck();
		}

		static void TearDownTestSuite() {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}
	};

	TEST_F(CExchangeTest, TestInitialize) {
		const CStockExchange Exchange;
		EXPECT_EQ(Exchange.GetExchangeCode(), " ");
		EXPECT_EQ(Exchange.m_strName, " ");
		EXPECT_EQ(Exchange.m_strMic, " ");
		EXPECT_EQ(Exchange.m_strTimeZone, " ");
		EXPECT_EQ(Exchange.m_strHour, " ");
		EXPECT_EQ(Exchange.m_strCloseDate, " ");
		EXPECT_EQ(Exchange.m_strCountry, "");
		EXPECT_EQ(Exchange.m_strSource, "");
		EXPECT_TRUE(Exchange.m_fUpdateStockSymbol);
		EXPECT_TRUE(Exchange.m_fUpdateMarketStatus);
		EXPECT_TRUE(Exchange.m_fUpdateMarketHoliday);
	}

	TEST_F(CExchangeTest, TestIsUpdateStockSymbol) {
		CStockExchange finnhubExchange;

		EXPECT_TRUE(finnhubExchange.IsUpdateStockSymbol());
		finnhubExchange.SetUpdateStockSymbol(false);
		EXPECT_FALSE(finnhubExchange.IsUpdateStockSymbol());
	}

	TEST_F(CExchangeTest, TestIsUpdateMarketStatus) {
		CStockExchange finnhubExchange;

		EXPECT_TRUE(finnhubExchange.IsUpdateMarketStatus());
		finnhubExchange.SetUpdateMarketStatus(false);
		EXPECT_FALSE(finnhubExchange.IsUpdateMarketStatus());
	}

	TEST_F(CExchangeTest, TestIsMarketHolidayUpdated) {
		CStockExchange finnhubExchange;

		EXPECT_TRUE(finnhubExchange.IsUpdateMarketHoliday());
		finnhubExchange.SetUpdateMarketHoliday(false);
		EXPECT_FALSE(finnhubExchange.IsUpdateMarketHoliday());
	}
}
