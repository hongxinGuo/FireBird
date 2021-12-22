#include"pch.h"

#include"globedef.h"
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
		// ��MockWorldMarketTest�������
	}

	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	FinnhubWebData finnhubWebData122(2, _T("AAPL"), _T("{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// ��һ������ȱ��actual
	FinnhubWebData finnhubWebData123(3, _T("AAPL"), _T("[{\"Missing\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// �ڶ�������ȱ��actual
	FinnhubWebData finnhubWebData124(4, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"Missing\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// ����������ȱ��actual
	FinnhubWebData finnhubWebData125(5, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"Missing\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// ��ȷ������
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
			m_pvEPSSurprise = nullptr;
		}
		virtual void TearDown(void) override {
			// clearup
			while (gl_systemMessage.GetErrorMessageDequeSize() > 0) gl_systemMessage.PopErrorMessage();
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
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvEPSSurprise->size(), 0);
			break;
		case 3: //
			EXPECT_EQ(m_pvEPSSurprise->size(), 0);
			break;
		case 4: // �ڶ�������ȱȱactual
			EXPECT_EQ(m_pvEPSSurprise->size(), 1);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dActual, 1.68);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(0)->m_lDate, 20201231);
			EXPECT_STREQ(m_pvEPSSurprise->at(0)->m_strSymbol, _T("AAPL"));
			break;
		case 5: // ����������ȱCodeNo
			EXPECT_EQ(m_pvEPSSurprise->size(), 2);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dActual, 1.68);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(0)->m_lDate, 20201231);
			EXPECT_STREQ(m_pvEPSSurprise->at(0)->m_strSymbol, _T("AAPL"));
			break;
		case 10:
			EXPECT_EQ(m_pvEPSSurprise->size(), 4);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dActual, 0.6375);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dEstimate, 0.5765856);
			EXPECT_EQ(m_pvEPSSurprise->at(0)->m_lDate, 20200331);
			EXPECT_STREQ(m_pvEPSSurprise->at(0)->m_strSymbol, _T("AAPL"));
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3)->m_dActual, 1.68) << "�ɹ�������Զ����������У������䱻���������";
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(3)->m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(3)->m_lDate, 20201231);
			EXPECT_STREQ(m_pvEPSSurprise->at(3)->m_strSymbol, _T("AAPL"));
			break;
		default:
			break;
		}
	}
}