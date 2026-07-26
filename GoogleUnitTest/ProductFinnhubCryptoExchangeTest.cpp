#include"pch.h"

#include <gtest/gtest.h>

#include "ContainerFinnhubCryptoExchange.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"
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
		EXPECT_EQ(cryptoExchange.CreateMessage(), "https://finnhub.io/api/v1/crypto/exchange?");
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
			m_pWebData = pData->m_pData;
			m_finnhubCryptoExchange.Test_checkAccessRight_(m_pWebData);

			m_pvExchange = nullptr;
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_index;
		CWebDataPtr m_pWebData;
		shared_ptr<vector<string>> m_pvExchange;
		CProductFinnhubCryptoExchange m_finnhubCryptoExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoExchange1, ParseFinnhubCryptoExchangeTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData202, &finnhubWebData203, &finnhubWebData210));

	TEST_P(ParseFinnhubCryptoExchangeTest, TestParseFinnhubCryptoExchange0) {
		m_pvExchange = m_finnhubCryptoExchange.ParseFinnhubCryptoExchange(m_pWebData);
		switch (m_index) {
		case 0: // 空数据
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvExchange->size(), 0);
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

	class ProcessFinnhubCryptoExchangeTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_index = pData->m_index;
			m_pWebData = pData->m_pData;
			m_finnhubCryptoExchange.Test_checkAccessRight_(m_pWebData);

			EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 12) << "最初装载了12个";
		}

		void TearDown() override {
			// clearUp
			gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);

			SCOPED_TRACE("");
			GeneralCheck();
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 12) << "最初装载了12个";
		}

	public:
		int m_index;
		CWebDataPtr m_pWebData;
		CProductFinnhubCryptoExchange m_finnhubCryptoExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubCryptoExchange1, ProcessFinnhubCryptoExchangeTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1,& finnhubWebData202, &finnhubWebData203, &finnhubWebData210));

	TEST_P(ProcessFinnhubCryptoExchangeTest, TestProcessFinnhubCryptoExchange0) {
		m_finnhubCryptoExchange.ParseAndStoreWebData(m_pWebData);
		switch (m_index) {
		case 0: // 空数据
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 12);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 12);
			break;
		case 2: // 格式不对
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 12);
			break;
		case 3: // 缺乏字符串
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 12);
			break;
		case 10:
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 13) << "加入了new exchange这个新的交易所";
			EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.Delete("new exchange")); // 清除new exchange这个新加入的
			break;
		default:
			break;
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
	}
}
