#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"TiingoDataSource.h"

#include"ProductTiingoCryptoSymbol.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoCryptoSymbolsTest : public Test {
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
		CProductTiingoCryptoSymbol stockSymbols;
	};

	TEST_F(CProductTiingoCryptoSymbolsTest, TestInitialize) {
		EXPECT_EQ(stockSymbols.GetIndex(), -1);
		EXPECT_STREQ(stockSymbols.GetInquiryFunction(), _T("https://api.tiingo.com/tiingo/crypto?"));
	}

	TEST_F(CProductTiingoCryptoSymbolsTest, TestCreatMessage) {
		EXPECT_STREQ(stockSymbols.CreateMessage(), stockSymbols.GetInquiryFunction());
	}

	TEST_F(CProductTiingoCryptoSymbolsTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateCryptoSymbol());

		stockSymbols.UpdateDataSourceStatus(gl_pTiingoDataSource);

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateCryptoSymbol());

		gl_pTiingoDataSource->SetUpdateCryptoSymbol(true);
	}

	// 格式不对，缺乏'{'
	Test_TiingoWebData tiingoWebData11(11, _T(""), _T("[\"ticker\":\"curebtc\",\"baseCurrency\":\"cure\",\"name\":\"CureCoin(CURE/BTC)\",\"quoteCurrency\":\"btc\"}]"));
	// 第一个数据缺项
	Test_TiingoWebData tiingoWebData12(12, _T(""), _T("[{\"Missing\":\"curebtc\",\"baseCurrency\":\"cure\",\"name\":\"CureCoin(CURE/BTC)\",\"quoteCurrency\":\"btc\"}]"));
	// 正确的数据
	Test_TiingoWebData tiingoWebData20(20, _T(""), _T("[{\"ticker\":\"New Symbol\",\"baseCurrency\":\"cure\",\"name\":\"CureCoin(CURE/BTC)\",\"quoteCurrency\":\"btc\"}]"));

	class ParseTiingoCryptoTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CTiingoCryptoVectorPtr m_pvCrypto;
		CProductTiingoCryptoSymbol m_tiingoCryptoSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoCrypto1,
	                         ParseTiingoCryptoTest,
	                         testing::Values(&tiingoWebData11, &tiingoWebData12, &tiingoWebData20));

	TEST_P(ParseTiingoCryptoTest, TestParseCryptoSymbol) {
		m_pvCrypto = m_tiingoCryptoSymbolProduct.ParseTiingoCryptoSymbol(m_pWebData);
		switch (m_lIndex) {
		case 11: // 格式不对
			EXPECT_EQ(m_pvCrypto->size(), 0);
			break;
		case 12: // 格式不对
			EXPECT_EQ(m_pvCrypto->size(), 0);
			break;
		case 20:
			EXPECT_EQ(m_pvCrypto->size(), 1);
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strBaseCurrency, _T("cure"));
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strTicker, _T("New Symbol"));
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strName, _T("CureCoin(CURE/BTC)"));
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strDescription, _T("")) << "此项已废弃。为了兼容才没有删除";
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strQuoteCurrency, _T("btc"));
			break;
		default:
			break;
		}
	}

	class ParseTiingoCryptoTest2 : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CTiingoCryptoVectorPtr m_pvCrypto;
		CProductTiingoCryptoSymbol m_tiingoCryptoSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoCrypto1,
	                         ParseTiingoCryptoTest2,
	                         testing::Values(&tiingoWebData11, &tiingoWebData12, &tiingoWebData20));

	TEST_P(ParseTiingoCryptoTest2, TestParseCryptoSymbol) {
		m_pvCrypto = m_tiingoCryptoSymbolProduct.ParseTiingoCryptoSymbol(m_pWebData);
		switch (m_lIndex) {
		case 11: // 格式不对
			EXPECT_EQ(m_pvCrypto->size(), 0);
			break;
		case 12: // 格式不对
			EXPECT_EQ(m_pvCrypto->size(), 0);
			break;
		case 20:
			EXPECT_EQ(m_pvCrypto->size(), 1);
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strBaseCurrency, _T("cure"));
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strTicker, _T("New Symbol"));
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strName, _T("CureCoin(CURE/BTC)"));
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strDescription, _T("")) << "此项已废弃。为了兼容才没有删除";
			EXPECT_STREQ(m_pvCrypto->at(0)->m_strQuoteCurrency, _T("btc"));
			break;
		default:
			break;
		}
	}

	class ProcessTiingoCryptoTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;

			m_tiingoCryptoSymbolProduct.SetMarket(gl_pWorldMarket);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWebDataPtr m_pWebData;
		CProductTiingoCryptoSymbol m_tiingoCryptoSymbolProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingoCrypto1,
	                         ProcessTiingoCryptoTest,
	                         testing::Values(&tiingoWebData11, &tiingoWebData12, &tiingoWebData20));

	TEST_P(ProcessTiingoCryptoTest, TestProcessCryptoSymbol) {
		CTiingoCryptoSymbolPtr pCrypto;
		const auto l = gl_dataContainerTiingoCryptoSymbol.Size();
		m_tiingoCryptoSymbolProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 11: // 格式不对
			break;
		case 12: // 格式不对
			break;
		case 20:
			EXPECT_EQ(gl_dataContainerTiingoCryptoSymbol.Size(), l + 1);
			EXPECT_TRUE(gl_dataContainerTiingoCryptoSymbol.IsSymbol(_T("New Symbol")));
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1) << gl_systemMessage.PopInnerSystemInformationMessage();

			gl_systemMessage.PopInnerSystemInformationMessage();
			pCrypto = gl_dataContainerTiingoCryptoSymbol.GetSymbol(_T("New Symbol"));
			gl_dataContainerTiingoCryptoSymbol.Delete(pCrypto);
			break;
		default:
			break;
		}
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateCryptoSymbol()) << "此标识由UpdateStatus函数更新";

		gl_pTiingoDataSource->SetUpdateCryptoSymbol(true);
	}
}
