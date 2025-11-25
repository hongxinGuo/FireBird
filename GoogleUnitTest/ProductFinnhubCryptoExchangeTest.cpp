#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"FinnhubDataSource.h"

#include"ProductFinnhubCryptoExchange.h"

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

		cryptoExchange.UpdateSystemStatus(gl_pFinnhubDataSource);

		EXPECT_FALSE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);
		EXPECT_EQ(gl_systemMessage.PopInformationMessage(), "Finnhub crypto exchange updated");

		gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);
	}

	// 格式不对(缺开始的‘[’），无法顺利Parser
	Test_FinnhubWebData finnhubWebData202(2, "", "\"oanda\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]");
	// 格式不对
	Test_FinnhubWebData finnhubWebData203(3, "", "[\"oanda\",fxcm,\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]");
	// 正确的数据
	Test_FinnhubWebData finnhubWebData210(10, "", "[\"new exchange\",\"FXPIG\",\"KRAKEN\",\"ZB\",\"HITBTC\",\"GEMINI\",\"COINBASE\",\"BITTREX\",\"HUOBI\"]");

	class ParseFinnhubCryptoExchangeTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubCryptoExchange.__Test_checkAccessRight(m_pWebData);

			m_pvExchange = nullptr;
		}

		void TearDown() override {
			// clearUp
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		shared_ptr<vector<string>> m_pvExchange;
		CProductFinnhubCryptoExchange m_finnhubCryptoExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoExchange1, ParseFinnhubCryptoExchangeTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData202, &finnhubWebData203, &finnhubWebData210));

	TEST_P(ParseFinnhubCryptoExchangeTest, TestParseFinnhubCryptoExchange0) {
		m_pvExchange = m_finnhubCryptoExchange.ParseFinnhubCryptoExchange(m_pWebData);
		switch (m_lIndex) {
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
			EXPECT_EQ(m_pvExchange->at(1), "FXPIG");
			EXPECT_EQ(m_pvExchange->size(), 9);
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
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubCryptoExchange.__Test_checkAccessRight(m_pWebData);

			m_finnhubCryptoExchange.SetMarket(gl_pWorldMarket);
			EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15) << "最初装载了15个";
		}

		void TearDown() override {
			// clearUp
			gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);

			SCOPED_TRACE("");
			GeneralCheck();
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15) << "最初装载了15个";
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubCryptoExchange m_finnhubCryptoExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubCryptoExchange1, ProcessFinnhubCryptoExchangeTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1,& finnhubWebData202, &finnhubWebData203, &finnhubWebData210));

	TEST_P(ProcessFinnhubCryptoExchangeTest, TestProcessFinnhubCryptoExchange0) {
		m_finnhubCryptoExchange.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // 空数据
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15);
			break;
		case 2: // 格式不对
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15);
			break;
		case 3: // 缺乏字符串
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 15);
			break;
		case 10:
			EXPECT_EQ(gl_dataContainerFinnhubCryptoExchange.Size(), 16) << "加入了new exchange这个新的交易所";
			EXPECT_TRUE(gl_dataContainerFinnhubCryptoExchange.Delete("new exchange")); // 清除new exchange这个新加入的
			break;
		default:
			break;
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
	}
}
