#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubStockEstimatesEPSSurprise.h"

using namespace testing;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace StockAnalysisTest {
	class CFinnhubStockEstimatesEPSSurpriseTest : public ::testing::Test {
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
		CProductFinnhubStockEstimatesEPSSurprise stockEstimatesEPSSurprise;
	};

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestInitialize) {
		EXPECT_EQ(stockEstimatesEPSSurprise.GetIndex(), -1);
		EXPECT_STREQ(stockEstimatesEPSSurprise.GetInquiringStr(), _T("https://finnhub.io/api/v1/stock/earnings?symbol="));
	}

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestCreatMessage) {
		stockEstimatesEPSSurprise.SetMarket(gl_pWorldMarket.get());
		stockEstimatesEPSSurprise.SetIndex(1);
		EXPECT_STREQ(stockEstimatesEPSSurprise.CreatMessage(), stockEstimatesEPSSurprise.GetInquiringStr() + gl_pWorldMarket->GetStock(1)->GetSymbol());
	}

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestProcessWebData) {
		// 由MockWorldMarketTest负责测试
	}

	// 格式不对(缺开始的‘[’），无法顺利Parser
	FinnhubWebData finnhubWebData122(2, _T("AAPL"), _T("{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// 第一个数据缺乏actual
	FinnhubWebData finnhubWebData123(3, _T("AAPL"), _T("[{\"Missing\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// 第二个数据缺乏actual
	FinnhubWebData finnhubWebData124(4, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"Missing\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// 第三个数据缺乏actual
	FinnhubWebData finnhubWebData125(5, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"Missing\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// 正确的数据
	FinnhubWebData finnhubWebData130(10, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));

	class ParseFinnhubEPSSurpriseTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
			m_pvEPSSurprise = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetProfileUpdated(false);
			m_pStock->SetUpdateProfileDB(false);
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CEPSSurpriseVectorPtr m_pvEPSSurprise;
		CProductFinnhubStockEstimatesEPSSurprise m_finnhubStockEstimatesEPSSurprise;
	};

	INSTANTIATE_TEST_SUITE_P(TestParseFinnhubEPSSurprise1, ParseFinnhubEPSSurpriseTest,
		testing::Values(&finnhubWebData122, &finnhubWebData123, &finnhubWebData124,
			&finnhubWebData125, &finnhubWebData130));

	TEST_P(ParseFinnhubEPSSurpriseTest, TestParseFinnhubEPSSurprise0) {
		m_pvEPSSurprise = m_finnhubStockEstimatesEPSSurprise.ParseFinnhubEPSSurprise(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_EQ(m_pvEPSSurprise->size(), 0);
			break;
		case 3: //
			EXPECT_EQ(m_pvEPSSurprise->size(), 4);
			break;
		case 4: // 第二个数据缺缺actual
			EXPECT_EQ(m_pvEPSSurprise->size(), 4);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3)->m_dActual, 1.68);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3)->m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(3)->m_lDate, 20201231);
			EXPECT_STREQ(m_pvEPSSurprise->at(3)->m_strSymbol, _T("AAPL"));
			break;
		case 5: // 第三个数据缺CodeNo
			EXPECT_EQ(m_pvEPSSurprise->size(), 4);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3)->m_dActual, 1.68);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3)->m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(3)->m_lDate, 20201231);
			EXPECT_STREQ(m_pvEPSSurprise->at(3)->m_strSymbol, _T("AAPL"));
			break;
		case 10:
			EXPECT_EQ(m_pvEPSSurprise->size(), 4);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dActual, 0.6375);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dEstimate, 0.5765856);
			EXPECT_EQ(m_pvEPSSurprise->at(0)->m_lDate, 20200331);
			EXPECT_STREQ(m_pvEPSSurprise->at(0)->m_strSymbol, _T("AAPL"));
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3)->m_dActual, 1.68) << "成功处理后，自动按日期排列，导致其被放置于最后";
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3)->m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(3)->m_lDate, 20201231);
			EXPECT_STREQ(m_pvEPSSurprise->at(3)->m_strSymbol, _T("AAPL"));
			break;
		default:
			break;
		}
	}

	class ProcessFinnhubEPSSurpriseTest : public::testing::TestWithParam<FinnhubWebData*> {
	protected:
		virtual void SetUp(void) override {
			GeneralCheck();
			FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_pWorldMarket->GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_pWebData->CreatePropertyTree();
			m_pWebData->SetJSonContentType(true);
			m_finnhubStockEstimatesEPSSurprise.SetMarket(gl_pWorldMarket.get());
			m_finnhubStockEstimatesEPSSurprise.SetIndex(0);
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetProfileUpdated(false);
			m_pStock->SetUpdateProfileDB(false);
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubStockEstimatesEPSSurprise m_finnhubStockEstimatesEPSSurprise;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubEPSSurprise, ProcessFinnhubEPSSurpriseTest,
		testing::Values(&finnhubWebData122, &finnhubWebData123, &finnhubWebData124,
			&finnhubWebData125, &finnhubWebData130));

	TEST_P(ProcessFinnhubEPSSurpriseTest, TestProcessFinnhubEPSSurprise) {
		CWorldStockPtr pStock = gl_pWorldMarket->GetStock(0);
		bool fSucceed = m_finnhubStockEstimatesEPSSurprise.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 2: // 格式不对
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->m_fEPSSurpriseUpdated);
			EXPECT_TRUE(pStock->m_fEPSSurpriseNeedSave);
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->GetLastEPSSurpriseUpdateDate(), 19700101);
			break;
		case 3: //
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->m_fEPSSurpriseUpdated);
			EXPECT_TRUE(pStock->m_fEPSSurpriseNeedSave);
			EXPECT_FALSE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->m_vEPSSurprise.size(), 4);
			EXPECT_EQ(pStock->GetLastEPSSurpriseUpdateDate(), 19800101);
			break;
		case 4: // 第二个数据缺缺actual
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->m_fEPSSurpriseUpdated);
			EXPECT_TRUE(pStock->m_fEPSSurpriseNeedSave);
			EXPECT_EQ(pStock->m_vEPSSurprise.size(), 4);
			break;
		case 5: // 第三个数据缺CodeNo
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->m_fEPSSurpriseUpdated);
			EXPECT_TRUE(pStock->m_fEPSSurpriseNeedSave);
			EXPECT_EQ(pStock->m_vEPSSurprise.size(), 4);
			break;
		case 10:
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->m_fEPSSurpriseUpdated);
			EXPECT_TRUE(pStock->m_fEPSSurpriseNeedSave);
			EXPECT_EQ(pStock->m_vEPSSurprise.size(), 4);
			break;
		default:
			break;
		}
		// 恢复原状
		pStock->m_fEPSSurpriseUpdated = false;
		pStock->m_fEPSSurpriseNeedSave = false;
		pStock->SetUpdateProfileDB(false);
		pStock->m_vEPSSurprise.resize(0);
		pStock->SetLastEPSSurpriseUpdateDate(19800101);
	}
}