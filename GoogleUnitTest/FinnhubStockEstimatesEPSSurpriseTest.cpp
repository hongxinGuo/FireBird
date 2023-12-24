#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubStockEstimatesEPSSurprise.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubStockEstimatesEPSSurpriseTest : public Test {
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
		CProductFinnhubStockEstimatesEPSSurprise stockEstimatesEPSSurprise;
	};

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestInitialize) {
		EXPECT_EQ(stockEstimatesEPSSurprise.GetIndex(), -1);
		EXPECT_STREQ(stockEstimatesEPSSurprise.GetInquiryFunction(), _T("https://finnhub.io/api/v1/stock/earnings?symbol="));
	}

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestCreatMessage) {
		stockEstimatesEPSSurprise.SetMarket(gl_pWorldMarket);
		stockEstimatesEPSSurprise.SetIndex(1);
		EXPECT_STREQ(stockEstimatesEPSSurprise.CreateMessage(), stockEstimatesEPSSurprise.GetInquiryFunction() + gl_containerStock.GetStock(1)->GetSymbol());
	}

	TEST_F(CFinnhubStockEstimatesEPSSurpriseTest, TestProcessWebData) {
		// ��MockWorldMarketTest�������
	}

	// ��ʽ����(ȱ��ʼ�ġ�[�������޷�˳��Parser
	Test_FinnhubWebData finnhubWebData122(2, _T("AAPL"), _T("{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// ��һ������ȱ��actual
	Test_FinnhubWebData finnhubWebData123(3, _T("AAPL"), _T("[{\"Missing\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// �ڶ�������ȱ��actual
	Test_FinnhubWebData finnhubWebData124(4, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"Missing\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// ����������ȱ��actual
	Test_FinnhubWebData finnhubWebData125(5, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"Missing\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));
	// ��ȷ������
	Test_FinnhubWebData finnhubWebData130(10, _T("AAPL"), _T("[{\"actual\":1.68,\"estimate\":1.555857,\"period\":\"2020-12-31\",\"symbol\":\"AAPL\"},{\"actual\":0.73,\"estimate\":0.7142244,\"period\":\"2020-09-30\",\"symbol\":\"AAPL\"},{\"actual\":0.645,\"estimate\":0.5211078,\"period\":\"2020-06-30\",\"symbol\":\"AAPL\"},{\"actual\":0.6375,\"estimate\":0.5765856,\"period\":\"2020-03-31\",\"symbol\":\"AAPL\"}]"));

	class ParseFinnhubEPSSurpriseTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_containerStock.GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
			m_pvEPSSurprise = nullptr;
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateCompanyProfile(true);
			m_pStock->SetUpdateProfileDB(false);
			SCOPED_TRACE("");
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
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData122, &finnhubWebData123, &finnhubWebData124,
		                         &finnhubWebData125, &finnhubWebData130));

	TEST_P(ParseFinnhubEPSSurpriseTest, TestParseFinnhubEPSSurprise0) {
		m_pvEPSSurprise = m_finnhubStockEstimatesEPSSurprise.ParseFinnhubEPSSurprise(m_pWebData);
		switch (m_lIndex) {
		case 0: // ������
			EXPECT_EQ(m_pvEPSSurprise->size(), 0);
			break;
		case 1: // ��Ȩ�����ʵ�����
			EXPECT_EQ(m_pvEPSSurprise->size(), 0);
			break;
		case 2: // ��ʽ����
			EXPECT_EQ(m_pvEPSSurprise->size(), 0);
			break;
		case 3: //��һ������ȱactual
			EXPECT_EQ(m_pvEPSSurprise->size(), 0) << "��һ�����ݼ�Ϊ�����";
			break;
		case 4: // �ڶ�������ȱȱactual
			EXPECT_EQ(m_pvEPSSurprise->size(), 1) << "�������ݲ��洢";
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dActual, 1.68);
			EXPECT_DOUBLE_EQ(m_pvEPSSurprise->at(0)->m_dEstimate, 1.555857);
			EXPECT_EQ(m_pvEPSSurprise->at(0)->m_lDate, 20201231);
			EXPECT_STREQ(m_pvEPSSurprise->at(0)->m_strSymbol, _T("AAPL"));
			break;
		case 5: // ����������ȱCodeNo
			EXPECT_EQ(m_pvEPSSurprise->size(), 2) << "�����������Ǵ����";
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

	class ProcessFinnhubEPSSurpriseTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_containerStock.GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pWebData = pData->m_pData;
			m_pWebData->CreateJson();
			m_pWebData->SetJSonContentType(true);
			m_finnhubStockEstimatesEPSSurprise.SetMarket(gl_pWorldMarket);
			m_finnhubStockEstimatesEPSSurprise.SetIndex(0);
		}

		void TearDown() override {
			// clearUp
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();
			m_pStock->SetUpdateCompanyProfile(true);
			m_pStock->SetUpdateProfileDB(false);
			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CWorldStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubStockEstimatesEPSSurprise m_finnhubStockEstimatesEPSSurprise;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubEPSSurprise, ProcessFinnhubEPSSurpriseTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData122, &finnhubWebData123, &finnhubWebData124,
		                         &finnhubWebData125, &finnhubWebData130));

	TEST_P(ProcessFinnhubEPSSurpriseTest, TestProcessFinnhubEPSSurprise) {
		CWorldStockPtr pStock = gl_containerStock.GetStock(0);
		bool fSucceed = m_finnhubStockEstimatesEPSSurprise.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // ������
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->m_fEPSSurpriseUpdated);
			EXPECT_TRUE(pStock->m_fEPSSurpriseNeedSave);
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->GetLastEPSSurpriseUpdateDate(), 19700101);
			break;
		case 1: // ��Ȩ�����ʵ�����
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->m_fEPSSurpriseUpdated);
			EXPECT_TRUE(pStock->m_fEPSSurpriseNeedSave);
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->GetLastEPSSurpriseUpdateDate(), 19700101);
			break;
		case 2: // ��ʽ����
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->m_fEPSSurpriseUpdated);
			EXPECT_TRUE(pStock->m_fEPSSurpriseNeedSave);
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->GetLastEPSSurpriseUpdateDate(), 19700101);
			break;
		case 3: //��һ������ȱactual
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->m_fEPSSurpriseUpdated);
			EXPECT_TRUE(pStock->m_fEPSSurpriseNeedSave);
			EXPECT_TRUE(pStock->IsUpdateProfileDB());
			EXPECT_EQ(pStock->m_vEPSSurprise.size(), 0);
			EXPECT_EQ(pStock->GetLastEPSSurpriseUpdateDate(), 19700101) << "����Ϊ��ʱ������������Ϊԭ��";
			break;
		case 4: // �ڶ�������ȱȱactual
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->m_fEPSSurpriseUpdated);
			EXPECT_TRUE(pStock->m_fEPSSurpriseNeedSave);
			EXPECT_EQ(pStock->m_vEPSSurprise.size(), 1);
			break;
		case 5: // ����������ȱCodeNo
			EXPECT_TRUE(fSucceed);
			EXPECT_TRUE(pStock->m_fEPSSurpriseUpdated);
			EXPECT_TRUE(pStock->m_fEPSSurpriseNeedSave);
			EXPECT_EQ(pStock->m_vEPSSurprise.size(), 2);
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
		// �ָ�ԭ״
		pStock->m_fEPSSurpriseUpdated = false;
		pStock->m_fEPSSurpriseNeedSave = false;
		pStock->SetUpdateProfileDB(false);
		pStock->m_vEPSSurprise.resize(0);
		pStock->SetLastEPSSurpriseUpdateDate(19800101);
	}
}
