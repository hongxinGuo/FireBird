#include"pch.h"

#include"globedef.h"
#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubCryptoExchange.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubCryptoExchangeTest : public ::testing::Test {
	protected:
		static void SetUpTestSuite(void) {
			GeneralCheck();
		}

		static void TearDownTestSuite(void) {
			GeneralCheck();
		}

		virtual void SetUp(void) override {
		}

		virtual void TearDown(void) override {
			// clearu
			GeneralCheck();
		}

	protected:
		CProductFinnhubCryptoExchange countryList;
	};

	TEST_F(CFinnhubCryptoExchangeTest, TestInitialize) {
		EXPECT_EQ(countryList.GetIndex(), -1);
		EXPECT_STREQ(countryList.GetInquiringStr(), _T("https://finnhub.io/api/v1/crypto/exchange?"));
	}

	TEST_F(CFinnhubCryptoExchangeTest, TestCreatMessage) {
		EXPECT_STREQ(countryList.CreatMessage(), _T("https://finnhub.io/api/v1/crypto/exchange?"));
	}

	TEST_F(CFinnhubCryptoExchangeTest, TestProcessWebData) {
		// not implmented yet
	}

	// 格式不对(缺开始的‘[’），无法顺利Parser
	FinnhubWebData finnhubWebData202(2, _T(""), _T("\"oanda\",\"fxcm\",\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
	// 格式不对
	FinnhubWebData finnhubWebData203(3, _T(""), _T("[\"oanda\",fxcm,\"forex.com\",\"pepperstone\",\"fxpro\",\"icmtrader\",\"ic markets\",\"octafx\",\"fxpig\"]"));
	// 正确的数据
	FinnhubWebData finnhubWebData210(10, _T(""), _T("[\"new exchange\",\"FXPIG\",\"KRAKEN\",\"ZB\",\"HITBTC\",\"GEMINI\",\"COINBASE\",\"BITTREX\",\"HUOBI\"]"));

	class ParseFinnhubCryptoExchangeTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pvExchange = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
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

	class ProcessFinnhubCryptoExchangeTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_finnhubCryptoExchange.SetMarket(gl_pWorldMarket.get());
			EXPECT_FALSE(gl_pWorldMarket->IsFinnhubCryptoExchangeUpdated());
			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 14) << "最初装载了14个";
		}
		virtual void TearDown(void) override {
			// clearup
			gl_pWorldMarket->SetFinnhubCryptoExchangeUpdated(false);

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
		m_finnhubCryptoExchange.ProcessWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoExchangeUpdated());
			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 14);
			break;
		case 3: // 缺乏字符串
			EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoExchangeUpdated());
			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 14);
			break;
		case 10:
			EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoExchangeUpdated());
			EXPECT_EQ(gl_pWorldMarket->GetCryptoExchangeSize(), 15) << "加入了new exchange这个新的交易所";

			EXPECT_TRUE(gl_pWorldMarket->DeleteCryptoExchange(_T("new exchange"))); // 清除new exchange这个新加入的
			break;
		default:
			break;
		}
	}
}