#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"FinnhubDataSource.h"

#include"ProductFinnhubCryptoExchange.h"

using namespace testing;

namespace FireBirdTest {
	class CProductFinnhubCryptoExchangeTest : public Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		void SetUp(void) override {
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	protected:
		CProductFinnhubCryptoExchange countryList;
	};

	TEST_F(CProductFinnhubCryptoExchangeTest, TestInitialize) {
		EXPECT_EQ(countryList.GetIndex(), -1);
		EXPECT_STREQ(countryList.GetInquiry(), _T("https://finnhub.io/api/v1/crypto/exchange?"));
	}

	TEST_F(CProductFinnhubCryptoExchangeTest, TestCreatMessage) {
		EXPECT_STREQ(countryList.CreateMessage(), _T("https://finnhub.io/api/v1/crypto/exchange?"));
	}

	TEST_F(CProductFinnhubCryptoExchangeTest, TestProcessWebData) {
		// not implmented yet
	}

	// 格式不对(缺开始的‘[’），无法顺利Parser
	FinnhubWebData finnhubWebData202(2, _T(""), _T("\"oanda\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
	// 格式不对
	FinnhubWebData finnhubWebData203(3, _T(""), _T("[\"oanda\",fxcm,\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
	// 正确的数据
	FinnhubWebData finnhubWebData210(10, _T(""), _T("[\"new exchange\",\"FXPIG\",\"KRAKEN\",\"ZB\",\"HITBTC\",\"GEMINI\",\"COINBASE\",\"BITTREX\",\"HUOBI\"]"));

	class ParseFinnhubCryptoExchangeTest : public TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreateNlohmannJson();
			m_pWebData->SetJSonContentType(true);
			m_pvExchange = nullptr;
		}

		void TearDown(void) override {
			// clearUp
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		shared_ptr<vector<CString>> m_pvExchange;
		CProductFinnhubCryptoExchange m_finnhubCryptoExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoExchange1, ParseFinnhubCryptoExchangeTest,
		testing::Values(&finnhubWebData202, &finnhubWebData203, &finnhubWebData210));

	TEST_P(ParseFinnhubCryptoExchangeTest, TestParseFinnhubCryptoExchange0) {
		m_pvExchange = m_finnhubCryptoExchange.ParseFinnhubCryptoExchange(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 3: // 缺乏字符串
			EXPECT_EQ(m_pvExchange->size(), 0);
			break;
		case 10:
			EXPECT_STREQ(m_pvExchange->at(0), _T("new exchange"));
			EXPECT_STREQ(m_pvExchange->at(1), _T("FXPIG"));
			EXPECT_EQ(m_pvExchange->size(), 9);
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubCryptoExchangeTest : public TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreateNlohmannJson();
			m_pWebData->SetJSonContentType(true);
			m_finnhubCryptoExchange.SetMarket(gl_pWorldMarket.get());
			EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 14) << "最初装载了14个";
		}

		void TearDown(void) override {
			// clearUp
			gl_pFinnhubDataSource->SetUpdateCryptoExchange(true);

			GeneralCheck();
			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 14) << "最初装载了14个";
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubCryptoExchange m_finnhubCryptoExchange;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubCryptoExchange1, ProcessFinnhubCryptoExchangeTest,
		testing::Values(&finnhubWebData202, &finnhubWebData203, &finnhubWebData210));

	TEST_P(ProcessFinnhubCryptoExchangeTest, TestProcessFinnhubCryptoExchange0) {
		m_finnhubCryptoExchange.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 14);
			break;
		case 3: // 缺乏字符串
			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 14);
			break;
		case 10:
			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 15) << "加入了new exchange这个新的交易所";
			EXPECT_TRUE(gl_pWorldMarket->DeleteCryptoExchange(_T("new exchange"))); // 清除new exchange这个新加入的
			break;
		default:
			break;
		}
		EXPECT_TRUE(gl_pFinnhubDataSource->IsUpdateCryptoExchange());
	}
}
