#include"pch.h"

#include"GeneralCheck.h"

#include"TiingoCrypto.h"

namespace FireBirdTest {
	class CTiingoCryptoSymbolTest : public ::testing::Test {
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

	TEST_F(CTiingoCryptoSymbolTest, TestInitialize) {
		const CTiingoCrypto tiingo;

		EXPECT_STREQ(tiingo.m_strBaseCurrency, _T(""));
		EXPECT_STREQ(tiingo.GetSymbol(), _T(""));
		EXPECT_STREQ(tiingo.GetDescription(), _T(""));
		EXPECT_STREQ(tiingo.m_strQuoteCurrency, _T(""));
		EXPECT_STREQ(tiingo.m_strName, _T(""));
	}

	TEST_F(CTiingoCryptoSymbolTest, TestSave) {
		CTiingoCrypto stock, stock2;
		CSetTiingoCrypto setTiingoCrypto;

		stock.m_strBaseCurrency = _T("USD");
		stock.SetDescription(_T("abcdefg"));
		stock.m_strName = _T("NewName");
		stock.m_strQuoteCurrency = _T("USD2");
		stock.SetSymbol(_T("ABC"));

		setTiingoCrypto.Open();
		stock.Append(setTiingoCrypto);
		setTiingoCrypto.Close();

		setTiingoCrypto.m_strFilter = _T("[Ticker] = 'ABC'");
		setTiingoCrypto.Open();
		stock2.Load(setTiingoCrypto);
		setTiingoCrypto.Delete();
		setTiingoCrypto.Close();

		EXPECT_STREQ(stock.m_strBaseCurrency, stock2.m_strBaseCurrency);
		EXPECT_STREQ(stock.GetSymbol(), stock2.GetSymbol());
		EXPECT_STREQ(stock.GetDescription(), stock2.GetDescription());
		EXPECT_TRUE(stock.m_strName == stock2.m_strName);
		EXPECT_TRUE(stock.m_strQuoteCurrency == stock2.m_strQuoteCurrency);
	}
}
