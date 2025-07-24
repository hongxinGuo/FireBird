#include"pch.h"

#include"GeneralCheck.h"

#include"StockExchange.h"

namespace FireBirdTest {
	class CExchangeTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite() { // ��������ĳ�ʼ������
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
		Exchange.m_fUpdateStockSymbol = false;

		ASSERT(!gl_systemConfiguration.IsWorkingMode());
		setExchange.Open();
		setExchange.m_pDatabase->BeginTrans();
		Exchange.Append(setExchange);
		setExchange.m_pDatabase->CommitTrans();
		setExchange.Close();

		setExchange2.m_strFilter = _T("[Code] = 'AA'");
		setExchange2.Open();
		setExchange2.m_pDatabase->BeginTrans();
		EXPECT_TRUE(!setExchange2.IsEOF()) << "��ʱ�Ѿ�������AA";
		Exchange2.Load(setExchange2);
		EXPECT_STREQ(Exchange.GetExchangeCode().c_str(), _T("AA"));
		EXPECT_STREQ(Exchange.m_strName.c_str(), _T("aaa"));
		EXPECT_STREQ(Exchange.m_strMic.c_str(), _T("abdc"));
		EXPECT_STREQ(Exchange.m_strTimeZone.c_str(), _T("Beijing"));
		EXPECT_STREQ(Exchange.m_strHour.c_str(), _T("10101010"));
		EXPECT_STREQ(Exchange.m_strCloseDate.c_str(), _T("20202020"));
		EXPECT_STREQ(Exchange.m_strCountry.c_str(), _T("dfe"));
		EXPECT_STREQ(Exchange.m_strSource.c_str(), _T("abc"));
		EXPECT_FALSE(Exchange.m_fUpdateStockSymbol) << "����������������ݿ�";
		setExchange2.Delete();
		setExchange2.m_pDatabase->CommitTrans();
		setExchange2.Close();
	}
}
