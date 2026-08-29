#include"pch.h"

#include <gtest/gtest.h>

#include "ContainerFinnhubCrypto.h"
#include "ContainerFinnhubCryptoExchange.h"
#include"GeneralCheck.h"
#include"ProductFinnhubCryptoSymbol.h"
#include "SystemMessage.h"
#include "WorldMarket.h"
#include "FinnhubCrypto.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubCryptoSymbolProductTest : public ::testing::Test {
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
		CProductFinnhubCryptoSymbol cryptoSymbolProduct;
	};

	TEST_F(CFinnhubCryptoSymbolProductTest, TestInitialize) {
		EXPECT_EQ(cryptoSymbolProduct.GetIndex(), 0);
		EXPECT_EQ(cryptoSymbolProduct.GetInquiryFunction(), "https://finnhub.io/api/v1/crypto/symbol?exchange=");
	}

	TEST_F(CFinnhubCryptoSymbolProductTest, TestCreatMessage) {
		cryptoSymbolProduct.SetIndex(1);
		EXPECT_EQ(cryptoSymbolProduct.CreateMessage()->front(), (cryptoSymbolProduct.GetInquiryFunction() + gl_dataContainerFinnhubCryptoExchange.GetItem(1)));
	}

	TEST_F(CFinnhubCryptoSymbolProductTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	namespace {
		// 格式不对(缺开始的‘[’），无法顺利Parser
		Test_FinnhubWebData finnhubWebData212(2, "", R"(["description":"Oanda Singapore 30","displaySymbol":"SG30/SGD","symbol":"OANDA:SG30_SGD"},{"description":"Oanda Bund","displaySymbol":"DE10YB/EUR","symbol":"OANDA:DE10YB_EUR"}])");
		// 数据缺乏description
		Test_FinnhubWebData finnhubWebData213(3, "", R"([{"a":"Oanda Singapore 30","displaySymbol":"SG30/SGD","symbol":"OANDA:SG30_SGD"},{"description":"Oanda Bund","displaySymbol":"DE10YB/EUR","symbol":"OANDA:DE10YB_EUR"}])");
		// 数据缺乏displaySymbol
		Test_FinnhubWebData finnhubWebData214(4, "", R"([{"description":"Oanda Singapore 30","a":"SG30/SGD","symbol":"OANDA:SG30_SGD"},{"description":"Oanda Bund","displaySymbol":"DE10YB/EUR","symbol":"OANDA:DE10YB_EUR"}])");
		// 数据缺乏symbol
		Test_FinnhubWebData finnhubWebData215(5, "", R"([{"description":"Oanda Singapore 30","displaySymbol":"SG30/SGD","a":"OANDA:SG30_SGD"},{"description":"Oanda Bund","displaySymbol":"DE10YB/EUR","symbol":"OANDA:DE10YB_EUR"}])");
		// 正确的数据
		Test_FinnhubWebData finnhubWebData220(10, "", R"([{"description":"Oanda Singapore 30","displaySymbol":"SG30/SGD","symbol":"New Symbol"},{"description":"Oanda Bund","displaySymbol":"DE10YB/EUR","symbol":"OANDA:DE10YB_EUR"}])");
	}

	class ParseFinnhubCryptoSymbolTest : public::testing::TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_index = pData->m_index;
			m_text = pData->m_data;
			m_finnhubCryptoSymbolProduct.Test_checkAccessRight_(m_text);

			m_pvCryptoSymbol = nullptr;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		int m_index;
		string m_text;
		CFinnhubCryptosPtr m_pvCryptoSymbol;
		CProductFinnhubCryptoSymbol m_finnhubCryptoSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubCryptoSymbol1, ParseFinnhubCryptoSymbolTest,
	                         testing::Values(&finnhubWebData0,&finnhubWebData1, &finnhubWebData212, &finnhubWebData213, &finnhubWebData214,
		                         &finnhubWebData215, &finnhubWebData220));

	TEST_P(ParseFinnhubCryptoSymbolTest, TestParseFinnhubCryptoSymbol0) {
		m_pvCryptoSymbol = m_finnhubCryptoSymbolProduct.Parse(m_text);
		switch (m_index) {
		case 0: // 空数据
			EXPECT_EQ(m_pvCryptoSymbol->size(), 0);
			break;
		case 1: // 无权利访问的数据
			EXPECT_EQ(m_pvCryptoSymbol->size(), 0);
			break;
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
			EXPECT_EQ(m_pvCryptoSymbol->at(0)->GetSymbol(), "New Symbol");
			EXPECT_EQ(m_pvCryptoSymbol->at(1)->GetSymbol(), "OANDA:DE10YB_EUR");
			EXPECT_EQ(m_pvCryptoSymbol->size(), 2);
			break;
		default:
			break;
		}
	}

	
}
