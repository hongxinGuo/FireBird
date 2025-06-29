#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"
#include"TiingoDataSource.h"

#include"ProductTiingoCryptoSymbol.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoCryptoTest : public Test {
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
		CProductTiingoCryptoSymbol stockSymbol;
	};

	TEST_F(CProductTiingoCryptoTest, TestInitialize) {
		EXPECT_EQ(stockSymbol.GetIndex(), 0);
		EXPECT_STREQ(stockSymbol.GetInquiryFunction().c_str(), _T("https://api.tiingo.com/tiingo/crypto?"));
	}

	TEST_F(CProductTiingoCryptoTest, TestCreatMessage) {
		EXPECT_STREQ(stockSymbol.CreateMessage().c_str(), stockSymbol.GetInquiryFunction().c_str());
	}

	TEST_F(CProductTiingoCryptoTest, TestUpdateDataSourceStatus) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateCryptoSymbol());

		stockSymbol.UpdateDataSourceStatus(gl_pTiingoDataSource);

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateCryptoSymbol());
		EXPECT_STREQ(gl_systemMessage.PopInformationMessage().c_str(), _T("Tiingo crypto symbol updated"));

		// 恢复原状
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
		CTiingoCryptosPtr m_pvCrypto;
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
			EXPECT_TRUE(m_pvCrypto->at(0)->m_strBaseCurrency== _T("cure"));
			EXPECT_TRUE(m_pvCrypto->at(0)->GetSymbol()== _T("New Symbol"));
			EXPECT_TRUE(m_pvCrypto->at(0)->m_strName == _T("CureCoin(CURE/BTC)"));
			EXPECT_TRUE(m_pvCrypto->at(0)->GetDescription()== _T("")) << "此项已废弃。为了兼容才没有删除";
			EXPECT_TRUE(m_pvCrypto->at(0)->m_strQuoteCurrency==_T("btc"));
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
		CTiingoCryptosPtr m_pvCrypto;
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
			EXPECT_TRUE(m_pvCrypto->at(0)->m_strBaseCurrency== _T("cure"));
			EXPECT_TRUE(m_pvCrypto->at(0)->GetSymbol()== _T("New Symbol"));
			EXPECT_TRUE(m_pvCrypto->at(0)->m_strName== _T("CureCoin(CURE/BTC)"));
			EXPECT_TRUE(m_pvCrypto->at(0)->GetDescription()== _T("")) << "此项已废弃。为了兼容才没有删除";
			EXPECT_TRUE(m_pvCrypto->at(0)->m_strQuoteCurrency== _T("btc"));
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
			gl_systemConfiguration.SetUpdateDB(false);
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
		CTiingoCryptoPtr pCrypto;
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

			pCrypto = gl_dataContainerTiingoCryptoSymbol.GetCrypto(_T("New Symbol"));
			gl_dataContainerTiingoCryptoSymbol.Delete(pCrypto);
			break;
		default:
			break;
		}
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateCryptoSymbol()) << "此标识由UpdateStatus函数更新";

		gl_pTiingoDataSource->SetUpdateCryptoSymbol(true);
	}
}
