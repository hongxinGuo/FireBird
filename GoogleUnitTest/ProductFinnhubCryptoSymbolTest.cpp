#include"pch.h"

#include"GeneralCheck.h"

#include"ProductFinnhubCryptoSymbol.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace FireBirdTest {
	class CFinnhubCryptoSymbolProductTest : public ::testing::Test {
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
		CProductFinnhubCryptoSymbol cryptoSymbolProduct;
	};

	TEST_F(CFinnhubCryptoSymbolProductTest, TestInitialize) {
		EXPECT_EQ(cryptoSymbolProduct.GetIndex(), -1);
		EXPECT_STREQ(cryptoSymbolProduct.GetInquiry(), _T("https://finnhub.io/api/v1/crypto/symbol?exchange="));
	}

	TEST_F(CFinnhubCryptoSymbolProductTest, TestCreatMessage) {
		cryptoSymbolProduct.SetMarket(gl_pWorldMarket.get());
		cryptoSymbolProduct.SetIndex(1);
		EXPECT_STREQ(cryptoSymbolProduct.CreateMessage(), cryptoSymbolProduct.GetInquiry() + gl_pWorldMarket->GetCryptoExchange(1));
	}

	TEST_F(CFinnhubCryptoSymbolProductTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对(缺开始的‘[’），无法顺利Parser
	FinnhubWebData finnhubWebData212(2, _T(""), _T("[\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 数据缺乏description
	FinnhubWebData finnhubWebData213(3, _T(""), _T("[{\"a\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 数据缺乏displaySymbol
	FinnhubWebData finnhubWebData214(4, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"a\":\"SG30/SGD\",\"symbol\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 数据缺乏symbol
	FinnhubWebData finnhubWebData215(5, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"a\":\"OANDA:SG30_SGD\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));
	// 正确的数据
	FinnhubWebData finnhubWebData220(10, _T(""), _T("[{\"description\":\"Oanda Singapore 30\",\"displaySymbol\":\"SG30/SGD\",\"symbol\":\"New Symbol\"},{\"description\":\"Oanda Bund\",\"displaySymbol\":\"DE10YB/EUR\",\"symbol\":\"OANDA:DE10YB_EUR\"}]"));

	class ParseFinnhubCryptoSymbolTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreateNlohmannJson();
			m_pWebData->SetJSonContentType(true);
			m_pvCryptoSymbol = nullptr;
		}

		void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CFinnhubCryptoSymbolVectorPtr m_pvCryptoSymbol;
		CProductFinnhubCryptoSymbol m_finnhubCryptoSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoSymbol1, ParseFinnhubCryptoSymbolTest,
	                         testing::Values(&finnhubWebData212, &finnhubWebData213, &finnhubWebData214,
		                         &finnhubWebData215, &finnhubWebData220));

	TEST_P(ParseFinnhubCryptoSymbolTest, TestParseFinnhubCryptoSymbol0) {
		m_pvCryptoSymbol = m_finnhubCryptoSymbolProduct.ParseFinnhubCryptoSymbol(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_EQ(m_pvCryptoSymbol->size(), 0);
			break;
		case 3: // 缺乏字符串
			EXPECT_EQ(m_pvCryptoSymbol->size(), 0);
			break;
		case 4: // 缺乏字符串
			EXPECT_EQ(m_pvCryptoSymbol->size(), 0);
			break;
		case 5: // 缺乏字符串
			EXPECT_EQ(m_pvCryptoSymbol->size(), 0);
			break;
		case 10:
			EXPECT_STREQ(m_pvCryptoSymbol->at(0)->GetSymbol(), _T("New Symbol"));
			EXPECT_STREQ(m_pvCryptoSymbol->at(1)->GetSymbol(), _T("OANDA:DE10YB_EUR"));
			EXPECT_EQ(m_pvCryptoSymbol->size(), 2);
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubCryptoSymbolTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
			m_pWebData->CreateNlohmannJson();
			m_pWebData->SetJSonContentType(true);
			m_finnhubCryptoSymbolProduct.SetMarket(gl_pWorldMarket.get());
			m_finnhubCryptoSymbolProduct.SetIndex(0);
		}

		void TearDown(void) override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductFinnhubCryptoSymbol m_finnhubCryptoSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubCryptoSymbol, ProcessFinnhubCryptoSymbolTest,
	                         testing::Values(&finnhubWebData212, &finnhubWebData213, &finnhubWebData214,
		                         &finnhubWebData215, &finnhubWebData220));

	TEST_P(ProcessFinnhubCryptoSymbolTest, TestProcessFinnhubCryptoSymbol) {
		CFinnhubCryptoSymbolPtr pCrypto;
		bool fSucceed = m_finnhubCryptoSymbolProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_TRUE(fSucceed);
			break;
		case 3: // 缺乏字符串
			EXPECT_TRUE(fSucceed);
			break;
		case 4: // 缺乏字符串
			EXPECT_TRUE(fSucceed);
			break;
		case 5: // 缺乏字符串
			EXPECT_TRUE(fSucceed);
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(gl_pWorldMarket->IsFinnhubCryptoSymbol(_T("New Symbol")));
			pCrypto = gl_pWorldMarket->GetFinnhubCryptoSymbol(_T("New Symbol"));
			EXPECT_STREQ(pCrypto->GetDescription(), _T("Oanda Singapore 30"));

		// 恢复原状
			EXPECT_TRUE(gl_pWorldMarket->DeleteFinnhubCryptoSymbol(pCrypto));
			break;
		default:
			break;
		}
	}
}
