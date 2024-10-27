#include"pch.h"

#include"GeneralCheck.h"

#include"WorldMarket.h"

#include"ProductFinnhubStockPriceQuote.h"

using namespace testing;

namespace FireBirdTest {
	class CFinnhubStockPriceQuoteTest : public Test {
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
		CProductFinnhubStockPriceQuote stockPriceQuote;
	};

	TEST_F(CFinnhubStockPriceQuoteTest, TestInitialize) {
		EXPECT_EQ(stockPriceQuote.GetIndex(), -1);
		EXPECT_STREQ(stockPriceQuote.GetInquiryFunction(), _T("https://finnhub.io/api/v1/quote?symbol="));
	}

	TEST_F(CFinnhubStockPriceQuoteTest, TestCreatMessage) {
		stockPriceQuote.SetMarket(gl_pWorldMarket);
		stockPriceQuote.SetIndex(1);
		EXPECT_STREQ(stockPriceQuote.CreateMessage(), stockPriceQuote.GetInquiryFunction() + gl_dataContainerFinnhubStock.GetStock(1)->GetSymbol());
	}

	TEST_F(CFinnhubStockPriceQuoteTest, TestProcessWebData) {
		// ��MockWorldMarketTest�������
	}

	// ��ʽ����(ȱ��ʼ�ġ�{�������޷�˳��Parser
	Test_FinnhubWebData finnhubWebData53(3, _T("AAPL"), _T("\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
	// ȱc
	Test_FinnhubWebData finnhubWebData54(4, _T("AAPL"), _T("{\"a\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
	// ȱh
	Test_FinnhubWebData finnhubWebData55(5, _T("AAPL"), _T("{\"c\":121.03,\"a\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
	// ȱl
	Test_FinnhubWebData finnhubWebData56(6, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"a\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));
	// ȱo
	Test_FinnhubWebData finnhubWebData57(7, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"a\":120.4,\"pc\":121.96,\"t\":1615507200}"));
	// ȱpc
	Test_FinnhubWebData finnhubWebData58(8, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"a\":121.96,\"t\":1615507200}"));
	// ȱ��t
	Test_FinnhubWebData finnhubWebData59(9, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"a\":1615507200}"));
	// ��ȷ������
	Test_FinnhubWebData finnhubWebData60(10, _T("AAPL"), _T("{\"c\":121.03,\"h\":121.17,\"l\":119.16,\"o\":120.4,\"pc\":121.96,\"t\":1615507200}"));

	class ProcessFinnhubStockQuoteTest : public TestWithParam<Test_FinnhubWebData*> {
	protected:
		void SetUp() override {
			SCOPED_TRACE("");
			GeneralCheck();
			const Test_FinnhubWebData* pData = GetParam();
			m_lIndex = pData->m_lIndex;
			m_pStock = gl_dataContainerFinnhubStock.GetStock(pData->m_strSymbol);
			EXPECT_TRUE(m_pStock != nullptr);
			m_pStock->SetNew(0);
			m_pStock->SetHigh(0);
			m_pStock->SetLow(0);
			m_pStock->SetOpen(0);
			m_pStock->SetLastClose(0);
			m_pStock->SetTransactionTime(0);
			m_pStock->SetActive(false);
			m_pStock->SetIPOStatus(_STOCK_NULL_);
			m_pStock->SetUpdateProfileDB(false);
			m_pWebData = pData->m_pData;
			m_finnhubStockPriceQuote.CheckAccessRight(m_pWebData);

			m_finnhubStockPriceQuote.SetMarket(gl_pWorldMarket);
			m_finnhubStockPriceQuote.SetIndex(gl_dataContainerFinnhubStock.GetOffset(pData->m_strSymbol));
		}

		void TearDown() override {
			// clearUp
			m_pStock->SetNew(0);
			m_pStock->SetHigh(0);
			m_pStock->SetLow(0);
			m_pStock->SetOpen(0);
			m_pStock->SetLastClose(0);
			m_pStock->SetTransactionTime(0);
			m_pStock->SetUpdateProfileDB(false);
			while (gl_systemMessage.ErrorMessageSize() > 0) gl_systemMessage.PopErrorMessage();

			SCOPED_TRACE("");
			GeneralCheck();
		}

	public:
		long m_lIndex;
		CFinnhubStockPtr m_pStock;
		CWebDataPtr m_pWebData;
		CProductFinnhubStockPriceQuote m_finnhubStockPriceQuote;
	};

	INSTANTIATE_TEST_SUITE_P(TestProcessFinnhubStockQuote1,
	                         ProcessFinnhubStockQuoteTest,
	                         testing::Values(&finnhubWebData0, &finnhubWebData1, &finnhubWebData53, &finnhubWebData54, &finnhubWebData55, &finnhubWebData56,
		                         &finnhubWebData57, &finnhubWebData58, &finnhubWebData58, &finnhubWebData60));

	TEST_P(ProcessFinnhubStockQuoteTest, TestParseFinnhubStockQuote0) {
		auto tt = gl_tUTCTime;
		if (m_lIndex == 10) {
			gl_tUTCTime = 1615507200;
		}
		m_finnhubStockPriceQuote.ParseAndStoreWebData(m_pWebData);
		switch (m_lIndex) {
		case 0: // ������
			break;
		case 1: // ��Ȩ�����ʵ�����
			break;
		case 3: // ��ʽ����
			break;
		case 4: // ȱ��c
			break;
		case 5: // ȱ��h��
			break;
		case 6: // ȱ��l��
			break;
		case 7: // ȱ��o��
			break;
		case 8: // ȱ��pc
			break;
		case 9: // ȱ��t��
			break;
		case 10: // ��ȷ������
			EXPECT_EQ(m_pStock->GetNew(), 121030);
			EXPECT_EQ(m_pStock->GetHigh(), 121170);
			EXPECT_EQ(m_pStock->GetLow(), 119160);
			EXPECT_EQ(m_pStock->GetOpen(), 120400);
			EXPECT_EQ(m_pStock->GetLastClose(), 121960);
			EXPECT_EQ(m_pStock->GetTransactionTime(), 1615507200);

			EXPECT_TRUE(m_pStock->IsActive());
			EXPECT_TRUE(m_pStock->IsIPOed());
			EXPECT_TRUE(m_pStock->IsUpdateProfileDB());

		// �ָ�ԭ״
			m_pStock->SetUpdateProfileDB(false);
			gl_tUTCTime = tt;
			break;
		default:
			break;
		}
	}
}
