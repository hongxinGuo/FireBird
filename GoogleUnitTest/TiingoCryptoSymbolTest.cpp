#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoCryptoSymbol.h"

namespace FireBirdTest {
	class CTiingoCryptoSymbolTest : public ::testing::Test {
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

	TEST_F(CTiingoCryptoSymbolTest, TestInitialize) {
		CTiingoCryptoSymbol tiingo;

		EXPECT_STREQ(tiingo.m_strBaseCurrency, _T(""));
		EXPECT_STREQ(tiingo.m_strTicker, _T(""));
		EXPECT_STREQ(tiingo.m_strDescription, _T(""));
		EXPECT_STREQ(tiingo.m_strQuoteCurrency, _T(""));
		EXPECT_STREQ(tiingo.m_strName, _T(""));
	}

	TEST_F(CTiingoCryptoSymbolTest, TestSave) {
		CTiingoCryptoSymbol stock, stock2;
		CSetTiingoCrypto setTiingoCrypto;

		stock.m_strBaseCurrency = _T("USD");
		stock.m_strDescription = _T("abcdefg");
		stock.m_strName = _T("NewName");
		stock.m_strQuoteCurrency = _T("USD2");
		stock.m_strTicker = _T("ABC");

		setTiingoCrypto.Open();
		stock.Append(setTiingoCrypto);
		setTiingoCrypto.Close();

		setTiingoCrypto.m_strFilter = _T("[Ticker] = 'ABC'");
		setTiingoCrypto.Open();
		stock2.Load(setTiingoCrypto);
		setTiingoCrypto.Delete();
		setTiingoCrypto.Close();

		EXPECT_STREQ(stock.m_strBaseCurrency, stock2.m_strBaseCurrency);
		EXPECT_STREQ(stock.m_strTicker, stock2.m_strTicker);
		EXPECT_STREQ(stock.m_strDescription, stock2.m_strDescription);
		EXPECT_TRUE(stock.m_strName == stock2.m_strName);
		EXPECT_TRUE(stock.m_strQuoteCurrency == stock2.m_strQuoteCurrency);
	}
}
