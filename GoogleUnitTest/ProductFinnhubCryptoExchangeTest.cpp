#include"pch.h"

#include <gtest/gtest.h>

#include"GeneralCheck.h"

#include"FinnhubDataSource.h"

#include"ProductFinnhubCryptoExchange.h"
#include "SystemMessage.h"

#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductFinnhubCryptoExchangeTest : public Test {
	protected:
		static void SetUpTestSuite() {
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

	protected:
		CProductFinnhubCryptoExchange cryptoExchange;
	};

	TEST_F(CProductFinnhubCryptoExchangeTest, TestInitialize) {
		EXPECT_EQ(cryptoExchange.GetIndex(), 0);
		EXPECT_EQ(cryptoExchange.GetInquiryFunction(), "https://finnhub.io/api/v1/crypto/exchange?");
	}

	TEST_F(CProductFinnhubCryptoExchangeTest, TestCreatMessage) {
		EXPECT_EQ(cryptoExchange.CreateMessage()->front(), "https://finnhub.io/api/v1/crypto/exchange?");
	}

	TEST_F(CProductFinnhubCryptoExchangeTest, TestProcessWebData) {
		// not implemented
	}

	TEST_F(CProductFinnhubCryptoExchangeTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());

		cryptoExchange.UpdateSystemStatus();

		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		EXPECT_EQ(gl_systemMessage.PopInformationMessage(), "Finnhub crypto exchange updated");

		gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);
	}

	namespace {
		// 格式不对(缺开始的‘[’），无法顺利Parser
		Test_FinnhubWebData finnhubWebData202(2, "", R"("oanda","fxcm","forex.com","pepperstone","fxpro","icmtrader","ic markets","octafx","fxpig"])");
		// 格式不对
		Test_FinnhubWebData finnhubWebData203(3, "", R"(["oanda",fxcm,"forex.com","pepperstone","fxpro","icmtrader","ic markets","octafx","fxpig"])");
		// 正确的数据,有新的交易所"new exchange"
		Test_FinnhubWebData finnhubWebData210(10, "", R"(["new exchange","KRAKEN","GEMINI","COINBASE","BINANCE","HUOBI"])");
	}

	class ParseFinnhubCryptoExchangeTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_index = pData->m_index;
			m_text = pData->m_data;
			m_finnhubCryptoExchange.Test_checkAccessRight_(m_text);

			m_pvExchange = nullptr;
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_index;
		string m_text;
		shared_ptr<vector<string>> m_pvExchange;
		CProductFinnhubCryptoExchange m_finnhubCryptoExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoExchange1, ParseFinnhubCryptoExchangeTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData202, &finnhubWebData203, &finnhubWebData210));

	TEST_P(ParseFinnhubCryptoExchangeTest, TestParseFinnhubCryptoExchange0) {
		m_pvExchange = m_finnhubCryptoExchange.Parse(m_text);
		switch (m_index) {
		case 0: // 空数据
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvExchange->size(), 1);
			break;
		case 2: // 格式不对
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 3: // 缺乏字符串
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 10:
			EXPECT_EQ(m_pvExchange->at(0), "new exchange");
			EXPECT_EQ(m_pvExchange->at(1), "KRAKEN");
			EXPECT_EQ(m_pvExchange->size(), 6);
			break;
		default:
			break;
		}
	}
}
