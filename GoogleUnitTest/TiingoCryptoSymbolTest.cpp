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

		EXPECT_EQ(tiingo.m_strBaseCurrency, "");
		EXPECT_EQ(tiingo.GetSymbol(), "");
		EXPECT_EQ(tiingo.GetDescription(), "");
		EXPECT_EQ(tiingo.m_strQuoteCurrency, "");
		EXPECT_EQ(tiingo.m_strName, "");

		EXPECT_EQ(tiingo.GetRatio(), 3);
	}

	TEST_F(CTiingoCryptoSymbolTest, TestSave) {
		CTiingoCrypto stock, stock2;
		CSetTiingoCrypto setTiingoCrypto;

		stock.m_strBaseCurrency = "USD";
		stock.SetDescription("abcdefg");
		stock.m_strName = "NewName";
		stock.m_strQuoteCurrency = "USD2";
		stock.SetSymbol("ABC");

		setTiingoCrypto.Open();
		stock.Append(setTiingoCrypto);
		setTiingoCrypto.Close();

		setTiingoCrypto.m_strFilter = "[Ticker] = 'ABC'";
		setTiingoCrypto.Open();
		stock2.Load(setTiingoCrypto);
		setTiingoCrypto.Delete();
		setTiingoCrypto.Close();

		EXPECT_EQ(stock.m_strBaseCurrency, stock2.m_strBaseCurrency);
		EXPECT_EQ(stock.GetSymbol(), stock2.GetSymbol());
		EXPECT_EQ(stock.GetDescription(), stock2.GetDescription());
		EXPECT_TRUE(stock.m_strName == stock2.m_strName);
		EXPECT_TRUE(stock.m_strQuoteCurrency == stock2.m_strQuoteCurrency);
	}
}
