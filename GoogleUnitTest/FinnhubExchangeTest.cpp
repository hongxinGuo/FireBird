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
		EXPECT_STREQ(Exchange.GetExchangeCode().c_str(), _T(" "));
		EXPECT_STREQ(Exchange.m_strName.c_str(), _T(" "));
		EXPECT_STREQ(Exchange.m_strMic.c_str(), _T(" "));
		EXPECT_STREQ(Exchange.m_strTimeZone.c_str(), _T(" "));
		EXPECT_STREQ(Exchange.m_strHour.c_str(), _T(" "));
		EXPECT_STREQ(Exchange.m_strCloseDate.c_str(), _T(" "));
		EXPECT_STREQ(Exchange.m_strCountry.c_str(), _T(""));
		EXPECT_STREQ(Exchange.m_strSource.c_str(), _T(""));
		EXPECT_FALSE(Exchange.m_fStockSymbolUpdated);
		EXPECT_FALSE(Exchange.m_fMarketStatusUpdated);
		EXPECT_FALSE(Exchange.m_fMarketHolidayUpdated);
	}

	TEST_F(CExchangeTest, TestIsStockSymbolUpdated) {
		CStockExchange finnhubExchange;

		EXPECT_FALSE(finnhubExchange.IsStockSymbolUpdated());
		finnhubExchange.SetStockSymbolUpdated(true);
		EXPECT_TRUE(finnhubExchange.IsStockSymbolUpdated());
	}

	TEST_F(CExchangeTest, TestIsMarketStatusUpdated) {
		CStockExchange finnhubExchange;

		EXPECT_FALSE(finnhubExchange.IsMarketStatusUpdated());
		finnhubExchange.SetMarketStatusUpdated(true);
		EXPECT_TRUE(finnhubExchange.IsMarketStatusUpdated());
	}

	TEST_F(CExchangeTest, TestIsMarketHolidayUpdated) {
		CStockExchange finnhubExchange;

		EXPECT_FALSE(finnhubExchange.IsMarketHolidayUpdated());
		finnhubExchange.SetMarketHolidayUpdated(true);
		EXPECT_TRUE(finnhubExchange.IsMarketHolidayUpdated());
	}

	TEST_F(CExchangeTest, TestAppend) {
		CSetStockExchange setExchange, setExchange2;
		CStockExchange Exchange, Exchange2;

		Exchange.SetExchangeCode(_T("AA"));
		Exchange.m_strName = _T("aaa");
		Exchange.m_strMic = _T("abdc");
		Exchange.m_strTimeZone = _T("Beijing");
		Exchange.m_strHour = _T("10101010");
		Exchange.m_strCloseDate = _T("20202020");
		Exchange.m_strCountry = _T("dfe");
		Exchange.m_strSource = _T("abc");
		Exchange.m_fStockSymbolUpdated = true;

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setExchange.Open();
		setExchange.m_pDatabase->BeginTrans();
		Exchange.Append(setExchange);
		setExchange.m_pDatabase->CommitTrans();
		setExchange.Close();

		setExchange2.m_strFilter = _T("[Code] = 'AA'");
		setExchange2.Open();
		setExchange2.m_pDatabase->BeginTrans();
		EXPECT_TRUE(!setExchange2.IsEOF()) << "此时已经存入了AA";
		Exchange2.Load(setExchange2);
		EXPECT_STREQ(Exchange.GetExchangeCode().c_str(), _T("AA"));
		EXPECT_STREQ(Exchange.m_strName.c_str(), _T("aaa"));
		EXPECT_STREQ(Exchange.m_strMic.c_str(), _T("abdc"));
		EXPECT_STREQ(Exchange.m_strTimeZone.c_str(), _T("Beijing"));
		EXPECT_STREQ(Exchange.m_strHour.c_str(), _T("10101010"));
		EXPECT_STREQ(Exchange.m_strCloseDate.c_str(), _T("20202020"));
		EXPECT_STREQ(Exchange.m_strCountry.c_str(), _T("dfe"));
		EXPECT_STREQ(Exchange.m_strSource.c_str(), _T("abc"));
		EXPECT_TRUE(Exchange.m_fStockSymbolUpdated) << "这个参数不存入数据库";
		setExchange2.Delete();
		setExchange2.m_pDatabase->CommitTrans();
		setExchange2.Close();
	}
}
