#include"pch.h"

#include"GeneralCheck.h"
#include"FinnhubStock.h"
#include"WorldMarket.h"
#include"TiingoDataSource.h"
#include"ProductTiingoMarketNews.h"
#include"TestWebData.h"

using namespace testing;

namespace FireBirdTest {
	class CProductTiingoMarketNewsTest : public Test {
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
		CProductTiingoMarketNews marketNews;
	};

	TEST_F(CProductTiingoMarketNewsTest, TestInitialize) {
		EXPECT_EQ(marketNews.GetIndex(), 0);
		EXPECT_EQ(marketNews.GetInquiryFunction(), "https://api.tiingo.com/tiingo/news?");
	}

	TEST_F(CProductTiingoMarketNewsTest, TestCreatMessage) {
		EXPECT_EQ(marketNews.CreateMessage(), marketNews.GetInquiryFunction());
	}

	TEST_F(CProductTiingoMarketNewsTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	TEST_F(CProductTiingoMarketNewsTest, TestUpdateDataSourceStatus1) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateMarketNews());

		marketNews.UpdateSystemStatus(gl_pTiingoDataSource);

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateMarketNews());

		gl_pTiingoDataSource->SetUpdateMarketNews(true);

		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_EQ(gl_systemMessage.InformationSize(), 1);

		// 恢复原状
		gl_systemMessage.PopInnerSystemInformationMessage();
		gl_systemMessage.PopInformationMessage();
		gl_systemConfiguration.SetWorldMarketTiingoInquiryTime(500);
		gl_systemConfiguration.SetUpdateDB(false);
	}

	TEST_F(CProductTiingoMarketNewsTest, TestUpdateDataSourceStatus2) {
		EXPECT_TRUE(gl_pTiingoDataSource->IsUpdateMarketNews());

		marketNews.UpdateSystemStatus(gl_pTiingoDataSource);

		EXPECT_FALSE(gl_pTiingoDataSource->IsUpdateMarketNews());

		gl_pTiingoDataSource->SetUpdateMarketNews(true);
		EXPECT_EQ(gl_systemConfiguration.GetWorldMarketTiingoInquiryTime().count(), 500);

		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
		EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);

		// 恢复原状
		gl_systemMessage.PopInnerSystemInformationMessage();
		gl_systemMessage.PopInformationMessage();
		gl_systemConfiguration.SetWorldMarketTiingoInquiryTime(500);
		gl_systemConfiguration.SetUpdateDB(false);
	}

	// 正确的数据
	Test_TiingoWebData tiingoMarketNews1(1, "", "[{\"source\":\"cnbc.com\",\"crawlDate\":\"2019-01-29T22:20:01.696871Z\",\"description\":\"Apple CEO Tim Cook told CNBC that trade tensions between the U.S. and China have improved since late December.\",\"url\":\"https://www.cnbc.com/2019/01/29/apples-ceo-sees-optimism-as-trade-tension-between-us-and-china-lessens.html\",\"publishedDate\":\"2019-01-29T22:17:00Z\",\"tags\":[\"China\",\"Economic Measures\",\"Economics\",\"Markets\",\"Stock\",\"Technology\",\"Tiingo Top\",\"Trade\"],\"tickers\":[\"aapl\"],\"id\":15063835,\"title\":\"Apple CEO Tim Cook on US-China trade negotiations: 'There is a bit more optimism in the air'\"}]");
	// 第一个数据缺项
	Test_TiingoWebData tiingoMarketNews2(2, "", "[{\"source\":\"cnbc.com\",\"crawlDate\":\"2019-01-29T22:20:01.696871Z\",\"description\":\"Apple CEO Tim Cook told CNBC that trade tensions between the U.S. and China have improved since late December.\",\"url\":\"https://www.cnbc.com/2019/01/29/apples-ceo-sees-optimism-as-trade-tension-between-us-and-china-lessens.html\",\"publishedDate\":\"2019-01-29T22:17:00Z\",\"tags\":[\"China\",\"Economic Measures\",\"Economics\"],\"tickers\":[\"aapl\"],\"id\":15063835,\"title\":\"Apple CEO Tim Cook on US-China trade negotiations: 'There is a bit more optimism in the air'\"}]");
	// 第二个数据缺项
	Test_TiingoWebData tiingoMarketNews3(3, "aapl", "[{\"source\":\"cnbc.com\",\"crawlDate\":\"2019-01-29T22:20:01.696871Z\",\"description\":\"Apple CEO Tim Cook told CNBC that trade tensions between the U.S. and China have improved since late December.\",\"url\":\"https://www.cnbc.com/2019/01/29/apples-ceo-sees-optimism-as-trade-tension-between-us-and-china-lessens.html\",\"publishedDate\":\"2019-01-29T22:17:00Z\",\"tags\":[\"China\",\"Economic Measures\",\"Economics\"],\"tickers\":[\"aapl\"],\"id\":15063835,\"title\":\"Apple CEO Tim Cook on US-China trade negotiations: 'There is a bit more optimism in the air'\"}]");
	// 正确的数据
	Test_TiingoWebData tiingoMarketNews4(4, "", "[{\"source\":\"cnbc.com\",\"crawlDate\":\"2019-01-29T22:20:01.696871Z\",\"description\":\"Apple CEO Tim Cook told CNBC that trade tensions between the U.S. and China have improved since late December.\",\"url\":\"https://www.cnbc.com/2019/01/29/apples-ceo-sees-optimism-as-trade-tension-between-us-and-china-lessens.html\",\"publishedDate\":\"2019-01-29T22:17:00Z\",\"tags\":[\"China\",\"Economic Measures\",\"Economics\",\"Markets\",\"Stock\",\"Technology\",\"Tiingo Top\",\"Trade\"],\"tickers\":[\"aapl\"],\"id\":15063835,\"title\":\"Apple CEO Tim Cook on US-China trade negotiations: 'There is a bit more optimism in the air'\"}]");
	// 正确的数据
	Test_TiingoWebData tiingoMarketNews10(10, "", "[{\"source\":\"cnbc.com\",\"crawlDate\":\"2019-01-29T22:20:01.696871Z\",\"description\":\"Apple CEO Tim Cook told CNBC that trade tensions between the U.S. and China have improved since late December.\",\"url\":\"https://www.cnbc.com/2019/01/29/apples-ceo-sees-optimism-as-trade-tension-between-us-and-china-lessens.html\",\"publishedDate\":\"2019-01-29T22:17:00Z\",\"tags\":[\"China\",\"Economic Measures\",\"Economics\",\"Markets\",\"Stock\",\"Technology\",\"Tiingo Top\",\"Trade\"],\"tickers\":[\"aapl\"],\"id\":15063835,\"title\":\"Apple CEO Tim Cook on US-China trade negotiations: 'There is a bit more optimism in the air'\"}]");

	class ParseTiingoMarketNewsTest : public TestWithParam<Test_TiingoWebData*> {
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
		CVectorTiingoMarketNewsPtr m_pvMarketNews;
		CProductTiingoMarketNews m_tiingoMarketNewsProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseTiingoMarketNews,
	                         ParseTiingoMarketNewsTest,
	                         testing::Values(&tiingoMarketNews1, &tiingoMarketNews2,
		                         &tiingoMarketNews3, &tiingoMarketNews4, &tiingoMarketNews10));

	TEST_P(ParseTiingoMarketNewsTest, TestParseTiingoMarketNews) {
		m_pvMarketNews = m_tiingoMarketNewsProduct.ParseTiingoMarketNews(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_EQ(m_pvMarketNews->size(), 1);
			break;
		case 2: // 格式不对
			EXPECT_EQ(m_pvMarketNews->size(), 1);
			break;
		case 3: // 缺乏address项
			EXPECT_EQ(m_pvMarketNews->size(), 1);
			break;
		case 4:
			EXPECT_EQ(m_pvMarketNews->size(), 1);
			break;
		case 10:
			EXPECT_EQ(m_pvMarketNews->size(), 1);
			break;
		default:
			break;
		}
	}
	/*
	class ProcessTiingoMarketNewsTest : public TestWithParam<Test_TiingoWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_TiingoWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pWebData = pData->m_pData;

			m_tiingoMarketNewsProduct.SetMarket(gl_pWorldMarket);
			m_tiingoMarketNewsProduct.SetIndex(0);
			gl_pTiingoDataSource->SetUpdateMarketNews(true);
			EXPECT_FALSE(gl_dataContainerFinnhubmarketNews.IsUpdateProfileDB());
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
		CProductTiingoMarketNews m_tiingoMarketNewsProduct;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessTiingomarketNews,
	                         ProcessTiingoMarketNewsTest,
	                         testing::Values(&tiingoMarketNews1, &tiingoMarketNews2,
		                         &tiingoMarketNews3, &tiingoMarketNews4, &tiingoMarketNews10));

	TEST_P(ProcessTiingoMarketNewsTest, TestProcessmarketNews) {
		CTiingoMarketNewsPtr pTiingoMarketNews = nullptr;
		CFinnhubStockPtr pStock = nullptr;
		EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
		m_tiingoMarketNewsProduct.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 1: // 格式不对
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 2: // 格式不对
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 0) << gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 3: // 第二个数据缺乏address项,返回一个成功
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1) << "第一个数据是正确的";
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateProfileDB()) << "第一个数据是正确的";

		//恢复原状
			pStock = gl_dataContainerFinnhubStock.GetItem("A");
			EXPECT_TRUE(pStock->IsUpdateCompanyProfile());
			pStock->SetUpdateProfileDB(false);
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 4:
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_TRUE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());

		//恢复原状
			pStock = gl_dataContainerFinnhubStock.GetItem("AA");
			EXPECT_TRUE(pStock->IsUpdateCompanyProfile());
			pStock->SetUpdateProfileDB(false);
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		case 10:
			EXPECT_EQ(gl_systemMessage.InnerSystemInfoSize(), 1);
			gl_systemMessage.PopInnerSystemInformationMessage();
			EXPECT_TRUE(gl_dataContainerTiingoStock.IsStock("NEW SYMBOL")));
			EXPECT_TRUE((pTiingoStock = gl_dataContainerTiingoStock.GetStock("NEW SYMBOL"))) != nullptr);
			pStock = gl_dataContainerFinnhubStock.GetItem("AA");
			EXPECT_STREQ(pStock->GetName(), "New Name")) << "更改为此新名字";
			EXPECT_TRUE(pStock->IsUpdateProfileDB());

		// 恢复原状
			gl_dataContainerTiingoStock.Delete(pTiingoStock);
			pStock->SetUpdateProfileDB(false);
			pStock->SetName("Alcoa Corp");
			EXPECT_FALSE(gl_dataContainerFinnhubStock.IsUpdateProfileDB());
			break;
		default:
			break;
		}
	}
	*/
}
